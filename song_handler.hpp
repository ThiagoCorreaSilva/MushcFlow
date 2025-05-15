#pragma once

#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>

class Song_handler
{
	private:
		QMediaPlayer *m_player;
		QAudioOutput *m_output;

	public:
		Song_handler();
		void play_song( const QString &song_path );
		void change_volume( const int &volume );
		void next_song();
		void previous_song();
};

