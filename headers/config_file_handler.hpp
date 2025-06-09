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
	private:

		Config_file_handler();
		~Config_file_handler() = default;

		Logs log;

		const QString m_config_file_name = "configs.json";
		QFile m_config_file;

	public:

		static Config_file_handler &get_Instance()
		{
			static Config_file_handler instance;
			return instance;
		}

		void write_values( const QMap< QString, QString> &values_to_write );
		std::optional<QMap< QString, QString >> get_values( const QStringList &values_to_read );
		QString get_value( const QString &value_to_read );
		void update_value( const QString &name, const QString &value );
};
