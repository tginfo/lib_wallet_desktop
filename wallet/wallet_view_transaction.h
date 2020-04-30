// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/layers/generic_box.h"

namespace Ton {
struct Transaction;
} // namespace Ton

namespace Wallet {

void ViewTransactionBox(
	not_null<Ui::GenericBox*> box,
	Ton::Transaction &&data,
	rpl::producer<not_null<std::vector<Ton::Transaction>*>> collectEncrypted,
	rpl::producer<not_null<const std::vector<Ton::Transaction>*>> decrypted,
	Fn<void(QImage, QString)> share,
	Fn<void()> decryptComment,
	Fn<void(QString)> send);

} // namespace Wallet
