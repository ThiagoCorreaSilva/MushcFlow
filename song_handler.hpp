#pragma once

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QLabel>
#include <QDir>

#include <algorithm>

class Song_handler
{
	private:
		QMediaPlayer *m_player;
		QAudioOutput *m_output;
		QFileInfoList m_playlist_songs;

		int m_current_song_index;
		int m_max_song_index;
		float m_song_speed = 1;
		bool m_random_track;

		QLabel *m_song_label;
		QList <int> m_random_index;

		void randomize_playlist_index();
		int get_random_index();

	public:
		Song_handler();
		void set_playlist( const QFileInfoList &playlist_path );
		void set_song_label( QLabel &song_label );
		void reset_playlist();
		void play_song( const QFileInfo &song_file );
		void change_volume( const int &value );
		void next_song();
		void previous_song();
		void media_status_changed();
		void pause_unpause_song();
		void change_replay( const bool &state );
		void change_random_track_state( const bool &state );
		void set_song_speed( const QString &value );
};
