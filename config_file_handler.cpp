#include "config_file_handler.hpp"

std::optional<QStringList> Config_file_handler::get_values( const QStringList &values_to_read )
{
	QStringList values;
	Logs log;

	QFile config_file( "configs.json" );
	if (!config_file.open( QFile::ReadOnly ))
	{
		QString error_1 = "ERROR IN OPENING CONFIG_FILE";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2} );
		return {};
	}

	QByteArray buffer = config_file.readAll();
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
		if (object.value(value).isNull())
		{
			qDebug() << "INVALID VALUE PASSED!";
			return {};
		}

		values.push_back( object.value(value).toString() );
	}

	return values;
}
