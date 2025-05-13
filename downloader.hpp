#pragma once

#include <QDialog>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
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

	private:
		Ui::Downloader *ui;

		QString m_songs_dir_path;

		Logs log;

		void read_config_file();
		void start_download();
		void check_if_songs_exists_in_folder();
};

