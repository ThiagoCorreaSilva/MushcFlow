#include "../headers/song_folder_manager.hpp"

Song_folder_manager::Song_folder_manager()
{
	m_song_dir_path = Config_file_handler::get_Instance().get_value( "songs_dir" );
}

void Song_folder_manager::set_layout( QVBoxLayout *layout )
{
	m_layout = layout;
}

QVector< QPushButton* > &Song_folder_manager::refresh_list()
{
	if (m_layout == nullptr)
	{
		qDebug() << "NULL LAYOUT!";

		m_songs_buttons.clear();
		return m_songs_buttons;
	}

	QDir song_dir( m_song_dir_path );
	if ( !song_dir.exists() )
	{
		m_songs_buttons.clear();
		return m_songs_buttons;
	}

	m_songs_buttons.clear();
	QFileInfoList files = song_dir.entryInfoList( {"*.mp3"} );

	Song_handler::get_Instance().reset_playlist();
	Song_handler::get_Instance().set_playlist( files );

	for ( auto &file : std::as_const( files ) )
	{
		create_button( file.fileName().remove( ".mp3" ) );
	}

	return m_songs_buttons;
}

void Song_folder_manager::create_button( const QString &name )
{
	QPushButton *button = new QPushButton( name );
	button->setFixedSize( 460, 60 );
	button->setFont( QFont( "Arial" ));
	button->setStyleSheet( "font: bold; text-decoration: underline;" );

	add_pix_map( *button );
	QPushButton::connect( button, &QPushButton::clicked, [ this, button ]{ button_pressed( button->text() ); } );

	m_songs_buttons.push_back( button );
	m_layout->addWidget( button );
}

void Song_folder_manager::add_pix_map( QPushButton &button )
{
	bool use_thumbnail = Config_file_handler::get_Instance().get_value( "use_thumbnail" ).toInt();
	if (!use_thumbnail)
	{
		return;
	}

	QString image_path = m_song_dir_path + '/' + button.text();

	if (QFileInfo::exists( image_path + ".jpg" ))
	{
		image_path += ".jpg";
	}
	else if (QFileInfo::exists( image_path + ".png" ))
	{
		image_path += ".png";
	}

	if (!image_path.contains( ".jpg" ) && !image_path.contains( ".png" ))
	{
		qDebug() << "IMAGE DONT EXISTS!: " + image_path;
		return;
	}

	QPixmap pixmap( image_path );
	pixmap = pixmap.scaled( button.size(), Qt::KeepAspectRatio );

	QPalette palette;
	palette.setBrush( button.backgroundRole(), QBrush( pixmap ) );

	button.setFlat( true );
	button.setAutoFillBackground( true );
	button.setPalette( palette );
}

void Song_folder_manager::button_pressed( const QString &name )
{
	qDebug() << "CLICKED: " << name;
}
