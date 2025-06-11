#pragma once

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QFileInfoList>
#include <QTabWidget>
#include <QFileInfo>
#include <QSlider>
#include <QDebug>
#include <QLabel>
#include <QDir>

#include <algorithm>
#include <random>

class Song_handler
{
	private:
		Song_handler();
		~Song_handler() = default;

		QMediaPlayer *m_player;
		QAudioOutput *m_output;
		QFileInfoList m_playlist_songs;

		int m_current_song_index;
		int m_max_song_index;
		float m_song_speed = 1;

		bool m_random_track;
		bool m_loop_track;

		QTabWidget *m_tab_widget;
		QLabel *m_song_label;
		QLabel *m_duration_label;
		QLabel *m_position_label;
		QSlider *m_position_slider;
		QList <int> m_random_index;

		void randomize_playlist();
		int get_random_index();

		void update_position_slider( const quint64 &position );
		void duration_changed( const quint64 &duration );

	public:

		static Song_handler &get_Instance()
		{
			static Song_handler instance;
			return instance;
		}

		void set_playlist( const QFileInfoList &playlist_path );
		void set_ui_elements( QLabel &song_label, QSlider &position_slider, QLabel &duration_label, QLabel &position_label, QTabWidget &tab_widget );
		void reset_playlist();
		void play_song( const QFileInfo &song_file );
		void stop_song();
		void change_volume( const int &value );
		void next_song();
		void previous_song();
		void media_status_changed();
		void pause_unpause_song();
		void change_replay( const bool &state );
		void change_random_track_state( const bool &state );
		void set_song_speed( QString value );
		void change_song_position( const quint64 &position );

		QTime get_song_time( const int &value );
};
