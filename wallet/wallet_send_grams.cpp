// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "wallet/wallet_send_grams.h"

#include "wallet/wallet_phrases.h"
#include "wallet/wallet_common.h"
#include "ui/widgets/input_fields.h"
#include "ui/widgets/labels.h"
#include "ui/widgets/buttons.h"
#include "base/algorithm.h"
#include "base/qthelp_url.h"
#include "styles/style_wallet.h"
#include "styles/style_layers.h"
#include "styles/palette.h"

namespace Wallet {
namespace {

constexpr auto kMaxCommentSize = 500;

struct FixedAmount {
	QString text;
	int position = 0;
};

struct FixedAddress {
	PreparedInvoice invoice;
	int position = 0;
};

[[nodiscard]] QString AmountSeparator() {
	return ParseAmount(1).separator;
}

[[nodiscard]] PreparedInvoice ParseInvoice(QString invoice) {
	const auto prefix = qstr("transfer/");
	auto result = PreparedInvoice();

	const auto position = invoice.indexOf(prefix, 0, Qt::CaseInsensitive);
	if (position >= 0) {
		invoice = invoice.mid(position + prefix.size());
	}
	const auto paramsPosition = invoice.indexOf('?');
	if (paramsPosition >= 0) {
		const auto params = qthelp::url_parse_params(
			invoice.mid(paramsPosition + 1),
			qthelp::UrlParamNameTransform::ToLower);
		result.amount = params.value("amount").toULongLong();
		result.comment = params.value("text");
	}
	result.address = invoice.mid(0, paramsPosition).replace(
		QRegularExpression("[^a-zA-Z0-9_\\-]"),
		QString());
	return result;
}

[[nodiscard]] FixedAmount FixAmountInput(const QString &text, int position) {
	constexpr auto kMaxDigitsCount = 9;
	const auto separator = AmountSeparator();

	auto result = FixedAmount{ text, position };
	if (text.isEmpty()) {
		return result;
	} else if (text.startsWith('.')
		|| text.startsWith(',')
		|| text.startsWith(separator)) {
		result.text.prepend('0');
		++result.position;
	}
	auto separatorFound = false;
	auto digitsCount = 0;
	for (auto i = 0; i != result.text.size();) {
		const auto ch = result.text[i];
		const auto atSeparator = result.text.midRef(i).startsWith(separator);
		if (ch >= '0' && ch <= '9' && digitsCount < kMaxDigitsCount) {
			++i;
			++digitsCount;
			continue;
		} else if (!separatorFound
			&& (atSeparator || ch == '.' || ch == ',')) {
			separatorFound = true;
			if (!atSeparator) {
				result.text.replace(i, 1, separator);
			}
			digitsCount = 0;
			i += separator.size();
			continue;
		}
		result.text.remove(i, 1);
		if (result.position > i) {
			--result.position;
		}
	}
	return result;
}

[[nodiscard]] FixedAddress FixAddressInput(
		const QString &text,
		int position) {
	auto result = FixedAddress{ ParseInvoice(text), position };
	if (result.invoice.address != text) {
		const auto removed = std::max(
			int(text.size()) - int(result.invoice.address.size()),
			0);
		position = std::max(position - removed, 0);
	}
	return result;
}

} // namespace

void SendGramsBox(
		not_null<Ui::GenericBox*> box,
		const QString &invoice,
		rpl::producer<int64> balance,
		Fn<void(PreparedInvoice, Fn<void(InvoiceField)> error)> done) {
	const auto prepared = ParseInvoice(invoice);
	const auto funds = std::make_shared<int64>();

	box->setTitle(ph::lng_wallet_send_title());

	box->addTopButton(st::boxTitleClose, [=] { box->closeBox(); });

	AddBoxSubtitle(box, ph::lng_wallet_send_recipient());
	const auto address = box->addRow(object_ptr<Ui::InputField>(
		box,
		st::walletSendInput,
		Ui::InputField::Mode::NoNewlines,
		ph::lng_wallet_send_address(),
		prepared.address));
	const auto about = box->addRow(
		object_ptr<Ui::FlatLabel>(
			box,
			ph::lng_wallet_send_about(),
			st::walletSendAbout),
		st::walletSendAboutPadding);

	const auto subtitle = AddBoxSubtitle(box, ph::lng_wallet_send_amount());
	const auto amount = box->addRow(
		object_ptr<Ui::InputField>(
			box,
			st::walletInput,
			Ui::InputField::Mode::SingleLine,
			rpl::single("0" + AmountSeparator() + "0"),
			(prepared.amount > 0
				? ParseAmount(prepared.amount).full
				: QString())),
		st::walletSendAmountPadding);

	auto balanceText = rpl::combine(
		ph::lng_wallet_send_balance(),
		rpl::duplicate(balance)
	) | rpl::map([](QString &&phrase, int64 value) {
		return phrase.replace(
			"{amount}",
			ParseAmount(std::max(value, 0LL)).full);
	});

	const auto balanceLabel = Ui::CreateChild<Ui::FlatLabel>(
		subtitle->parentWidget(),
		std::move(balanceText),
		st::walletSendBalanceLabel);
	rpl::combine(
		subtitle->geometryValue(),
		balanceLabel->widthValue()
	) | rpl::start_with_next([=](QRect rect, int innerWidth) {
		const auto top = rect.top()
			+ st::walletSubsectionTitle.style.font->ascent
			- st::walletSendBalanceLabel.style.font->ascent;
		balanceLabel->moveToRight(st::boxRowPadding.right(), top);
	}, balanceLabel->lifetime());

	const auto comment = box->addRow(
		object_ptr<Ui::InputField>(
			box,
			st::walletInput,
			Ui::InputField::Mode::MultiLine,
			ph::lng_wallet_send_comment(),
			prepared.comment),
		st::walletSendAmountPadding);
	comment->setMaxLength(kMaxCommentSize);

	const auto checkFunds = [=](const QString &amount) {
		if (const auto value = ParseAmountString(amount)) {
			const auto insufficient = (*value > std::max(*funds, 0LL));
			balanceLabel->setTextColorOverride(insufficient
				? std::make_optional(st::boxTextFgError->c)
				: std::nullopt);
		}
	};
	std::move(
		balance
	) | rpl::start_with_next([=](int64 value) {
		*funds = value;
		checkFunds(amount->getLastText());
	}, amount->lifetime());

	Ui::Connect(address, &Ui::InputField::changed, [=] {
		Ui::PostponeCall(address, [=] {
			const auto position = address->textCursor().position();
			const auto now = address->getLastText();
			const auto fixed = FixAddressInput(now, position);
			if (fixed.invoice.address != now) {
				address->setText(fixed.invoice.address);
				address->setFocusFast();
				address->setCursorPosition(fixed.position);
			}
			if (fixed.invoice.amount > 0) {
				amount->setText(ParseAmount(fixed.invoice.amount).full);
			}
			if (!fixed.invoice.comment.isEmpty()) {
				comment->setText(fixed.invoice.comment);
			}
		});
	});

	Ui::Connect(amount, &Ui::InputField::changed, [=] {
		Ui::PostponeCall(amount, [=] {
			const auto position = amount->textCursor().position();
			const auto now = amount->getLastText();
			const auto fixed = FixAmountInput(now, position);
			checkFunds(fixed.text);
			if (fixed.text == now) {
				return;
			}
			amount->setText(fixed.text);
			amount->setFocusFast();
			amount->setCursorPosition(fixed.position);
		});
	});

	box->setFocusCallback([=] {
		if (prepared.address.isEmpty()
			|| address->getLastText() != prepared.address) {
			address->setFocusFast();
		} else {
			amount->setFocusFast();
		}
	});

	const auto showError = crl::guard(box, [=](InvoiceField field) {
		switch (field) {
		case InvoiceField::Address: address->showError(); return;
		case InvoiceField::Amount: amount->showError(); return;
		case InvoiceField::Comment: comment->showError(); return;
		}
		Unexpected("Field value in SendGramsBox error callback.");
	});
	const auto submit = [=] {
		auto collected = PreparedInvoice();
		const auto parsed = ParseAmountString(amount->getLastText());
		if (!parsed) {
			amount->showError();
			return;
		}
		collected.amount = *parsed;
		collected.address = address->getLastText();
		collected.comment = comment->getLastText();
		done(collected, showError);
	};
	box->addButton(
		ph::lng_wallet_send_button(),
		submit,
		st::walletBottomButton
	)->setTextTransform(Ui::RoundButton::TextTransform::NoTransform);
}

} // namespace Wallet