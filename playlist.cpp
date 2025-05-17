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

	read_config_file_and_make_class_configs();
	refresh_songs_list();
	config_watcher_to_songs_dir();
}

Playlist::~Playlist()
{
	delete ui;
}

void Playlist::read_config_file_and_make_class_configs()
{
	auto result = Config_file_handler::get_values( {"songs_dir", "use_thumbnail"} );
	if (!result.has_value())
	{
		QString error_1 = "ERROR IN GETTING VALUES FROM CONFIG_FILE!";
		QString error_2 = "PLEASE, TRY AGAIN OR DELETE CONFIG_FILE";

		log.create_log( {error_1, error_2}, this );
		exit( EXIT_FAILURE );
	}

	m_songs_dir_path = result.value().value("songs_dir");
	ui->volume_label->setText( "Volume: " + QString::number( 50 ) );
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

	m_song_handler.reset_playlist();
	m_song_handler.set_playlist( files_info );

	int total_files = 0;

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

		if (m_show_thumbnail)
		{
			set_pix_map( *button, file.filePath().remove(".mp3") );
		}

		connect(button, &QPushButton::clicked, this, [ this, file ]{ button_action( const_cast<QFileInfo&>(file)); } );

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

void Playlist::button_action( QFileInfo &song_info )
{
	QString song_name = song_info.fileName().remove(".mp3");

	QMessageBox question_box;

	question_box.setIcon( QMessageBox::Question );
	question_box.setWindowTitle( tr("Question!") );
	question_box.setText( tr("What you want to make with: ") + '\n' + song_name );
	QAbstractButton *yes_button = question_box.addButton( tr("Play"), QMessageBox::AcceptRole );
	QAbstractButton *nothing_button = question_box.addButton( tr("Nothing"), QMessageBox::NoRole);
	QAbstractButton *delete_button = question_box.addButton( tr("Delete"), QMessageBox::DestructiveRole);

	question_box.exec();

	if (question_box.clickedButton() == yes_button)
	{
		play_song( song_info );
		return;
	}

	if (question_box.clickedButton() == delete_button)
	{
		delete_song( song_info );
		return;
	}

}

void Playlist::play_song( const QFileInfo &song_info )
{
	ui->tabWidget->setCurrentIndex( 0 );

	m_song_handler.set_song_label( *ui->song_label );
	m_song_handler.set_song_speed( ui->speed_combo->currentText() );
	m_song_handler.change_replay( ui->loop_check->isChecked() );
	m_song_handler.change_random_track_state( ui->random_track_check->isChecked() );
	m_song_handler.play_song( song_info );
}

void Playlist::delete_song( QFileInfo &song_info )
{
	ui->tabWidget->setCurrentIndex( 1 );
	m_song_handler.stop_song();

	QString song_thumbnail = song_info.absoluteFilePath().remove(".mp3") + ".jpg";
	qDebug() << song_info.absoluteFilePath();
	QFile::remove( song_thumbnail );
	QFile::remove( song_info.absoluteFilePath() );

	refresh_songs_list();
}

void Playlist::on_play_pause_button_clicked()
{
	m_song_handler.pause_unpause_song();
}

void Playlist::on_thumbnail_check_stateChanged(int state)
{
	m_show_thumbnail = state;
	refresh_songs_list();
}

void Playlist::on_volume_slider_valueChanged(int value)
{
	ui->volume_label->setText( "Volume: " + QString::number( value ) );
	m_song_handler.change_volume( value );
}

void Playlist::on_previous_button_clicked()
{
	m_song_handler.previous_song();
}

void Playlist::on_next_button_clicked()
{
	m_song_handler.next_song();
}

void Playlist::on_speed_combo_currentTextChanged(const QString &speed)
{
	m_song_handler.set_song_speed( speed );
}


void Playlist::on_random_track_check_clicked(bool checked)
{
	m_song_handler.change_random_track_state( checked );
}


void Playlist::on_loop_check_clicked(bool checked)
{
	m_song_handler.change_replay( checked );
}

