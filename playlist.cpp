#include "playlist.hpp"
#include "ui_playlist.h"

Playlist::Playlist(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Playlist)
{
	ui->setupUi(this);

	m_container = new QWidget();
	ui->scrollArea->setWidget( m_container );

	m_layout = new QVBoxLayout( m_container );

	read_config_file_and_set_playlist();
	refresh_songs_list();
	config_watcher_to_songs_dir();
}

Playlist::~Playlist()
{
	delete ui;
}

void Playlist::read_config_file_and_set_playlist()
{
	auto result = Config_file_handler::get_values( {"songs_dir"} );
	if (!result.has_value())
	{
		QString error_1 = "ERROR IN GETTING VALUES FROM CONFIG_FILE!";
		QString error_2 = "PLEASE, TRY AGAIN OR DELETE CONFIG_FILE";

		log.create_log( {error_1, error_2}, this );
		exit( EXIT_FAILURE );
	}

	m_songs_dir_path = result.value().at(0);
	m_song_handler.set_playlist( m_songs_dir_path );
}

void Playlist::config_watcher_to_songs_dir()
{
	m_watcher.addPath( m_songs_dir_path );
	connect( &m_watcher, &QFileSystemWatcher::directoryChanged, this, &Playlist::songs_dir_watcher_event );
}

void Playlist::songs_dir_watcher_event()
{
	refresh_songs_list();
	m_song_handler.reset_playlist();
}

void Playlist::refresh_songs_list()
{
	QDir songs_dir( m_songs_dir_path );
	if (!songs_dir.exists())
	{
		QString error_1 = "SONGS FOLDER DONT EXISTS!";
		QString error_2 = "TRY TO REOPEN MUSHCFLOW! IF NOT WORK, DELETE CONFIG_FILE!";

		log.create_log( {error_1, error_2}, this );

		return;
	}

	// Delete all childrens in m_container (who have a QVBoxLayout)
	qDeleteAll( m_container->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly) );
	QFileInfoList files_info = songs_dir.entryInfoList();
	int total_files = 0;

	for ( auto &file : std::as_const(files_info) )
	{
		if (file.isDir())
		{
			continue;
		}

		if (!file.fileName().contains( ".mp3" ))
		{
			continue;
		}

		QPushButton *button = new QPushButton( file.fileName().remove(".mp3") );

		button->setFixedSize( 460, 60 );
		button->setFont( QFont( "Arial" ));
		button->setStyleSheet( "font: bold; text-decoration: underline;" );

		if (m_active_thumbnail)
		{
			set_pix_map( *button, file.filePath().remove(".mp3") );
		}

		connect(button, &QPushButton::clicked, this, [ this, file ]{ play_song( file ); } );

		m_layout->addWidget( button );

		total_files++;
		ui->songs_count_label->setText( QString::number( total_files ) );
	}
}

void Playlist::set_pix_map( QPushButton &button, const QString &path )
{
	if (!QFile::exists( path + ".jpg" ))
	{
		qDebug() << "IMAGE OF " + button.text() << " DONT EXISTS!";
		return;
	}

	QPixmap pixmap( path + ".jpg" );
    pixmap = pixmap.scaled( button.size(), Qt::KeepAspectRatio );

	QPalette palette;
	palette.setBrush( button.backgroundRole(), QBrush( pixmap ) );

	button.setFlat( true );
	button.setAutoFillBackground( true );
	button.setPalette( palette );
}

void Playlist::play_song( const QFileInfo &song_info )
{
	ui->song_label->setText( song_info.fileName().remove(".mp3") );
	ui->tabWidget->setCurrentIndex( 0 );

	m_song_handler.play_song( song_info );
}

void Playlist::on_play_pause_button_clicked()
{
	m_song_handler.pause_unpause_song();
}

void Playlist::on_thumbnail_check_stateChanged(int state)
{
	m_active_thumbnail = state;
	refresh_songs_list();
}
