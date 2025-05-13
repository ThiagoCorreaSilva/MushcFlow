#include "downloader.hpp"
#include "ui_downloader.h"

Downloader::Downloader(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Downloader)
{
	ui->setupUi(this);
}

Downloader::~Downloader()
{
	delete ui;
}
