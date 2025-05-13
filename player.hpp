#pragma once

#include <QDialog>

namespace Ui {
class Player;
}

class Player : public QDialog
{
		Q_OBJECT

	public:
		explicit Player(QWidget *parent = nullptr);
		~Player();

	private:
		Ui::Player *ui;
};

