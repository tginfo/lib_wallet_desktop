// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "ui/address_label.h"

#include "wallet/wallet_phrases.h"
#include "ui/widgets/labels.h"
#include "styles/style_widgets.h"
#include "styles/style_wallet.h"
#include "styles/palette.h"

#include <QtGui/QPainter>

namespace Ui {

style::TextStyle ComputeAddressStyle(const style::TextStyle &parent) {
	auto result = parent;
	result.font = result.font->monospace();
	result.linkFont = result.linkFont->monospace();
	result.linkFontOver = result.linkFontOver->monospace();
	return result;
}

not_null<RpWidget*> CreateAddressLabel(
		not_null<RpWidget*> parent,
		const QString &text,
		const style::FlatLabel &st,
		Fn<void()> onClickOverride,
		std::optional<QColor> bg) {
	const auto mono = parent->lifetime().make_state<style::FlatLabel>(st);
	mono->style = ComputeAddressStyle(mono->style);
	mono->minWidth = 50;

	const auto result = CreateChild<Ui::RpWidget>(parent.get());
	const auto label = CreateChild<Ui::FlatLabel>(
		result,
		rpl::single(text),
		*mono);
	label->setBreakEverywhere(true);
	if (onClickOverride) {
		label->setAttribute(Qt::WA_TransparentForMouseEvents);
		result->setCursor(style::cur_pointer);
		result->events(
		) | rpl::filter([=](not_null<QEvent*> event) {
			return (event->type() == QEvent::MouseButtonRelease);
		}) | rpl::start_with_next(onClickOverride, result->lifetime());
	} else {
		label->setDoubleClickSelectsParagraph(true);
		label->setContextCopyText(ph::lng_wallet_copy_address(ph::now));
	}

	const auto half = text.size() / 2;
	const auto first = text.mid(0, half);
	const auto second = text.mid(half);
	const auto width = std::max(
		mono->style.font->width(first),
		mono->style.font->width(second)
	) + mono->style.font->spacew / 2;
	label->resizeToWidth(width);
	if (!onClickOverride) {
		label->setSelectable(true);
	}
	result->resize(label->size());
	result->widthValue(
	) | rpl::start_with_next([=](int width) {
		if (st.align & Qt::AlignHCenter) {
			label->moveToLeft(
				(width - label->widthNoMargins()) / 2,
				label->getMargins().top(),
				width);
		} else {
			label->moveToLeft(0, label->getMargins().top(), width);
		}
	}, result->lifetime());

	if (bg) {
		result->paintRequest(
		) | rpl::start_with_next([=] {
			auto p = QPainter(result);
			auto hq = PainterHighQualityEnabler(p);
			p.setBrush(*bg);
			p.setPen(Qt::NoPen);
			p.drawRoundedRect(
				result->rect(),
				st::roundRadiusSmall,
				st::roundRadiusSmall);
		}, result->lifetime());
	}

	return result;
}

} // namespace Ui
