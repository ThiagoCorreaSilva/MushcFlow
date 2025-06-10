#include "../headers/song_folder_manager.hpp"

Song_folder_manager::Song_folder_manager()
{
	m_song_dir_path = Config_file_handler::get_Instance().get_value( "songs_dir" );
	qDebug() << m_song_dir_path;
}

QVector< QPushButton* > &Song_folder_manager::refresh_list()
{
	m_songs_buttons.clear();

	QDir song_dir( m_song_dir_path );
	if ( !song_dir.exists() )
	{
		m_songs_buttons.clear();
		return m_songs_buttons;
	}

	QFileInfoList files = song_dir.entryInfoList( {"*.mp3"} );

	for ( auto &file : std::as_const( files ) )
	{
		create_button( file.fileName().remove( ".mp3" ) );
	}

	return m_songs_buttons;
}

void Song_folder_manager::create_button( const QString &name )
{
	QPushButton button;
	button.setText( name );
	button.setFixedSize( 460, 60 );
	button.setFont( QFont( "Arial" ));
	button.setStyleSheet( "font: bold; text-decoration: underline;" );

	add_pix_map( button );

	m_songs_buttons.push_back( &button );
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
		qDebug() << "JPG FOUNDED!";
		image_path += ".jpg";
	}
	else if (QFileInfo::exists( image_path + ".png" ))
	{
		qDebug() << "PNG FOUNDED!";
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
