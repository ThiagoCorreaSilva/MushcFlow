#include "../headers/config_file_handler.hpp"

Config_file_handler::Config_file_handler()
{
	m_config_file.setFileName( m_config_file_name );
}

void Config_file_handler::write_values( const QMap< QString, QString > &values_to_write )
{
	if (!m_config_file.open( QFile::WriteOnly ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		m_config_file.remove();

		return;
	}

	QJsonDocument document;
	QJsonObject object;

	for ( const auto [key, value] : values_to_write.asKeyValueRange() )
	{
		object[key] = value;
	}

	document.setObject( object );

	m_config_file.write( document.toJson() );
	m_config_file.close();
}

void Config_file_handler::update_value( const QString &name, const QString &value )
{
	if (!m_config_file.open( QFile::ReadWrite ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		m_config_file.remove();

		return;
	}

	QByteArray buffer = m_config_file.readAll();
	QJsonParseError error;

	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );

	if (error.error != QJsonParseError::NoError)
	{
		QString error_1 = "ERROR IN READING JSON DATA FROM CONFIG FILE!";
		QString error_2 = error.errorString();

		log.create_log( {error_1, error_2} );

		return;
	}

	QJsonObject object = document.object();
	QJsonValueRef value_ref = object.find( name ).value();

	value_ref = value;
	document.setObject( object );

	m_config_file.seek( 0 );
	m_config_file.write( document.toJson() );
	m_config_file.close();
}

std::optional<QMap< QString, QString >> Config_file_handler::get_values( const QStringList &values_to_read )
{
	if (!m_config_file.open( QFile::ReadOnly ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		m_config_file.remove();

		return {};
	}

	QByteArray buffer = m_config_file.readAll();
	m_config_file.close();

	QJsonParseError error;

	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );
	if (error.error != QJsonParseError::NoError)
	{
		QString error_1 = "ERROR IN READING JSON DATA! (CONFIG_FILE)";
		QString error_2 = "PLEASE OPEN THE PROGRAM AGAIN OR DELETE CONFIG_FILE!";

		log.create_log( {error_1, error_2} );
		return {};
	}

	QJsonObject object = document.object();
	QMap< QString, QString> values;

	for ( const QString &value : values_to_read )
	{
		if (object.value(value).isUndefined())
		{
			qDebug() << "INVALID VALUE PASSED!";
			return {};
		}

		values.insert( value, object.value(value).toString() );
	}

	return values;
}

QString Config_file_handler::get_value( const QString &value_to_read )
{
	if (!m_config_file.open( QFile::ReadOnly ))
	{
		log.create_log( {"ERROR IN READING CONFIG FILE!"} );

		return {};
	}

	QByteArray buffer = m_config_file.readAll();
	m_config_file.close();

	QJsonParseError error;
	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );

	if (error.error != QJsonParseError::NoError)
	{
		QString error_1 = "ERROR IN READING JSON DATA FROM CONFIG FILE!";
		QString error_2 = error.errorString();

		log.create_log( {error_1, error_2} );

		return {};
	}

	QJsonObject object = document.object();
	return object.value( value_to_read ).toString();
}
