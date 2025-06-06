#pragma once

#include <QJsonParseError>
#include <QJsonValueRef>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QMap>

#include <optional>

#include "logs.hpp"

class Config_file_handler
{
	public:
		static std::optional<QMap< QString, QString >> get_values( const QStringList &values_to_read );
		static void write_values( const QMap< QString, QString> &values_to_write );
};
