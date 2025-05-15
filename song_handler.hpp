#pragma once

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QDir>

class Song_handler
{
	private:
		QMediaPlayer *m_player;
		QAudioOutput *m_output;
		QFileInfoList m_playlist_songs;

	public:
		Song_handler();
		void set_playlist( const QString &playlist_path );
		void reset_playlist();
		void play_song( const QFileInfo &song_file );
		void change_volume( const int &value );
		void next_song();
		void previous_song();
		void song_ended();
		void pause_unpause_song();
};
