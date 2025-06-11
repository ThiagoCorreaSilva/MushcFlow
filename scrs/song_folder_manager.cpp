#include "../headers/song_folder_manager.hpp"

Song_folder_manager::Song_folder_manager()
{
	m_song_dir_path = Config_file_handler::get_Instance().get_value( "songs_dir" );
}

void Song_folder_manager::set_layout( QVBoxLayout *layout )
{
	m_layout = layout;
}

void Song_folder_manager::set_container( QWidget *container )
{
	m_container = container;
}

void Song_folder_manager::set_label( QLabel *label )
{
	m_song_count_label = label;
}

QVector< QPushButton* > &Song_folder_manager::refresh_list()
{
	m_songs_buttons.clear();
	if (m_layout == nullptr)
	{
		qDebug() << "NULL LAYOUT!";
		return m_songs_buttons;
	}

	if (m_container == nullptr)
	{
		qDebug() << "NULL CONTAINER";
		return m_songs_buttons;
	}

	QDir song_dir( m_song_dir_path );
	if ( !song_dir.exists() )
	{
		return m_songs_buttons;
	}

	QFileInfoList files = song_dir.entryInfoList( {"*.mp3"} );

	Song_handler::get_Instance().reset_playlist();
	Song_handler::get_Instance().set_playlist( files );

	// Delete all childrens in m_container (who have a QVBoxLayout)
	qDeleteAll( m_container->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly) );

	move_thumbnails();

	for ( auto &file : std::as_const( files ) )
	{
		create_button( file.fileName().remove( ".mp3" ) );
	}

	if (m_song_count_label != nullptr)
	{
		m_song_count_label->setText( QString::number( m_songs_buttons.size() ) );
	}

	return m_songs_buttons;
}

void Song_folder_manager::move_thumbnails()
{
	QString thumbnails_path = Config_file_handler::get_Instance().get_value( "thumbnails_dir" );

	if (thumbnails_path.isEmpty())
	{
		qDebug() << "EMPTY THUMBNAIL PATH!";
		return;
	}

	QDir thumbnails_dir( thumbnails_path );
	QFileInfoList thumbs = thumbnails_dir.entryInfoList( {"*.png", "*.jpg"});

	for ( auto &thumb : thumbs )
	{
		QString previous_name = thumb.fileName();
		QFile::rename( thumb.absoluteFilePath(), thumbnails_path + '/' + previous_name );
	}
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
	QString song_name = name;
	QMessageBox question_box;

	question_box.setIcon( QMessageBox::Question );
	question_box.setWindowTitle( QTranslator::tr( "Question!" ) );
	question_box.setText( QTranslator::tr("What you want to make with: ") + '\n' + song_name );

	question_box.addButton( QTranslator::tr( "Play" ), QMessageBox::AcceptRole );
	question_box.addButton( QTranslator::tr( "Nothing" ), QMessageBox::RejectRole );
	question_box.addButton( QTranslator::tr( "Delete" ), QMessageBox::DestructiveRole );

	int result = question_box.exec();

	static const int ACCEPT_ROLE = 2;
	static const int DELETE_ROLE = 4;

	QFileInfo file( m_song_dir_path + '/' + name + ".mp3"  );

	switch ( result )
	{
		case ACCEPT_ROLE:
			Song_handler::get_Instance().play_song( file );
			break;

		case DELETE_ROLE:
			delete_song( file );
			break;
	}
}

void Song_folder_manager::delete_song( const QFileInfo &file )
{
	Song_handler::get_Instance().stop_song();

	QString song_thumbnail_jpg = file.absoluteFilePath().remove( ".mp3" ) + ".jpg";
	QString song_thumbnail_png = file.absoluteFilePath().remove( ".mp3" ) + ".png";

	QFile::remove( song_thumbnail_jpg );
	QFile::remove( song_thumbnail_png );
	QFile::remove( file.absoluteFilePath() );

	refresh_list();
}
