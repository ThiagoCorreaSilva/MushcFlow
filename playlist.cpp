#include "playlist.hpp"
#include "ui_playlist.h"

Playlist::Playlist(QWidget *parent, QTabWidget *tab) :
	QDialog(parent),
	m_tab( tab ),
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
		log.create_log( {error_1, error_2}, this );

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

		log.create_log( {error_1, error_2}, this );

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

		button->setFixedHeight( 50 );
		set_pix_map( *button, file.filePath().remove(".mp3") );
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
	QPalette palette;
	palette.setBrush( button.backgroundRole(), QBrush( pixmap ) );

	button.setFlat( true );
	button.setAutoFillBackground( true );
	button.setPalette( palette );
}

void Playlist::play_song( const QFileInfo &song_info )
{
	player.play_song( song_info );
	m_tab->setCurrentIndex( 0 );
}
