// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "wallet/wallet_phrases.h"

#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QtMath>

namespace ph {

namespace {

struct ShortenedCount {
	int64 number = 0;
	QString string;
};

QString FormatDouble(float64 value) {
	auto result = QString::number(value, 'f', 6);
	while (result.endsWith('0')) {
		result.chop(1);
	}
	if (result.endsWith('.')) {
		result.chop(1);
	}
	return result;
}

ShortenedCount FormatCountToShort(int64 number) {
	auto result = ShortenedCount{ number };
	const auto abs = std::abs(number);
	const auto shorten = [&](int64 divider, char multiplier) {
		const auto sign = (number > 0) ? 1 : -1;
		const auto rounded = abs / (divider / 10);
		result.string = QString::number(sign * rounded / 10);
		if (rounded % 10) {
			result.string += '.' + QString::number(rounded % 10) + multiplier;
		} else {
			result.string += multiplier;
		}
		// Update given number.
		// E.g. 12345 will be 12000.
		result.number = rounded * divider;
	};
	if (abs >= 1'000'000) {
		shorten(1'000'000, 'M');
	} else if (abs >= 10'000) {
		shorten(1'000, 'K');
	} else {
		result.string = QString::number(number);
	}
	return result;
}

QString ChoosePlural(float64 value, QString one, QString few, QString many, QString other) {
	// To correctly select a shift for PluralType::Short
	// we must first round the number.
	const auto shortened = FormatCountToShort(qRound(value));

	// Simplified.
	const auto n = std::abs(shortened.number ? float64(shortened.number) : value);
	const auto i = qFloor(n);
	const auto integer = (qCeil(n) == i);
	const auto formatted = integer ? QString() : FormatDouble(n);
	const auto dot = formatted.indexOf('.');
	const auto fraction = (dot >= 0) ? formatted.mid(dot + 1) : QString();
	const auto v = fraction.size();

	if (v == 0) {
		const auto mod10 = (i % 10);
		const auto mod100 = (i % 100);
		if ((mod10 >= 2) && (mod10 <= 4) && (mod100 < 12 || mod100 > 14)) {
			return few;
		} else if ((mod10 == 1) && (mod100 != 11)) {
			return one;
		}
		return many;
	}
	return other;
}

} // namespace

const auto walletCountStart = start_phrase_count();

phrase lng_wallet_cancel = "Отмена";
phrase lng_wallet_continue = "Продолжить";
phrase lng_wallet_done = "Готово";
phrase lng_wallet_save = "Сохранить";
phrase lng_wallet_warning = "Внимание";
phrase lng_wallet_error = "Ошибка";
phrase lng_wallet_ok = "Ок";

phrase lng_wallet_copy_address = "Копировать адрес кошелька";

phrase lng_wallet_intro_title = "Gram Wallet";
phrase lng_wallet_intro_description = "Кошелёк Gram позволяет вам совершать быстрые\nи надёжные переводы на основе блокчейна\nбез посредников";
phrase lng_wallet_intro_create = "Создать кошелёк";
phrase lng_wallet_intro_import = "Импорт существующего кошелька";

phrase lng_wallet_import_title = "24 секретных слова";
phrase lng_wallet_import_description = "Вы можете восстановить доступ к кошельку,\n введя 24 слова,\nкоторые вы записали при создании кошелька.";
phrase lng_wallet_import_dont_have = "У меня их нет";
phrase lng_wallet_import_incorrect_title = "Неверные слова";
phrase lng_wallet_import_incorrect_text = "К сожалению, вы ввели неверные секретные слова. Пожалуйста, перепроверьте и попробуйте ещё раз.";
phrase lng_wallet_import_incorrect_retry = "Ещё раз";

phrase lng_wallet_too_bad_title = "Очень плохо!";
phrase lng_wallet_too_bad_description = "Без секретных слов вы не можете восстановить доступ к кошельку.";
phrase lng_wallet_too_bad_enter_words = "Введите 24 секретных слова";

phrase lng_wallet_created_title = "Поздравляем!";
phrase lng_wallet_created_description = "Ваш Gram-колешёк был создан.\nТолько вы его контролируете.\n\nЧтобы не потерять к нему доступ,\nзапишите секретные слова и установите надёжный пароль.";

phrase lng_wallet_words_title = "24 секретных слова";
phrase lng_wallet_words_description = "Запишите эти 24 слова в правильном порядке, и сохраните их в надёжном месте.\n\nИспользуйте эти секретные слова для восстановления доступа к кошельку, если вы потеряете пароль или аккаунт Telegram.";
phrase lng_wallet_words_sure_title = "Точно готово?";
phrase lng_wallet_words_sure_text = "Вы потратили недостаточно времени для записи слов.";
phrase lng_wallet_words_sure_ok = "Oк, простите";

phrase lng_wallet_check_title = "Время теста!";
phrase lng_wallet_check_description = "Теперь давайте проверим, записали ли вы секретные слова правильно.\n\nВведите слова под номерами {index1}, {index2} и {index3} ниже:";
phrase lng_wallet_check_incorrect_title = "Неверные слова";
phrase lng_wallet_check_incorrect_text = "Введённые вами секретные слова не совпадают с теми, что были в списке.";
phrase lng_wallet_check_incorrect_view = "Просмотреть слова";
phrase lng_wallet_check_incorrect_retry = "Ещё раз";

phrase lng_wallet_set_passcode_title = "Локальный пароль";
phrase lng_wallet_set_passcode_description = "Теперь установите пароль \nдля подтверждения ваших транзакций.";
phrase lng_wallet_set_passcode_enter = "Введите пароль";
phrase lng_wallet_set_passcode_repeat = "Повторите пароль";

phrase lng_wallet_change_passcode_title = "Измегить локальный пароль";
phrase lng_wallet_change_passcode_old = "Введите старый пароль";
phrase lng_wallet_change_passcode_new = "Введите новый пароль";
phrase lng_wallet_change_passcode_repeat = "Повторите новый пароль";
phrase lng_wallet_change_passcode_done = "Пароль успешно изменён.";

phrase lng_wallet_ready_title = "Всё готово!";
phrase lng_wallet_ready_description = "Всё настроено. Теперь у вас есть кошелёк,\nкоторый контролируете только вы\xe2\x80\x93 напрямую,\nбез посредников или банкиров.";
phrase lng_wallet_ready_show_account = "Открыть кошелёк";

phrase lng_wallet_sync = "синзронизация...";
phrase lng_wallet_sync_percent = "синхронизация... {percent}%";
phrase lng_wallet_refreshing = "обновление...";
phrase lng_wallet_refreshed_just_now = "обновлено только что";

phrase lng_wallet_cover_balance = "Ваш баланс";
phrase lng_wallet_cover_receive_full = "Получить грамы";
phrase lng_wallet_cover_receive = "Получить";
phrase lng_wallet_cover_send = "Отправить";

phrase lng_wallet_update = "Обновить кошелёк";
phrase lng_wallet_update_short = "Обновить";

phrase lng_wallet_empty_history_title = "Кошелёк создан";
phrase lng_wallet_empty_history_welcome = "Добро пожаловать";
phrase lng_wallet_empty_history_address = "Адрес вашего кошелька";

phrase lng_wallet_row_from = "от:";
phrase lng_wallet_row_to = "на:";
phrase lng_wallet_row_fees = "комиссия блокчейна: {amount}";
phrase lng_wallet_row_pending_date = "Ожидание";
phrase lng_wallet_click_to_decrypt = "Введите пароль чтобы увидеть комментарий";
phrase lng_wallet_decrypt_failed = "Ошибка дешифрации :(";

phrase lng_wallet_view_title = "Транзакция";
phrase lng_wallet_view_transaction_fee = "{amount} коммиссия хранилища";
phrase lng_wallet_view_storage_fee = "{amount} комиссия транзакции";
phrase lng_wallet_view_sender = "Отправитель";
phrase lng_wallet_view_recipient = "Получатель";
phrase lng_wallet_view_date = "Дата";
phrase lng_wallet_view_comment = "Комментарий";
phrase lng_wallet_view_send_to_address = "Отправить грамы на этот адрес";
phrase lng_wallet_view_send_to_recipient = "Отправить грамы этому получателю";

phrase lng_wallet_receive_title = "Получить грамы";
phrase lng_wallet_receive_description = "Поделитесь этой ссылкой, чтобы получить тестовые Gram. Важно: эта ссылка не будет работать для настоящих токенов.";
phrase lng_wallet_receive_create_invoice = "Выставить счёт";
phrase lng_wallet_receive_share = "Поделиться адресом кошелька";
phrase lng_wallet_receive_copied = "Адрес скопирован в буфер.";
phrase lng_wallet_receive_copied_qr = "QR-код скопирован в буфер.";

phrase lng_wallet_invoice_title = "Выставить счёт";
phrase lng_wallet_invoice_amount = "Сумма";
phrase lng_wallet_invoice_number = "Сумма в граммах, которую вы хотите получить";
phrase lng_wallet_invoice_comment = "Комментарий (необязательно)";
phrase lng_wallet_invoice_comment_about = "Вы можете указать сумму и назначение платежа, чтобы упростить жизнь отправителю.";
phrase lng_wallet_invoice_url = "Ссылка на счёт";
phrase lng_wallet_invoice_copy_url = "Скопировать ссылку на счёт";
phrase lng_wallet_invoice_url_about = ">Поделитесь этой ссылкой, чтобы получить тестовые Gram. Важно: эта ссылка не будет работать для настоящих токенов.";
phrase lng_wallet_invoice_generate_qr = "Сгенерировать QR-код";
phrase lng_wallet_invoice_share = "Поделиться ссылкой на счёт";
phrase lng_wallet_invoice_qr_title = "QR-код";
phrase lng_wallet_invoice_qr_amount = "Ожидаемая сумма";
phrase lng_wallet_invoice_qr_comment = "Комментарий";
phrase lng_wallet_invoice_qr_share = "Поделиться QR-кодом";
phrase lng_wallet_invoice_copied = "Ссылка на счёт скопирована в буфер.";

phrase lng_wallet_menu_settings = "Настройки";
phrase lng_wallet_menu_change_passcode = "Поменять пароль";
phrase lng_wallet_menu_export = "Экспорт кошелька";
phrase lng_wallet_menu_delete = "Удалить кошелёк";

phrase lng_wallet_delete_title = "Удалить кошелёк";
phrase lng_wallet_delete_about = "Это действие отключит кошелёк от приложения. Вы сможете восстановить кошелёк, введя 24 секретных слова  \xe2\x80\x93 – или импортировать другой кошелёк.\n\nКошельки расположены внутри блокчейна TON, который не контролируется Telegram. Если вы хотите удалить кошелёк, просто переведите с него все Gram и оставьте пустым.";
phrase lng_wallet_delete_disconnect = "Удалить";

phrase lng_wallet_send_title = "Отправить грамы";
phrase lng_wallet_send_recipient = "Адрес кошелька получателя";
phrase lng_wallet_send_address = "Введите адрес кошелька";
phrase lng_wallet_send_about = "Скопируйте 48-значный адрес кошелька получателя сюда, или попросите его отправить вам ссылку вида ton://.";
phrase lng_wallet_send_amount = "Сумма";
phrase lng_wallet_send_balance = "Баланс: {amount}";
phrase lng_wallet_send_comment = "Комментарий (необязательно)";
phrase lng_wallet_send_button = "Отправить грамы";
phrase lng_wallet_send_button_amount = "Отправить {grams}";

phrase lng_wallet_send_failed_title = "Ошибка отправки";
phrase lng_wallet_send_failed_text = "Не удалось выполнить транзакцию. Пожалуйста, проверьте баланс своего кошелька и попробуйте снова.";

phrase lng_wallet_confirm_title = "Подтверждение";
phrase lng_wallet_confirm_text = "Вы хотите отправить **{grams}** на:";
phrase lng_wallet_confirm_fee = "Комиссия: ~{grams}";
phrase lng_wallet_confirm_send = "Отправить грамы";
phrase lng_wallet_confirm_warning = "**Заметьте:** ваш комментарий \xC2\xAB{comment}\xC2\xBB **не зашифрован**.";

phrase lng_wallet_same_address_title = "Внимание";
phrase lng_wallet_same_address_text = "Отправка Gram на свой кошелёк бессмысленна, вы просто потратите часть суммы на комиссию блокчейна.";
phrase lng_wallet_same_address_proceed = "Продолжить";

phrase lng_wallet_passcode_title = "Пароль";
phrase lng_wallet_passcode_enter = "Введите ваш локальный пароль";
phrase lng_wallet_passcode_next = "Далее";
phrase lng_wallet_passcode_incorrect = "Неверный пароль.";

phrase lng_wallet_sending_title = "Отправление грамов";
phrase lng_wallet_sending_text = "Подождите пару секунд для обработки транзакции...;

phrase lng_wallet_sent_title = "Готово!";
phrase lng_wallet_sent_close = "Закрыть";

phrase lng_wallet_settings_title = "Настройки";
phrase lng_wallet_settings_version_title = "Версия и обновление";
phrase lng_wallet_settings_autoupdate = "Автоматическое обновление";
phrase lng_wallet_settings_version = "Версия {version}";
phrase lng_wallet_settings_checking = "Проверка обновлений...";
phrase lng_wallet_settings_latest = "Установлена последняя версия";
phrase lng_wallet_settings_check = "Проверить обновление";
phrase lng_wallet_settings_downloading = "Скачивание обновления {progress}...";
phrase lng_wallet_settings_ready = "Обновление готово";
phrase lng_wallet_settings_fail = "Ошибка проверки обновления :(";
phrase lng_wallet_settings_update = "Обновить кошелёк";
phrase lng_wallet_settings_configuration = "Настройки сервера";
phrase lng_wallet_settings_update_config = "Изменить источник автоматически";
phrase lng_wallet_settings_config_url = "Изменить источник";
phrase lng_wallet_settings_config_from_file = "Загрузить из файла";
phrase lng_wallet_settings_blockchain_name = "ID блокчейна";

phrase lng_wallet_warning_blockchain_name = "Вы точно хотите изменить ID блокчейна? Вам не следует этого делать, если вы не тестируюете свою сеть TON.\n\nЕсли вы продолжите, вам нужно будет переподключить кошелёк используя 24 секретных слова.";
phrase lng_wallet_bad_config = "К сожалению, этот конфиг неверен.";
phrase lng_wallet_bad_config_url = "Приложение не смогло загрузить конфиг блокчейна. Убедитесь, что у вас работает интернет-соединение.";
phrase lng_wallet_wait_pending = "Подождите, пока не завершится текущая транзакция.";
phrase lng_wallet_wait_syncing = "Пожалуйста, подождите, пока кошелёк завершит синхронизацию с блокчейном TON.";

phrase lng_wallet_downloaded = "{ready} / {total} {mb}";

const auto walletCountValidate = check_phrase_count(Wallet::kPhrasesCount);

Fn<phrase(int)> lng_wallet_refreshed_minutes_ago = [](int minutes) {
	return "обновлено " + QString::number(minutes)
		+ ChoosePlural(
			minutes,
			" минуту назад",
			" минуты назад",
			" минут назад",
			" минуты назад");
};

Fn<phrase(QDate)> lng_wallet_short_date = [](QDate date) {
	const auto month = date.month();
	const auto result = [&]() -> QString {
		switch (month) {
		case 1: return "января";
		case 2: return "февраля";
		case 3: return "марта";
		case 4: return "апреля";
		case 5: return "мая";
		case 6: return "июня";
		case 7: return "июля";
		case 8: return "августа";
		case 9: return "сентября";
		case 10: return "октября";
		case 11: return "ноября";
		case 12: return "декабря";
		}
		return QString();
	}();
	if (result.isEmpty()) {
		return result;
	}
	const auto small =  QString::number(date.day()) + ' ' + result;
	const auto year = date.year();
	const auto current = QDate::currentDate();
	const auto currentYear = current.year();
	const auto currentMonth = current.month();
	if (year == currentYear) {
		return small;
	}
	const auto yearIsMuchGreater = [](int year, int otherYear) {
		return (year > otherYear + 1);
	};
	const auto monthIsMuchGreater = [](
			int year,
			int month,
			int otherYear,
			int otherMonth) {
		return (year == otherYear + 1) && (month + 12 > otherMonth + 3);
	};
	if (false
		|| yearIsMuchGreater(year, currentYear)
		|| yearIsMuchGreater(currentYear, year)
		|| monthIsMuchGreater(year, month, currentYear, currentMonth)
		|| monthIsMuchGreater(currentYear, currentMonth, year, month)) {
		return small + " " + QString::number(year);
	}
	return small;
};

Fn<phrase(QTime)> lng_wallet_short_time = [](QTime time) {
	return time.toString(Qt::SystemLocaleShortDate);
};

Fn<phrase(QString)> lng_wallet_grams_count = [](QString text) {
	return text + " Gram";
};

Fn<phrase(QString)> lng_wallet_grams_count_sent = [](QString text) {
	return text + " Gram успешно отправлено.";
};

} // namespace ph

namespace Wallet {

void SetPhrases(
		ph::details::phrase_value_array<kPhrasesCount> data,
		Fn<rpl::producer<QString>(int)> wallet_refreshed_minutes_ago,
		Fn<rpl::producer<QString>(QDate)> wallet_short_date,
		Fn<rpl::producer<QString>(QTime)> wallet_short_time,
		Fn<rpl::producer<QString>(QString)> wallet_grams_count) {
	ph::details::set_values(std::move(data));
	ph::lng_wallet_refreshed_minutes_ago = [=](int minutes) {
		return ph::phrase{ wallet_refreshed_minutes_ago(minutes) };
	};
	ph::lng_wallet_short_date = [=](QDate date) {
		return ph::phrase{ wallet_short_date(date) };
	};
	ph::lng_wallet_short_time = [=](QTime date) {
		return ph::phrase{ wallet_short_time(date) };
	};
	ph::lng_wallet_grams_count = [=](QString text) {
		return ph::phrase{ wallet_grams_count(text) };
	};
}

} // namespace Wallet
