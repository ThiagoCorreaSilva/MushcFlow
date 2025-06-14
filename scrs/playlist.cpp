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
	Song_folder_manager::get_Instance().set_container( m_container );
	Song_folder_manager::get_Instance().set_label( ui->songs_count_label );

	make_class_configs();
}

Playlist::~Playlist()
{
	delete ui;
}

void Playlist::make_class_configs()
{
	m_songs_dir_path = Config_file_handler::get_Instance().get_value( VALUE::SONGS_DIR );
	ui->volume_label->setText( "Volume: " + QString::number( 50 ) );

	Song_folder_manager::get_Instance().refresh_list();

	m_show_thumbnail = Config_file_handler::get_Instance().get_value( VALUE::USE_THUMBNAIL ).toInt();
	ui->thumbnail_check->setChecked( m_show_thumbnail );

	config_song_handler();
}

void Playlist::config_song_handler()
{
	Song_handler::get_Instance().set_ui_elements( *ui->song_label, *ui->position_slider, *ui->duration_label, *ui->position_label, *ui->tabWidget );
	Song_handler::get_Instance().set_song_speed( ui->speed_combo->currentText() );
	Song_handler::get_Instance().change_replay( ui->loop_check->isChecked() );
	Song_handler::get_Instance().change_random_track_state( ui->random_track_check->isChecked() );
}

void Playlist::on_play_pause_button_clicked()
{
	Song_handler::get_Instance().pause_unpause_song();
}

void Playlist::on_thumbnail_check_stateChanged(int state)
{
	m_show_thumbnail = ( state == 0) ? false : true;

	Config_file_handler::get_Instance().update_value( VALUE::USE_THUMBNAIL, QString::number( m_show_thumbnail ) );
	Song_folder_manager::get_Instance().change_thumbnails_state();
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
