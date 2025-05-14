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

	read_config_file();
	refresh_songs_list();
	config_watcher_to_songs_dir();
}

Playlist::~Playlist()
{
	delete ui;
}

void Playlist::read_config_file()
{
	QFile config_file( "configs.json" );

	if (!config_file.open( QFile::ReadOnly ))
	{
		QString error_1 = "ERROR WHILE READING CONFIG_FILE!";
		QString error_2 = "IF YOU MAKE ANY CHANGES IN COFIG_FILE, DELETE IT AND TRY AGAIN!";
		log.create_log( {error_1, error_2} );

		exit( EXIT_FAILURE );
	}

	QByteArray buffer = config_file.readAll();
	config_file.close();

	QJsonParseError error;
	QJsonDocument document = QJsonDocument::fromJson( buffer, &error );

	if (error.error != QJsonParseError::NoError)
	{
		QString error_1 = "ERROR IN READING JSON DATA IN CONFIG_FILE!";
		QString error_2 = "PLEASE, DELETE CONFIG_FILE AND TRY AGAIN!";

		log.create_log( {error_1, error_2} );

		exit( EXIT_FAILURE );
	}

	QJsonObject object = document.object();

	m_songs_dir_path = object.value("songs_dir").toString();
}

void Playlist::config_watcher_to_songs_dir()
{
	m_watcher.addPath( m_songs_dir_path );
	connect( &m_watcher, &QFileSystemWatcher::directoryChanged, this, &Playlist::songs_dir_watcher_event );
}

void Playlist::songs_dir_watcher_event()
{
	refresh_songs_list();
	qDebug() << "WATCHER EVENT!";
}

void Playlist::refresh_songs_list()
{
	QDir songs_dir( m_songs_dir_path );
	if (!songs_dir.exists())
	{
		QString error_1 = "SONGS FOLDER DONT EXISTS!";
		QString error_2 = "TRY TO REOPEN MUSHCFLOW! IF NOT WORK, DELETE CONFIG_FILE!";

		log.create_log( {error_1, error_2} );

		return;
	}

	QFileInfoList files_info = songs_dir.entryInfoList();
	for ( const auto &file : std::as_const(files_info) )
	{
		if (file.isDir())
		{
			break;
		}

		QPushButton *button = new QPushButton( file.fileName() );
		connect(button, &QPushButton::clicked, this, [ this, button ]{ play_song( button->text() ); } );
	}
}

void Playlist::play_song( const QString &song_name )
{
	qDebug() << "song_name";
}
