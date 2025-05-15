#include "song_handler.hpp"

Song_handler::Song_handler()
{
	m_player = new QMediaPlayer();
	m_output = new QAudioOutput();

	m_output->setVolume( 0.5 );
	m_player->setAudioOutput( m_output );
	m_player->connect( m_player, &QMediaPlayer::mediaStatusChanged, [this]{ song_ended(); } );
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

			qDebug() << file.fileName();
			m_playlist_songs.push_back( file );
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

void Song_handler::song_ended()
{
	if (m_player->mediaStatus() == QMediaPlayer::EndOfMedia)
	{

	}
}

void Song_handler::pause_unpause_song()
{
	(m_player->isPlaying()) ? m_player->pause() : m_player->play();
}
