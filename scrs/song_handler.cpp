#include "../headers/song_handler.hpp"

Song_handler::Song_handler()
{
	m_player = new QMediaPlayer();
	m_output = new QAudioOutput();

	m_output->setVolume( 0.5 );
	m_player->setAudioOutput( m_output );
	m_player->setLoops( -1 );
	m_player->connect( m_player, &QMediaPlayer::mediaStatusChanged, [this]{ media_status_changed(); } );
	m_player->connect( m_player, &QMediaPlayer::positionChanged, [this]( quint64 position ){  update_position_slider( position ) ;} );
	m_player->connect( m_player, &QMediaPlayer::durationChanged, [this]( quint64 duration ) { duration_changed( duration ) ;} );
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

	m_max_song_index = m_playlist_songs.size();
	randomize_playlist_index();
}

void Song_handler::reset_playlist()
{
	m_playlist_songs.clear();
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

void Song_handler::set_ui_elements( QLabel &song_label, QSlider &position_slider, QLabel &duration_label, QLabel &position_label )
{
	m_song_label = &song_label;
	m_position_slider = &position_slider;

	m_duration_label = &duration_label;
	m_position_label = &position_label;
}

void Song_handler::play_song( const QFileInfo &song_file )
{
	m_current_song_index = m_playlist_songs.indexOf( song_file );
	m_player->setSource( QUrl::fromLocalFile( m_playlist_songs.at( m_current_song_index ).absoluteFilePath() ));

	m_song_label->setText( m_player->source().fileName().remove(".mp3") );
	m_position_slider->setValue( 0 );

	m_player->setPlaybackRate( m_song_speed );
	m_player->play();
}

void Song_handler::stop_song()
{
	m_player->setSource( QUrl("") );
	m_player->stop();

	m_song_label->setText( "" );
	m_position_label->setText( "" );
	m_duration_label->setText( "" );
}

void Song_handler::change_volume( const int &value )
{
	float volume = ( static_cast<float>( value ) / 100 );
	m_output->setVolume( volume );
}

void Song_handler::next_song()
{
	if (m_playlist_songs.empty() || m_player->source().isEmpty())
	{
		return;
	}

	if (m_random_track)
	{
		m_current_song_index = get_random_index();
		play_song( m_playlist_songs.at( m_current_song_index ) );

		return;
	}

	if ((m_current_song_index + 1) == m_max_song_index)
	{
		m_current_song_index = 0;
		play_song( m_playlist_songs.first() );

		return;
	}

	++m_current_song_index;
	play_song( m_playlist_songs.at( m_current_song_index ) );
}

int Song_handler::get_random_index()
{
	static int index = -1;
	++index;

	if (index == m_max_song_index)
	{
		index = 0;
	}

	return m_random_index[ index ];
}

void Song_handler::previous_song()
{
	if (m_playlist_songs.empty() || !m_player->isPlaying())
	{
		return;
	}

	if ((m_current_song_index - 1) < 0)
	{
		m_current_song_index = m_max_song_index - 1;
		play_song( m_playlist_songs.last() );

		return;
	}

	--m_current_song_index;
	play_song( m_playlist_songs.at( m_current_song_index ) );
}

void Song_handler::media_status_changed()
{
	if (m_player->mediaStatus() == QMediaPlayer::EndOfMedia)
	{
		next_song();
		return;
	}
}

void Song_handler::pause_unpause_song()
{
	(m_player->isPlaying()) ? m_player->pause() : m_player->play();
}

void Song_handler::change_replay( const bool &state )
{
	m_loop_track = state;
	( m_loop_track ) ? m_player->setLoops( -1 ) : m_player->setLoops( 1 );
}

void Song_handler::change_random_track_state( const bool &state )
{
	m_random_track = state;
}

void Song_handler::set_song_speed( const QString &speed )
{
	QString value = speed;
	value.remove("x");

	m_song_speed = value.toFloat();
	m_player->setPlaybackRate( m_song_speed );
}

void Song_handler::update_position_slider( const quint64 &position )
{
	int seconds = ( position / 1000 ) % 60;
	int minutes = ( position / 60000 ) % 60;

	QTime time( minutes, seconds );
	m_position_label->setText( time.toString() );
	m_position_slider->setValue( position );
}

void Song_handler::duration_changed( const quint64 &duration )
{
	int seconds = ( duration / 1000 ) % 60;
	int minutes = ( duration / 60000 ) % 60;

	QTime time( minutes, seconds );
	m_duration_label->setText(time.toString());
	m_position_slider->setMaximum( duration );
}

void Song_handler::change_song_position( const quint64 &position )
{

	m_player->setPosition( position );
}
