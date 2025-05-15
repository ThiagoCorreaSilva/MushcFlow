#pragma once

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include <QFile>
#include <QDir>

#include <optional>

#include "logs.hpp"

class Config_file_handler
{
	public:
		static std::optional<QStringList> get_values( const QStringList &values_to_read );
};
