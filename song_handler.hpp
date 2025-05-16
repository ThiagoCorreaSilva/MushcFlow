#pragma once

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDebug>
#include <QLabel>
#include <QDir>

class Song_handler
{
	private:
		QMediaPlayer *m_player;
		QAudioOutput *m_output;
		QFileInfoList m_playlist_songs;
		int m_current_song_index;
		int m_max_song_index;
		QLabel *m_song_label;

		void debug_mode();

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
};
