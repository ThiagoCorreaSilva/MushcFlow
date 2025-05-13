#pragma once

#include <QDialog>

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
};

