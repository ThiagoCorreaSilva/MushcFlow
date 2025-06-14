#pragma once

#include <QDialog>

#include <QtConcurrent/QtConcurrent>
#include <QProcess>
#include <QSysInfo>
#include <QThread>
#include <QFile>

#include "config_file_handler.hpp"
#include "song_folder_manager.hpp"
#include "logs.hpp"

namespace Ui {
class Downloader;
}

class Downloader : public QDialog
{
		Q_OBJECT

	public:
		explicit Downloader(QWidget *parent = nullptr);
		~Downloader();

	private slots:
		void on_download_button_clicked();

	private:
		Ui::Downloader *ui;

		QString m_songs_dir_path;

		void read_config_file();
		void start_download();
		void save_thumbnail_format();

		void debug( const QMap< QString, QString > &content );
};

