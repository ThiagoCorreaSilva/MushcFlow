#include "song_handler.hpp"

Song_handler::Song_handler()
{
	m_player = new QMediaPlayer();
	m_output = new QAudioOutput();

	m_output->setVolume( 0.5 );
	m_player->setAudioOutput( m_output );
	m_player->setLoops( -1 );
	m_player->connect( m_player, &QMediaPlayer::mediaStatusChanged, [this]{ media_status_changed(); } );
}

void Song_handler::set_playlist( const QString &playlist_path )
{
	QDir songs_dir( playlist_path );
	QFileInfoList files = songs_dir.entryInfoList();
	{
		for ( auto &file : std::as_const(files) )
		{
			if (!file.fileName().contains(".mp3"))
			{
				continue;
			}

			m_playlist_songs.push_back( file.absoluteFilePath() );
		}
	}
}

void Song_handler::reset_playlist()
{
	m_playlist_songs.clear();
}

void Song_handler::play_song( const QFileInfo &song_file )
{
	m_player->setSource( song_file.absoluteFilePath() );
	m_player->play();
}

void Song_handler::change_volume( const int &value )
{
	float volume = ( static_cast<float>( value ) / 100 );
	m_output->setVolume( volume );
}

void Song_handler::next_song()
{

}

void Song_handler::previous_song()
{

}

void Song_handler::media_status_changed()
{
	if (m_player->mediaStatus() != QMediaPlayer::EndOfMedia)
	{
		return;
	}
}

void Song_handler::pause_unpause_song()
{
	(m_player->isPlaying()) ? m_player->pause() : m_player->play();
}

void Song_handler::change_replay( const bool &state )
{
	( state ) ? m_player->setLoops( -1 ) : m_player->setLoops( 1 );
}
