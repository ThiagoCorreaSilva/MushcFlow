#pragma once

#include <QDialog>
#include <QWidget>

#include <QFileSystemWatcher>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>

#include "player.hpp"
#include "logs.hpp"

namespace Ui {
class Playlist;
}

class Playlist : public QDialog
{
		Q_OBJECT

	public:
		explicit Playlist(QWidget *parent = nullptr);
		~Playlist();

	private:
		Ui::Playlist *ui;

		void read_config_file();
		void config_watcher_to_songs_dir();
		void songs_dir_watcher_event();
		void refresh_songs_list();
		void play_song( const QString &song_name );

		QWidget *m_container;
		QVBoxLayout *m_layout;
		QFileSystemWatcher m_watcher;

		QString m_songs_dir_path;

		Logs log;
};

