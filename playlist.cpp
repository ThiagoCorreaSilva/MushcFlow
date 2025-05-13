#include "playlist.hpp"
#include "ui_playlist.h"

Playlist::Playlist(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Playlist)
{
	ui->setupUi(this);
}

Playlist::~Playlist()
{
	delete ui;
}
