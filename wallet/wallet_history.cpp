// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "wallet/wallet_history.h"

#include "wallet/wallet_common.h"
#include "wallet/wallet_phrases.h"
#include "base/unixtime.h"
#include "ui/address_label.h"
#include "ui/painter.h"
#include "ui/text/text.h"
#include "styles/style_wallet.h"
#include "styles/palette.h"

#include <QtCore/QDateTime>

namespace Wallet {
namespace {

constexpr auto kPreloadScreens = 3;

struct TransactionLayout {
	QDateTime dateTime;
	Ui::Text::String date;
	Ui::Text::String time;
	Ui::Text::String amount;
	Ui::Text::String address;
	Ui::Text::String comment;
	Ui::Text::String fees;
	int addressWidth = 0;
	int addressHeight = 0;
	bool incoming = false;
};

[[nodiscard]] const style::TextStyle &AddressStyle() {
	const static auto result = Ui::ComputeAddressStyle(st::defaultTextStyle);
	return result;
}

[[nodiscard]] TransactionLayout PrepareLayout(const Ton::Transaction &data) {
	auto result = TransactionLayout();
	result.dateTime = base::unixtime::parse(data.time);
	result.time.setText(
		st::defaultTextStyle,
		ph::lng_wallet_short_time(result.dateTime.time())(ph::now));
	result.amount.setText(
		st::semiboldTextStyle,
		ParseAmount(CalculateValue(data), true).full);
	const auto address = ExtractAddress(data);
	const auto addressPartWidth = [&](int from, int length = -1) {
		return AddressStyle().font->width(address.mid(from, length));
	};
	result.address = Ui::Text::String(
		AddressStyle(),
		address,
		_defaultOptions,
		st::walletAddressWidthMin);
	result.addressWidth = (AddressStyle().font->spacew / 2) + std::max(
		addressPartWidth(0, address.size() / 2),
		addressPartWidth(address.size() / 2));
	result.addressHeight = AddressStyle().font->height * 2;
	result.comment = Ui::Text::String(
		st::defaultTextStyle,
		ExtractMessage(data),
		_textPlainOptions,
		st::walletAddressWidthMin);
	if (data.fee) {
		const auto fee = ParseAmount(data.fee).full;
		result.fees.setText(
			st::defaultTextStyle,
			ph::lng_wallet_row_fees(ph::now).replace("{amount}", fee));
	}
	result.incoming = data.outgoing.empty();
	return result;
}

} // namespace

class HistoryRow final {
public:
	explicit HistoryRow(const Ton::Transaction &transaction);

	[[nodiscard]] Ton::TransactionId id() const;

	[[nodiscard]] QDateTime date() const;
	void setShowDate(bool show);

	[[nodiscard]] int top() const;
	void setTop(int top);

	void resizeToWidth(int width);
	[[nodiscard]] int height() const;
	[[nodiscard]] int bottom() const;

	void paint(Painter &p, int x, int y);
	[[nodiscard]] bool isUnderCursor(QPoint point) const;

private:
	Ton::TransactionId _id;
	TransactionLayout _layout;
	int _top = 0;
	int _width = 0;
	int _height = 0;

};

HistoryRow::HistoryRow(const Ton::Transaction &transaction)
: _id(transaction.id)
, _layout(PrepareLayout(transaction)) {
}

Ton::TransactionId HistoryRow::id() const {
	return _id;
}

QDateTime HistoryRow::date() const {
	return _layout.dateTime;
}

void HistoryRow::setShowDate(bool show) {
	if (!show) {
		_layout.date.clear();
	} else if (_id.lt != 0) {
		_layout.date.setText(
			st::semiboldTextStyle,
			ph::lng_wallet_short_date(_layout.dateTime.date())(ph::now));
	} else {
		_layout.date.setText(
			st::semiboldTextStyle,
			ph::lng_wallet_row_pending_date(ph::now));
	}
}

int HistoryRow::top() const {
	return _top;
}

void HistoryRow::setTop(int top) {
	_top = top;
}

void HistoryRow::resizeToWidth(int width) {
	if (_width == width) {
		return;
	}
	_width = width;
	const auto padding = st::walletRowPadding;
	const auto avail = width - padding.left() - padding.right();
	_height = 0;
	if (!_layout.date.isEmpty()) {
		_height += st::walletRowDateHeight;
	}
	_height += padding.top() + _layout.amount.minHeight();
	_height += st::walletRowAddressTop + _layout.addressHeight;
	if (!_layout.comment.isEmpty()) {
		_height += st::walletRowCommentTop
			+ _layout.comment.countHeight(avail);
	}
	if (!_layout.fees.isEmpty()) {
		_height += st::walletRowFeesTop + _layout.fees.minHeight();
	}
	_height += padding.bottom();
}

int HistoryRow::height() const {
	return _height;
}

int HistoryRow::bottom() const {
	return _top + _height;
}

void HistoryRow::paint(Painter &p, int x, int y) {
	const auto padding = st::walletRowPadding;
	const auto avail = _width - padding.left() - padding.right();
	x += padding.left();

	if (!_layout.date.isEmpty()) {
		p.setPen(st::windowFg);
		_layout.date.draw(p, x, y + st::walletRowDateTop, avail);
		y += st::walletRowDateHeight;
	} else {
		const auto shadowWidth = _width - padding.left();
		p.fillRect(x, y, shadowWidth, st::lineWidth, st::shadowFg);
	}
	y += padding.top();

	p.setPen(_layout.incoming ? st::boxTextFgGood : st::boxTextFgError);
	_layout.amount.draw(p, x, y, avail);

	const auto labelLeft = x
		+ _layout.amount.maxWidth()
		+ st::normalFont->spacew;
	p.setPen(st::windowFg);
	p.setFont(st::normalFont);
	p.drawText(
		labelLeft,
		y + st::normalFont->ascent,
		(_layout.incoming
			? ph::lng_wallet_row_from(ph::now)
			: ph::lng_wallet_row_to(ph::now)));

	p.setPen(st::windowSubTextFg);
	_layout.time.draw(p, x + avail - _layout.time.maxWidth(), y, avail);
	y += _layout.amount.minHeight();

	p.setPen(st::windowFg);
	y += st::walletRowAddressTop;
	_layout.address.drawElided(
		p,
		x,
		y,
		_layout.addressWidth,
		2,
		style::al_topleft,
		0,
		-1,
		0,
		true);
	y += _layout.addressHeight;

	if (!_layout.comment.isEmpty()) {
		y += st::walletRowCommentTop;
		_layout.comment.draw(p, x, y, avail);
		y += _layout.comment.countHeight(avail);
	}
	if (!_layout.fees.isEmpty()) {
		p.setPen(st::windowSubTextFg);
		y += st::walletRowFeesTop;
		_layout.fees.draw(p, x, y, avail);
	}
}

bool HistoryRow::isUnderCursor(QPoint point) const {
	auto y = top();
	if (!_layout.date.isEmpty()) {
		y += st::walletRowDateHeight;
	}
	return (point.y() >= y) && (point.y() < bottom());
}

History::History(
	not_null<Ui::RpWidget*> parent,
	rpl::producer<HistoryState> state,
	rpl::producer<Ton::LoadedSlice> loaded)
: _widget(parent) {
	setupContent(std::move(state), std::move(loaded));
}

History::~History() = default;

void History::updateGeometry(QPoint position, int width) {
	_widget.move(position);
	resizeToWidth(width);
}

void History::resizeToWidth(int width) {
	if (!width) {
		return;
	}
	auto height = (_pendingRows.empty() && _rows.empty())
		? 0
		: st::walletRowsSkip;
	for (const auto &row : ranges::view::concat(_pendingRows, _rows)) {
		row->setTop(height);
		row->resizeToWidth(width);
		height += row->height();
	}
	if (height > 0) {
		height += st::walletRowsSkip;
	}
	_widget.resize(width, height);
}

rpl::producer<int> History::heightValue() const {
	return _widget.heightValue();
}

void History::setVisibleTopBottom(int top, int bottom) {
	_visibleTop = top - _widget.y();
	_visibleBottom = bottom - _widget.y();
	if (_visibleBottom <= _visibleTop || !_previousId.lt || _rows.empty()) {
		return;
	}
	const auto visibleHeight = (_visibleBottom - _visibleTop);
	const auto preloadHeight = kPreloadScreens * visibleHeight;
	if (_visibleBottom + preloadHeight >= _widget.height()) {
		_preloadRequests.fire_copy(_previousId);
	}
}

rpl::producer<Ton::TransactionId> History::preloadRequests() const {
	return _preloadRequests.events();
}

rpl::producer<Ton::Transaction> History::viewRequests() const {
	return _viewRequests.events();
}

rpl::lifetime &History::lifetime() {
	return _widget.lifetime();
}

void History::setupContent(
		rpl::producer<HistoryState> &&state,
		rpl::producer<Ton::LoadedSlice> &&loaded) {
	std::move(
		state
	) | rpl::start_with_next([=](HistoryState &&state) {
		mergeState(std::move(state));
	}, lifetime());

	std::move(
		loaded
	) | rpl::filter([=](const Ton::LoadedSlice &slice) {
		return (slice.after == _previousId);
	}) | rpl::start_with_next([=](Ton::LoadedSlice &&slice) {
		_previousId = slice.data.previousId;
		_listData.insert(
			end(_listData),
			slice.data.list.begin(),
			slice.data.list.end());
		refreshRows();
	}, lifetime());

	_widget.paintRequest(
	) | rpl::start_with_next([=](QRect clip) {
		auto p = Painter(&_widget);
		paint(p, clip);
	}, lifetime());

	_widget.setAttribute(Qt::WA_MouseTracking);
	_widget.events(
	) | rpl::start_with_next([=](not_null<QEvent*> e) {
		switch (e->type()) {
		case QEvent::Leave: selectRow(-1); return;
		case QEvent::Enter:
		case QEvent::MouseMove: selectRowByMouse(); return;
		case QEvent::MouseButtonPress: pressRow(); return;
		case QEvent::MouseButtonRelease: releaseRow(); return;
		}
	}, lifetime());
}

void History::selectRow(int selected) {
	if (_selected == selected) {
		return;
	}
	_selected = selected;
	_widget.setCursor((_selected >= 0)
		? style::cur_pointer
		: style::cur_default);
}

void History::selectRowByMouse() {
	const auto point = _widget.mapFromGlobal(QCursor::pos());
	const auto from = ranges::upper_bound(
		_rows,
		point.y(),
		ranges::less(),
		&HistoryRow::bottom);
	const auto till = ranges::lower_bound(
		_rows,
		point.y(),
		ranges::less(),
		&HistoryRow::top);
	if (from != till && from == end(_rows)) {
		const auto b = ranges::upper_bound(
			_rows,
			point.y(),
			ranges::less(),
			&HistoryRow::bottom);
		int a = 0;
	}
	if (from != till && (*from)->isUnderCursor(point)) {
		selectRow(from - begin(_rows));
	} else {
		selectRow(-1);
	}
}

void History::pressRow() {
	_pressed = _selected;
}

void History::releaseRow() {
	Expects(_selected < int(_rows.size()));

	if (std::exchange(_pressed, -1) != _selected || _selected < 0) {
		return;
	}
	const auto i = ranges::find(
		_listData,
		_rows[_selected]->id(),
		&Ton::Transaction::id);
	Assert(i != end(_listData));
	_viewRequests.fire_copy(*i);
}

void History::paint(Painter &p, QRect clip) {
	if (_pendingRows.empty() && _rows.empty()) {
		return;
	}
	const auto paintRows = [&](
		const std::vector<std::unique_ptr<HistoryRow>> &rows) {
		const auto from = ranges::upper_bound(
			rows,
			clip.top(),
			ranges::less(),
			&HistoryRow::bottom);
		const auto till = ranges::lower_bound(
			rows,
			clip.top() + clip.height(),
			ranges::less(),
			&HistoryRow::top);
		for (const auto &row : ranges::make_subrange(from, till)) {
			row->paint(p, 0, row->top());
		}
	};
	paintRows(_pendingRows);
	paintRows(_rows);
}

History::ScrollState History::computeScrollState() const {
	const auto item = ranges::upper_bound(
		_rows,
		_visibleTop,
		ranges::less(),
		&HistoryRow::bottom);
	if (item == _rows.end()
		|| (item == _rows.begin()
			&& _rows.front()->id() == _listData.front().id)) {
		return ScrollState();
	}
	auto result = ScrollState();
	result.top = (*item)->id();
	result.offset = _visibleTop - (*item)->top();
	return result;
}

void History::mergeState(HistoryState &&state) {
	if (mergePendingChanged(std::move(state.pendingTransactions))) {
		refreshPending();
	}
	if (mergeListChanged(std::move(state.lastTransactions))) {
		refreshRows();
	}
}

bool History::mergePendingChanged(
		std::vector<Ton::PendingTransaction> &&list) {
	if (_pendingData == list) {
		return false;
	}
	_pendingData = std::move(list);
	return true;
}

bool History::mergeListChanged(Ton::TransactionsSlice &&data) {
	const auto i = _listData.empty()
		? data.list.cend()
		: ranges::find(std::as_const(data.list), _listData.front());
	if (i == data.list.cend()) {
		_listData = data.list | ranges::to_vector;
		_previousId = std::move(data.previousId);
		return true;
	} else if (i != data.list.cbegin()) {
		_listData.insert(begin(_listData), data.list.cbegin(), i);
		return true;
	}
	return false;
}

void History::refreshPending() {
	_pendingRows = ranges::view::all(
		_pendingData
	) | ranges::view::transform([](const Ton::PendingTransaction &data) {
		return std::make_unique<HistoryRow>(data.fake);
	}) | ranges::to_vector;

	if (!_pendingRows.empty()) {
		_pendingRows.front()->setShowDate(true);
	}
	resizeToWidth(_widget.width());
}

void History::refreshRows() {
	auto addedFront = std::vector<std::unique_ptr<HistoryRow>>();
	auto addedBack = std::vector<std::unique_ptr<HistoryRow>>();
	for (const auto &element : _listData) {
		if (!_rows.empty() && element.id == _rows.front()->id()) {
			break;
		}
		addedFront.push_back(std::make_unique<HistoryRow>(element));
	}
	if (!_rows.empty()) {
		const auto from = ranges::find(
			_listData,
			_rows.back()->id(),
			&Ton::Transaction::id);
		if (from != end(_listData)) {
			addedBack = ranges::make_subrange(
				from + 1,
				end(_listData)
			) | ranges::view::transform([](const Ton::Transaction &data) {
				return std::make_unique<HistoryRow>(data);
			}) | ranges::to_vector;
		}
	}
	if (addedFront.empty() && addedBack.empty()) {
		return;
	} else if (!addedFront.empty()) {
		if (addedFront.size() < _listData.size()) {
			addedFront.insert(
				end(addedFront),
				std::make_move_iterator(begin(_rows)),
				std::make_move_iterator(end(_rows)));
		}
		_rows = std::move(addedFront);
	}
	_rows.insert(
		end(_rows),
		std::make_move_iterator(begin(addedBack)),
		std::make_move_iterator(end(addedBack)));

	auto previous = QDate();
	for (const auto &row : _rows) {
		const auto current = row->date().date();
		row->setShowDate(current != previous);
		previous = current;
	}
	resizeToWidth(_widget.width());
}

rpl::producer<HistoryState> MakeHistoryState(
		rpl::producer<Ton::WalletViewerState> state) {
	return std::move(
		state
	) | rpl::map([](Ton::WalletViewerState &&state) {
		return HistoryState{
			std::move(state.wallet.lastTransactions),
			std::move(state.wallet.pendingTransactions)
		};
	});
}

} // namespace Wallet