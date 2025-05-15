#pragma once

#include <QDialog>

#include <QMessageBox>
#include <QProcess>
#include <QSysInfo>
#include <QFile>

#include "config_file_handler.hpp"
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

        Logs log;

		void read_config_file();
		void start_download();
};

