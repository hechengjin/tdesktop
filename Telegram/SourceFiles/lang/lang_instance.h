/*
This file is part of Telegram Desktop,
the official desktop version of Telegram messaging app, see https://telegram.org

Telegram Desktop is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

It is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

In addition, as a special exception, the copyright holders give permission
to link the code of portions of this program with the OpenSSL library.

Full license: https://github.com/telegramdesktop/tdesktop/blob/master/LICENSE
Copyright (c) 2014-2017 John Preston, https://desktop.telegram.org
*/
#pragma once

#include "lang_auto.h"
#include "base/weak_unique_ptr.h"

namespace Lang {

constexpr auto kLegacyLanguageNone = -2;
constexpr auto kLegacyCustomLanguage = -1;
constexpr auto kLegacyDefaultLanguage = 0;

QString DefaultLanguageId();

class Instance;
Instance &Current();

class Instance : public base::enable_weak_from_this {
public:
	Instance() {
		fillDefaults();
	}
	void switchToId(const QString &id);
	void chooseCustomFile();
	void switchToCustomFile(const QString &filePath);

	Instance(const Instance &other) = delete;
	Instance &operator=(const Instance &other) = delete;
	Instance(Instance &&other) = default;
	Instance &operator=(Instance &&other) = default;

	QString cloudLangCode() const;

	QString id() const {
		return _id;
	}
	bool isCustom() const {
		return id() == qstr("custom");
	}
	int version() const {
		return _version;
	}

	QByteArray serialize() const;
	void fillFromSerialized(const QByteArray &data);
	void fillFromLegacy(int legacyId, const QString &legacyPath);

	void applyDifference(const MTPDlangPackDifference &difference);
	base::Observable<void> &updated() {
		return _updated;
	}

	QString getValue(LangKey key) {
		Expects(key >= 0 && key < kLangKeysCount);
		Expects(_values.size() == kLangKeysCount);
		return _values[key];
	}

private:
	void applyValue(const QByteArray &key, const QByteArray &value);
	void resetValue(const QByteArray &key);
	void reset();
	void fillDefaults();
	void fillFromCustomFile(const QString &filePath);
	void loadFromContent(const QByteArray &content);
	void loadFromCustomContent(const QString &absolutePath, const QString &relativePath, const QByteArray &content);

	QString _id;
	int _legacyId = kLegacyLanguageNone;
	QString _customFilePathAbsolute;
	QString _customFilePathRelative;
	QByteArray _customFileContent;
	int _version = 0;
	base::Observable<void> _updated;

	mutable QString _systemLanguage;

	std::vector<QString> _values;
	std::map<QByteArray, QByteArray> _nonDefaultValues;

};

} // namespace Lang
