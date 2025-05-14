#include "player.hpp"
#include "ui_player.h"

Player::Player(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Player)
{
	ui->setupUi(this);

	m_player = new QMediaPlayer;
	m_output = new QAudioOutput;

	m_player->setAudioOutput( m_output );
}

Player::~Player()
{
	delete ui;
}

void Player::play_song( const QFileInfo &song_info )
{
	if (!QFile::exists( song_info.absoluteFilePath() ))
	{
		QString error_1 = "THE SONG: " + song_info.fileName() + " DONT EXISTS!";
		QString error_2 = "PLEASE TRY AGAIN OR DELETE THE SONG!";

		log.create_log( {error_1, error_2}, this ,ERROR_TYPE::NON_FATAL );

		return;
	}

	m_player->setSource( song_info.absoluteFilePath() );
	m_player->play();
}
