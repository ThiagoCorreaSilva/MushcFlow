#pragma once

#include <QDialog>

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
};

