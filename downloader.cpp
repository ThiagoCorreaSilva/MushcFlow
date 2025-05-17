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
	auto result = Config_file_handler::get_values( {"songs_dir"} );
	if (!result.has_value())
	{
		QString error_1 = "ERROR IN GETTING VALUES FROM CONFIG_FILE!";
		QString error_2 = "PLEASE, TRY AGAIN!";

		log.create_log( {error_1, error_2}, this );
	}

	m_songs_dir_path = result.value().value("songs_dir");
}

void Downloader::on_download_button_clicked()
{
	if (ui->url_input->text().isEmpty())
	{
		QMessageBox::warning( this, tr("Empty URL!"), tr("You need to put a valid URL!"));
		return;
	}

	QtConcurrent::run( QThreadPool::globalInstance(), [this]{ start_download() ;} );
}

void Downloader::start_download()
{
	static bool on_download = false;
	if (on_download)
	{
		return;
	}

	on_download = true;
	ui->status_label->setText( tr("Working... WAIT!") );

	qDebug() << "started";
	QString system_type = QSysInfo::productType();
	QString yt_dlp_binary = (system_type == "windows") ? "yt-dlp.exe" : "yt-dlp";
	QString yt_dlp_flags = yt_dlp_binary + " --ies all,-generic ";
	QString yt_dlp_download_thumb = "--write-thumbnail --convert-thumbnails jpg --ppa \"ThumbnailsConvertor:-q:v 1\" ";
	QString yt_dlp_song_format = "-x --audio-format mp3 -o ";
	QString yt_dlp_path = "\"" + m_songs_dir_path + "/%(title)s.%(ext)s\" ";
	QString yt_dlp_url = ui->url_input->text();
	QString ffmpeg_location =  (system_type == "windows") ? " --ffmpeg-location \"ffmpeg/\"" : "";
	QString command = yt_dlp_flags + yt_dlp_download_thumb + yt_dlp_song_format + yt_dlp_path + yt_dlp_url + ffmpeg_location;

	ui->url_input->setReadOnly( true );
	ui->url_input->clear();
	ui->url_input->setFocus();

	QProcess process;
	process.startCommand( command );
	process.waitForFinished();

	if (process.exitCode() != 0)
	{
		on_download = false;

		ui->status_label->setText( "ERROR!" );
		ui->url_input->setReadOnly( false );

		return;
	}

    ui->status_label->setText( "DOWNLOAD COMPLETED SUCCESSFULLY!" );
	ui->url_input->setReadOnly( false );

	on_download = false;
}
