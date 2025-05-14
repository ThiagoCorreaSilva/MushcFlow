#pragma once

#include <QDialog>

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QFileInfo>
#include <QFile>

#include "logs.hpp"

namespace Ui {
class Player;
}

class Player : public QDialog
{
		Q_OBJECT

	public:
		explicit Player(QWidget *parent = nullptr);
		~Player();

		void play_song( const QFileInfo &song_info );

	private:
		Ui::Player *ui;

		QAudioOutput *m_output;
		QMediaPlayer *m_player;

		Logs log;
};
