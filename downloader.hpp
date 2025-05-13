#pragma once

#include <QDialog>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QMessageBox>
#include <QJsonObject>
#include <QProcess>
#include <QSysInfo>
#include <QFile>

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
		QString m_app_dir_path;

		Logs log;

		void read_config_file();
		void start_download();
};

