#include "../headers/downloader.hpp"
#include "uis/ui_downloader.h"

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
	save_thumbnail_format();

	delete ui;
}

void Downloader::save_thumbnail_format()
{
	QString previous_value = Config_file_handler::get_Instance().get_value( VALUE::THUMBNAIL_FORMAT );
	QString new_value = ui->thumbnail_format->currentText();

	if (previous_value == new_value)
	{
		qDebug() << "SAME VALUE!";
		return;
	}

	Config_file_handler::get_Instance().update_value( VALUE::THUMBNAIL_FORMAT, new_value );
}

void Downloader::read_config_file()
{
	ui->thumbnail_format->setCurrentText( Config_file_handler::get_Instance().get_value( VALUE::THUMBNAIL_FORMAT ) );
	m_songs_dir_path = Config_file_handler::get_Instance().get_value( VALUE::SONGS_DIR );
}

void Downloader::on_download_button_clicked()
{
	if (ui->url_input->text().isEmpty())
	{
		QMessageBox::warning( this, tr( "Empty URL!" ), tr( "You need to put a valid URL!" ) );
		return;
	}

	start_download();
}

void Downloader::start_download()
{
	QString system_type = QSysInfo::productType();
	QString yt_dlp_binary = (system_type == "windows") ? "yt-dlp.exe" : "yt-dlp";
	QString yt_dlp_flags = yt_dlp_binary + " --ies all,-generic ";
	QString yt_dlp_thumbnail_format = ui->thumbnail_format->currentText();
	QString yt_dlp_download_thumb = "--write-thumbnail --convert-thumbnails " + yt_dlp_thumbnail_format + " --ppa \"ThumbnailsConvertor:-q:v 1\" ";
	QString yt_dlp_song_format = "-x --audio-format mp3 -o ";
	QString yt_dlp_path = "\"" + m_songs_dir_path + "/%(title)s.%(ext)s\" ";
	QString yt_dlp_url = ui->url_input->text();
	QString ffmpeg_location =  (system_type == "windows") ? " --ffmpeg-location \"ffmpeg/\"" : "";
	QString command = yt_dlp_flags + yt_dlp_download_thumb + yt_dlp_song_format + yt_dlp_path + yt_dlp_url + ffmpeg_location;

	ui->url_input->setReadOnly( true );
	ui->url_input->clear();
	ui->url_input->setFocus();

	debug( system_type, yt_dlp_thumbnail_format, yt_dlp_url );

	QProcess process;
	process.startCommand( command );
	process.waitForFinished();

	if (process.exitCode() != 0)
	{
		ui->status_label->setText( "ERROR!" );
		ui->url_input->setReadOnly( false );

		return;
	}

	Song_folder_manager::get_Instance().refresh_list();

	ui->status_label->setText( "DOWNLOAD COMPLETED SUCCESSFULLY!" );
	ui->url_input->setReadOnly( false );
}

void Downloader::debug( const QString &system, const QString &format, const QString &url )
{
	qDebug();
	qDebug() << "DOWNLOAD STARTED!";
	qDebug() << "SYSTEM_TYPE_______: " << system;
	qDebug() << "THUMBNAIL_FORMAT__: " << format;
	qDebug() << "SONG_PATH_________: " << m_songs_dir_path;
	qDebug() << "SONG_URL__________: " << url;
	qDebug();
}
