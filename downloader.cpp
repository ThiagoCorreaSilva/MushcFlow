#include "downloader.hpp"
#include "ui_downloader.h"

Downloader::Downloader(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Downloader)
{
	ui->setupUi(this);

	read_config_file();
}

Downloader::~Downloader()
{
	delete ui;
}

void Downloader::read_config_file()
{
	QFile config_file( "configs.json" );
	if (!config_file.open( QFile::ReadOnly ))
	{
		QStringList error_list;
		QString error_1 = "ERROR IN OPENING CONFIG FILE!";

		error_list.push_back( error_1 );
		log.create_log( error_list, ERROR_TYPE::NON_FATAL );

		return;
	}

	QByteArray buffer = config_file.readAll();
	config_file.close();

	QJsonParseError error;
	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );

	if (error.error == QJsonParseError::NoError)
	{
		QStringList error_list;
		QString error_1 = "ERROR IN READING JSON DOCUMENT!";
		QString error_2 = "IF YOU CHANGE THE CONFIG_FILE, PLEASE DELETE IT AND OPEN AGAIN!";

		error_list.push_front( error_1 );
		error_list.push_back( error_2 );
		log.create_log( error_list );

		exit( EXIT_FAILURE );
	}

	QJsonObject object = document.object();
	m_songs_dir_path = object.value( "songs_dir" ).toString();
}
