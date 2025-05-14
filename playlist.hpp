#pragma once

#include <QDialog>

#include <QFileSystemWatcher>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QFileInfo>
#include <QPalette>
#include <QPixmap>
#include <QWidget>
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
		explicit Playlist(QWidget *parent = nullptr, QTabWidget *tab = nullptr);
		~Playlist();

	private:
		Ui::Playlist *ui;

		void read_config_file();
		void config_watcher_to_songs_dir();
		void songs_dir_watcher_event();
		void refresh_songs_list();
		void set_pix_map( QPushButton &button, const QString &path );
		void play_song( const QFileInfo &song_info );

		QWidget *m_container;
		QVBoxLayout *m_layout;
		QFileSystemWatcher m_watcher;
		QTabWidget *m_tab;

		QString m_songs_dir_path;

		Player player;
		Logs log;
};

