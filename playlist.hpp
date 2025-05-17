#pragma once

#include <QDialog>

#include <QFileSystemWatcher>
#include <QAbstractButton>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFileInfo>
#include <QPalette>
#include <QPixmap>
#include <QWidget>
#include <QFile>

#include <optional>

#include "config_file_handler.hpp"
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

		void on_random_track_check_clicked(bool checked);

		void on_loop_check_clicked(bool checked);

	private:
		Ui::Playlist *ui;

		void read_config_file_and_make_class_configs();
		void set_thumbnail_show_state();
		void config_watcher_to_songs_dir();
		void songs_dir_watcher_event();
		void refresh_songs_list();
		void set_pix_map( QPushButton &button, const QString &path );
		void button_action( QFileInfo &song_info );
		void play_song( const QFileInfo &song_info );
		void delete_song( QFileInfo &song_info );

		QWidget *m_container;
		QVBoxLayout *m_layout;
		QFileSystemWatcher m_watcher;
		Song_handler m_song_handler;

		QString m_songs_dir_path;
		bool m_show_thumbnail = false;

		Logs log;
};

