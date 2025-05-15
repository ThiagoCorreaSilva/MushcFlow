#include "song_handler.hpp"

Song_handler::Song_handler()
{
	m_player = new QMediaPlayer();
	m_output = new QAudioOutput();

	m_player->setAudioOutput( m_output );
}

void Song_handler::play_song( const QString &song_path )
{
	m_player->setSource( song_path );
	m_player->play();
}

void Song_handler::change_volume( const int &volume )
{

}

void Song_handler::next_song()
{

}

void Song_handler::previous_song()
{

}
