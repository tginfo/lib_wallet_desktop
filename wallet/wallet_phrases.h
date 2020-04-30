// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include "ui/ph.h"

class QDate;
class QTime;

namespace ph {

extern phrase lng_wallet_cancel;
extern phrase lng_wallet_continue;
extern phrase lng_wallet_done;
extern phrase lng_wallet_save;
extern phrase lng_wallet_warning;
extern phrase lng_wallet_error;
extern phrase lng_wallet_ok;

extern phrase lng_wallet_copy_address;

extern phrase lng_wallet_intro_title;
extern phrase lng_wallet_intro_description;
extern phrase lng_wallet_intro_create;
extern phrase lng_wallet_intro_import;

extern phrase lng_wallet_import_title;
extern phrase lng_wallet_import_description;
extern phrase lng_wallet_import_dont_have;
extern phrase lng_wallet_import_incorrect_title;
extern phrase lng_wallet_import_incorrect_text;
extern phrase lng_wallet_import_incorrect_retry;

extern phrase lng_wallet_too_bad_title;
extern phrase lng_wallet_too_bad_description;
extern phrase lng_wallet_too_bad_enter_words;

extern phrase lng_wallet_created_title;
extern phrase lng_wallet_created_description;

extern phrase lng_wallet_words_title;
extern phrase lng_wallet_words_description;
extern phrase lng_wallet_words_sure_title;
extern phrase lng_wallet_words_sure_text;
extern phrase lng_wallet_words_sure_ok;

extern phrase lng_wallet_check_title;
extern phrase lng_wallet_check_description;
extern phrase lng_wallet_check_incorrect_title;
extern phrase lng_wallet_check_incorrect_text;
extern phrase lng_wallet_check_incorrect_view;
extern phrase lng_wallet_check_incorrect_retry;

extern phrase lng_wallet_set_passcode_title;
extern phrase lng_wallet_set_passcode_description;
extern phrase lng_wallet_set_passcode_enter;
extern phrase lng_wallet_set_passcode_repeat;

extern phrase lng_wallet_change_passcode_title;
extern phrase lng_wallet_change_passcode_old;
extern phrase lng_wallet_change_passcode_new;
extern phrase lng_wallet_change_passcode_repeat;
extern phrase lng_wallet_change_passcode_done;

extern phrase lng_wallet_ready_title;
extern phrase lng_wallet_ready_description;
extern phrase lng_wallet_ready_show_account;

extern phrase lng_wallet_sync;
extern phrase lng_wallet_sync_percent;
extern phrase lng_wallet_refreshing;
extern phrase lng_wallet_refreshed_just_now;

extern phrase lng_wallet_cover_balance;
extern phrase lng_wallet_cover_balance_test;
extern phrase lng_wallet_cover_locked;
extern phrase lng_wallet_cover_receive_full;
extern phrase lng_wallet_cover_receive;
extern phrase lng_wallet_cover_send;

extern phrase lng_wallet_update;
extern phrase lng_wallet_update_short;

extern phrase lng_wallet_empty_history_title;
extern phrase lng_wallet_empty_history_welcome;
extern phrase lng_wallet_empty_history_address;

extern phrase lng_wallet_row_from;
extern phrase lng_wallet_row_to;
extern phrase lng_wallet_row_init;
extern phrase lng_wallet_row_service;
extern phrase lng_wallet_row_fees;
extern phrase lng_wallet_row_pending_date;
extern phrase lng_wallet_click_to_decrypt;
extern phrase lng_wallet_decrypt_failed;

extern phrase lng_wallet_view_title;
extern phrase lng_wallet_view_transaction_fee;
extern phrase lng_wallet_view_storage_fee;
extern phrase lng_wallet_view_sender;
extern phrase lng_wallet_view_recipient;
extern phrase lng_wallet_view_date;
extern phrase lng_wallet_view_comment;
extern phrase lng_wallet_view_send_to_address;
extern phrase lng_wallet_view_send_to_recipient;

extern phrase lng_wallet_receive_title;
extern phrase lng_wallet_receive_description;
extern phrase lng_wallet_receive_description_test;
extern phrase lng_wallet_receive_create_invoice;
extern phrase lng_wallet_receive_share;
extern phrase lng_wallet_receive_copied;
extern phrase lng_wallet_receive_address_copied;
extern phrase lng_wallet_receive_copied_qr;

extern phrase lng_wallet_invoice_title;
extern phrase lng_wallet_invoice_amount;
extern phrase lng_wallet_invoice_number;
extern phrase lng_wallet_invoice_comment;
extern phrase lng_wallet_invoice_comment_about;
extern phrase lng_wallet_invoice_url;
extern phrase lng_wallet_invoice_copy_url;
extern phrase lng_wallet_invoice_url_about;
extern phrase lng_wallet_invoice_url_about_test;
extern phrase lng_wallet_invoice_generate_qr;
extern phrase lng_wallet_invoice_share;
extern phrase lng_wallet_invoice_qr_title;
extern phrase lng_wallet_invoice_qr_amount;
extern phrase lng_wallet_invoice_qr_comment;
extern phrase lng_wallet_invoice_qr_share;
extern phrase lng_wallet_invoice_copied;

extern phrase lng_wallet_menu_settings;
extern phrase lng_wallet_menu_change_passcode;
extern phrase lng_wallet_menu_export;
extern phrase lng_wallet_menu_delete;

extern phrase lng_wallet_delete_title;
extern phrase lng_wallet_delete_about;
extern phrase lng_wallet_delete_disconnect;

extern phrase lng_wallet_send_title;
extern phrase lng_wallet_send_recipient;
extern phrase lng_wallet_send_address;
extern phrase lng_wallet_send_about;
extern phrase lng_wallet_send_amount;
extern phrase lng_wallet_send_balance;
extern phrase lng_wallet_send_comment;
extern phrase lng_wallet_send_button;
extern phrase lng_wallet_send_button_amount;

extern phrase lng_wallet_send_failed_title;
extern phrase lng_wallet_send_failed_text;

extern phrase lng_wallet_confirm_title;
extern phrase lng_wallet_confirm_text;
extern phrase lng_wallet_confirm_fee;
extern phrase lng_wallet_confirm_send;
extern phrase lng_wallet_confirm_warning;

extern phrase lng_wallet_same_address_title;
extern phrase lng_wallet_same_address_text;
extern phrase lng_wallet_same_address_proceed;

extern phrase lng_wallet_passcode_title;
extern phrase lng_wallet_passcode_enter;
extern phrase lng_wallet_passcode_next;
extern phrase lng_wallet_passcode_incorrect;

extern phrase lng_wallet_sending_title;
extern phrase lng_wallet_sending_text;

extern phrase lng_wallet_sent_title;
extern phrase lng_wallet_sent_close;

extern phrase lng_wallet_settings_title;
extern phrase lng_wallet_settings_version_title;
extern phrase lng_wallet_settings_autoupdate;
extern phrase lng_wallet_settings_version;
extern phrase lng_wallet_settings_checking;
extern phrase lng_wallet_settings_latest;
extern phrase lng_wallet_settings_check;
extern phrase lng_wallet_settings_downloading;
extern phrase lng_wallet_settings_ready;
extern phrase lng_wallet_settings_fail;
extern phrase lng_wallet_settings_update;
extern phrase lng_wallet_settings_configuration;
extern phrase lng_wallet_settings_update_config;
extern phrase lng_wallet_settings_config_url;
extern phrase lng_wallet_settings_config_from_file;
extern phrase lng_wallet_settings_mainnet;
extern phrase lng_wallet_settings_testnet;
extern phrase lng_wallet_settings_blockchain_name;

extern phrase lng_wallet_warning_reconnect;
extern phrase lng_wallet_warning_blockchain_name;
extern phrase lng_wallet_warning_to_testnet;
extern phrase lng_wallet_warning_to_mainnet;
extern phrase lng_wallet_bad_config;
extern phrase lng_wallet_bad_config_url;
extern phrase lng_wallet_wait_pending;
extern phrase lng_wallet_wait_syncing;

extern phrase lng_wallet_downloaded;

extern Fn<phrase(int)> lng_wallet_refreshed_minutes_ago;
extern Fn<phrase(QDate)> lng_wallet_short_date;
extern Fn<phrase(QTime)> lng_wallet_short_time;
extern Fn<phrase(QString)> lng_wallet_grams_count;
extern Fn<phrase(QString)> lng_wallet_grams_count_sent;

} // namespace ph

namespace Wallet {

inline constexpr auto kPhrasesCount = 163;

void SetPhrases(
	ph::details::phrase_value_array<kPhrasesCount> data,
	Fn<rpl::producer<QString>(int)> wallet_refreshed_minutes_ago,
	Fn<rpl::producer<QString>(QDate)> wallet_short_date,
	Fn<rpl::producer<QString>(QTime)> wallet_short_time,
	Fn<rpl::producer<QString>(QString)> wallet_grams_count,
	Fn<rpl::producer<QString>(QString)> wallet_grams_count_sent);

} // namespace Wallet
