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

enum class VALUE
{
	APP_DIR,
	SONGS_DIR,
	THUMBNAILS_DIR,
	USE_THUMBNAIL,
	THUMBNAIL_FORMAT,
	ALL
};

class Config_file_handler
{
	private:

		Config_file_handler();
		~Config_file_handler() = default;

		const QString m_config_file_name = "configs.json";
		QFile m_config_file;

		QString enum_to_string( const VALUE &value );

	public:

		static Config_file_handler &get_Instance()
		{
			static Config_file_handler instance;
			return instance;
		}

		void write_values( const QMap< QString, QString> &values_to_write );
		void update_value( const VALUE &name, const QString &value );
		bool check_config_file( const QStringList &keys_to_check );
		std::optional<QMap< QString, QString >> get_values( const QStringList &values_to_read );
		QString get_value( const VALUE &value_to_read );
};
