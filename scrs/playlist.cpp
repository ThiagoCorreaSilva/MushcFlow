#include "../headers/playlist.hpp"
#include "uis/ui_playlist.h"

Playlist::Playlist(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Playlist)
{
	ui->setupUi(this);

	m_container = new QWidget();
	ui->scrollArea->setWidget( m_container );

	m_layout = new QVBoxLayout( m_container );
	Song_folder_manager::get_Instance().set_layout( m_layout );

	Song_handler::get_Instance().set_ui_elements( *ui->song_label, *ui->position_slider, *ui->duration_label, *ui->position_label );
	Song_handler::get_Instance().set_song_speed( ui->speed_combo->currentText() );
	Song_handler::get_Instance().change_replay( ui->loop_check->isChecked() );
	Song_handler::get_Instance().change_random_track_state( ui->random_track_check->isChecked() );

	make_class_configs();
	refresh_songs_list();
	//config_watcher_to_songs_dir();
}

Playlist::~Playlist()
{
	delete ui;
}

void Playlist::make_class_configs()
{
	auto result = Config_file_handler::get_Instance().get_values( {"songs_dir", "use_thumbnail"} );
	if (!result.has_value())
	{
		QString error_1 = "ERROR IN GETTING VALUES FROM CONFIG_FILE!";
		QString error_2 = "PLEASE, TRY AGAIN OR DELETE CONFIG_FILE";

		Logs::get_Instance().create_log( {error_1, error_2}, this );
		exit( EXIT_FAILURE );
	}

	m_songs_dir_path = result.value().value("songs_dir");
	ui->volume_label->setText( "Volume: " + QString::number( 50 ) );

	m_show_thumbnail = Config_file_handler::get_Instance().get_value( "use_thumbnail" ).toInt();
	ui->thumbnail_check->setChecked( m_show_thumbnail );
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

		Logs::get_Instance().create_log( {error_1, error_2}, this );

		return;
	}

	// Delete all childrens in m_container (who have a QVBoxLayout)
	qDeleteAll( m_container->findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly) );

	/*
	QFileInfoList files_info = songs_dir.entryInfoList();

	m_songs_button.clear();
	m_song_handler.reset_playlist();
	m_song_handler.set_playlist( files_info );

	for ( auto &file : std::as_const(files_info) )
	{
		if (!file.fileName().contains( ".mp3" ))
		{
			continue;
		}

		QPushButton *button = new QPushButton( file.fileName().remove(".mp3") );

		button->setFixedSize( 460, 60 );
		button->setFont( QFont( "Arial" ));
		button->setStyleSheet( "font: bold; text-decoration: underline;" );
		m_songs_button.push_back( button );

		if (m_show_thumbnail)
		{
			set_pix_map( *button, file.filePath().remove(".mp3") );
		}

		connect(button, &QPushButton::clicked, this, [ this, file ]{ button_action( const_cast<QFileInfo&>(file)); } );

		m_layout->addWidget( button );
	}
*/
	int songs_button_quantity = Song_folder_manager::get_Instance().refresh_list().size();
	ui->songs_count_label->setText( QString::number( songs_button_quantity ) );
}

void Playlist::add_image_in_button()
{
	if (!m_show_thumbnail)
	{
		return;
	}

}
void Playlist::remove_image_from_button()
{

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

void Playlist::button_action( QFileInfo &song_info )
{
	QString song_name = song_info.fileName().remove(".mp3");
	QMessageBox question_box;

	question_box.setIcon( QMessageBox::Question );
	question_box.setWindowTitle( tr( "Question!" ) );
	question_box.setText( tr("What you want to make with: ") + '\n' + song_name );

	question_box.addButton( tr( "Play" ), QMessageBox::AcceptRole );
	question_box.addButton( tr( "Nothing" ), QMessageBox::RejectRole );
	question_box.addButton( tr( "Delete" ), QMessageBox::DestructiveRole );

	int result = question_box.exec();

	static const int ACCEPT_ROLE = 2;
	static const int DELETE_ROLE = 4;

	switch ( result )
	{
		case ACCEPT_ROLE:
			play_song( song_info );
			break;

		case DELETE_ROLE:
			delete_song( song_info );
			break;
	}
}

void Playlist::play_song( const QFileInfo &song_info )
{
	ui->tabWidget->setCurrentIndex( 0 );

	Song_handler::get_Instance().set_ui_elements( *ui->song_label, *ui->position_slider, *ui->duration_label, *ui->position_label );
	Song_handler::get_Instance().set_song_speed( ui->speed_combo->currentText() );
	Song_handler::get_Instance().change_replay( ui->loop_check->isChecked() );
	Song_handler::get_Instance().change_random_track_state( ui->random_track_check->isChecked() );
	Song_handler::get_Instance().play_song( song_info );
}

void Playlist::delete_song( QFileInfo &song_info )
{
	qDebug() << "DELETED";
	ui->tabWidget->setCurrentIndex( 1 );
	Song_handler::get_Instance().stop_song();

	QString song_thumbnail = song_info.absoluteFilePath().remove(".mp3") + ".jpg";

	QFile::remove( song_thumbnail );
	QFile::remove( song_info.absoluteFilePath() );

	Song_folder_manager::get_Instance().refresh_list();
	refresh_songs_list();
}

void Playlist::on_play_pause_button_clicked()
{
	Song_handler::get_Instance().pause_unpause_song();
}

void Playlist::on_thumbnail_check_stateChanged(int state)
{
	m_show_thumbnail = ( state == 0) ? 0 : 1;

	Config_file_handler::get_Instance().update_value( "use_thumbnail", QString::number( m_show_thumbnail ) );

	Song_folder_manager::get_Instance().refresh_list();
	refresh_songs_list();
}

void Playlist::on_volume_slider_valueChanged(int value)
{
	ui->volume_label->setText( "Volume: " + QString::number( value ) );
	Song_handler::get_Instance().change_volume( value );
}

void Playlist::on_previous_button_clicked()
{
	Song_handler::get_Instance().previous_song();
}

void Playlist::on_next_button_clicked()
{
	Song_handler::get_Instance().next_song();
}

void Playlist::on_speed_combo_currentTextChanged(const QString &speed)
{
	Song_handler::get_Instance().set_song_speed( speed );
}

void Playlist::on_random_track_check_toggled(bool checked)
{
	Song_handler::get_Instance().change_random_track_state( checked );
}

void Playlist::on_loop_check_toggled(bool checked)
{
	Song_handler::get_Instance().change_replay( checked );
}

void Playlist::on_position_slider_sliderMoved(int position)
{
	Song_handler::get_Instance().change_song_position( position );
}
