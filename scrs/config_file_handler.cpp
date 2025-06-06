#include "../headers/config_file_handler.hpp"

void Config_file_handler::write_values( const QMap< QString, QString > &values_to_write )
{
	Logs log;
	QFile config_file( "configs.json" );

	if (!config_file.open( QFile::WriteOnly ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		config_file.remove();

		return;
	}

	QJsonDocument document;
	QJsonObject object;

	for ( const auto [key, value] : values_to_write.asKeyValueRange() )
	{
		object[key] = value;
	}

	document.setObject( object );

	config_file.write( document.toJson() );
	config_file.close();

	qDebug() << "writed";
}

std::optional<QMap< QString, QString >> Config_file_handler::get_values( const QStringList &values_to_read )
{
	QMap< QString, QString> values;
	Logs log;

	QFile config_file( "configs.json" );
	if (!config_file.open( QFile::ReadOnly ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		config_file.remove();

		return {};
	}

	QByteArray buffer = config_file.readAll();
	config_file.close();

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
