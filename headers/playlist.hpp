#pragma once

#include <QDialog>

#include <QAbstractButton>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QFile>

#include <optional>

#include "config_file_handler.hpp"
#include "song_folder_manager.hpp"
#include "song_handler.hpp"
#include "logs.hpp"

namespace Ui {
class Playlist;
}

class Playlist : public QDialog
{
		Q_OBJECT

	public:
		explicit Playlist(QWidget *parent = nullptr);
		~Playlist();

	private slots:
		void on_play_pause_button_clicked();

		void on_thumbnail_check_stateChanged(int state);

		void on_volume_slider_valueChanged(int value);

		void on_previous_button_clicked();

		void on_next_button_clicked();

		void on_speed_combo_currentTextChanged(const QString &speed);

		void on_random_track_check_toggled(bool checked);

		void on_loop_check_toggled(bool checked);

		void on_position_slider_sliderMoved(int position);

	private:
		Ui::Playlist *ui;

		void make_class_configs();
		void set_thumbnail_show_state();
		void config_song_handler();

		QWidget *m_container;
		QVBoxLayout *m_layout;

		QString m_songs_dir_path;
		bool m_show_thumbnail;
};

