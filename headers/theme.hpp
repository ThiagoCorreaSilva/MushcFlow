#pragma once

#include <QApplication>
#include <QDialog>

#include <QFileDialog>
#include <QFileInfo>
#include <QFile>

#include "config_file_handler.hpp"
#include "logs.hpp"

namespace Ui {
class Theme;
}

class Theme : public QDialog
{
		Q_OBJECT

	public:
		explicit Theme( QWidget *parent = nullptr, QApplication *app = nullptr );
		~Theme();

	private slots:
		void on_change_theme_button_clicked();

	private:
		Ui::Theme *ui;

		QApplication *app;
		const QString m_themes_path = "Themes";
};

