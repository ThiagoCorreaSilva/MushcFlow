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

void Song_handler::set_playlist( const QFileInfoList &playlist )
{
	reset_playlist();

	for ( const QFileInfo &file : playlist )
	{
		if (!file.fileName().contains(".mp3"))
		{
			continue;
		}

		m_playlist_songs.push_back( file );
	}

	m_max_song_index = m_playlist_songs.size() - 1;
	randomize_playlist_index();
}

void Song_handler::reset_playlist()
{
	m_playlist_songs.clear();
}

void Song_handler::set_song_label( QLabel &song_label )
{
	m_song_label = &song_label;
}

void Song_handler::play_song( const QFileInfo &song_file )
{
	m_current_song_index = m_playlist_songs.indexOf( song_file );
	m_player->setSource( m_playlist_songs.at( m_current_song_index ).absoluteFilePath() );

	if (m_song_label != nullptr)
	{
		m_song_label->setText( m_player->source().fileName().remove(".mp3") );
	}

	m_player->setPlaybackRate( m_song_speed );
	m_player->play();
	//debug_mode();
}

void Song_handler::debug_mode()
{
	int next_song_index;
	int previous_song_index;

	qDebug() << "\nCURRENT INDEX: " << m_current_song_index;
	qDebug() << "CURRENT SONG: " << m_playlist_songs.at( m_current_song_index ).fileName().remove(".mp3");

	qDebug() << "NEXT INDEX: " << next_song_index;
	qDebug() << "NEXT SONG:" << m_playlist_songs.at( next_song_index ).fileName().remove(".mp3");

	qDebug() << "PREVIOUS INDEX: " << previous_song_index;
	qDebug() << "PREVIOUS SONG: " << m_playlist_songs.at( previous_song_index ).fileName().remove(".mp3");

	qDebug() << "SIZE OF PLAYLIST: " << m_max_song_index;
	qDebug() << "SIZE OF PLAYLIST (-1): " << m_max_song_index -1 << '\n';

	qDebug() << "SPEED: " << m_song_speed;
}

void Song_handler::change_volume( const int &value )
{
	float volume = ( static_cast<float>( value ) / 100 );
	m_output->setVolume( volume );
}

void Song_handler::next_song()
{
	if (m_playlist_songs.empty())
	{
		qDebug() << "EMPTY PLAYLIST!";
		return;
	}

	if ((m_current_song_index + 1) > (m_max_song_index - 1))
	{
		m_current_song_index = 0;
		play_song( m_playlist_songs.at( m_current_song_index ) );

		return;
	}

	if (m_random_track)
	{
		m_current_song_index = get_random_index();
		play_song( m_playlist_songs.at( m_current_song_index ) );

		return;
	}

	++m_current_song_index;
	play_song( m_playlist_songs.at( m_current_song_index ) );
}

int Song_handler::get_random_index()
{
	static int index = -1;
	++index;

	if (index == m_playlist_songs.size())
	{
		index = 0;
	}

	return m_random_index[ index ];
}

void Song_handler::previous_song()
{
	if (m_playlist_songs.empty())
	{
		qDebug() << "EMPTY PLAYLIST!";
		return;
	}

	if ((m_current_song_index - 1) < 0)
	{
		m_current_song_index = m_max_song_index;
		play_song( m_playlist_songs.at( m_current_song_index ) );

		return;
	}

	--m_current_song_index;
	play_song( m_playlist_songs.at( m_current_song_index ) );
}

void Song_handler::media_status_changed()
{
	if (m_player->mediaStatus() != QMediaPlayer::EndOfMedia)
	{
		return;
	}

	next_song();
}

void Song_handler::pause_unpause_song()
{
	(m_player->isPlaying()) ? m_player->pause() : m_player->play();
}

void Song_handler::change_replay( const bool &state )
{
	( state ) ? m_player->setLoops( -1 ) : m_player->setLoops( 1 );
}

void Song_handler::change_random_track_state( const bool &state )
{
	m_random_track = state;
}

void Song_handler::randomize_playlist_index()
{
	m_random_index.clear();

	for ( int i = 0; i < m_playlist_songs.size(); i++ )
	{
		m_random_index.append( i );
	}
	qDebug() << "SIZE: " << m_random_index.size();

	std::random_shuffle( m_random_index.begin(), m_random_index.end());
}

void Song_handler::set_song_speed( const float &speed )
{
	m_song_speed = speed;
	m_player->setPlaybackRate( m_song_speed );
}
