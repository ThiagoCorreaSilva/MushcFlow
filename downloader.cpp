#include "downloader.hpp"
#include "ui_downloader.h"

Downloader::Downloader(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Downloader)
{
	ui->setupUi(this);

	read_config_file();
	ui->downloader_folder_label->setText( m_songs_dir_path );
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
		QString error = "ERROR IN OPENING CONFIG FILE!";

		log.create_log( {error}, ERROR_TYPE::NON_FATAL );

		return;
	}

	QByteArray buffer = config_file.readAll();
	config_file.close();

	QJsonParseError error;
	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );

	if (error.error != QJsonParseError::NoError)
	{
		QString error_1 = "ERROR IN READING JSON DOCUMENT!";
		QString error_2 = "IF YOU CHANGE THE CONFIG_FILE, PLEASE DELETE IT AND OPEN AGAIN!";

		log.create_log( {error_1, error_2} );

		exit( EXIT_FAILURE );
	}

	QJsonObject object = document.object();

	m_app_dir_path = object.value("app_dir").toString();
	m_songs_dir_path = object.value( "songs_dir" ).toString();
}

void Downloader::on_download_button_clicked()
{
	if (ui->url_input->text().isEmpty())
	{
		QMessageBox::warning( this, tr("Empty URL!"), tr("You need to put a valid URL!"));
		return;
	}

	start_download();
}

void Downloader::start_download()
{
	QString system_type = QSysInfo::productType();
	QString yt_dlp_path = (system_type == "windows") ? "yt-dlp.exe" : "yt-dlp";
	QString yt_dlp_flags = yt_dlp_path + " --ies all,-generic -x --audio-format mp3 -o ";
	QString path = "\"" + m_songs_dir_path + "/%(title)s.%(ext)s\" ";
	QString url = ui->url_input->text();
	QString ffmpeg_location =  (system_type == "windows") ? " --ffmpeg-location \"ffmpeg/\"" : "";
	QString command = yt_dlp_flags + path + url + ffmpeg_location;

	QProcess process;
	process.startCommand( command );
	process.waitForFinished();

	ui->url_input->clear();
	ui->url_input->setFocus();

	if (process.exitCode() != 0)
	{
		QMessageBox::warning( this, "Error", tr("Download Failure! Read Log to more informations") );

		ui->status_label->setText( "ERROR!" );

		QStringList error_list;
		QString error_1 = "ERROR IN DOWNLOAD SONG!";
		QString error_2 = "SEE IF YOU SONG URL IS VALID AND TRY AGAIN!";

		error_list.push_back( error_1 );
		error_list.push_back( error_2 );
		log.create_log( error_list, ERROR_TYPE::NON_FATAL );

		return;
	}

    ui->status_label->setText( "DOWNLOAD COMPLETED SUCCESSFULLY!" );
}
