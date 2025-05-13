#pragma once

#include <QDialog>

#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDir>

#include "logs.hpp"

namespace Ui {
class First_Open_Config;
}

class First_Open_Config : public QDialog
{
        Q_OBJECT

    public:
        explicit First_Open_Config(QWidget *parent = nullptr);
        ~First_Open_Config();

    private slots:
        void on_select_folder_button_clicked();

        void on_confirm_button_clicked();

    private:
        Ui::First_Open_Config *ui;

        const QString m_app_dir_name = "MushcFlow";
        const QString m_songs_dir_name = "Songs";

        QString m_app_dir_path;
        QString m_songs_dir_path;

        Logs log;

        void create_config_file();
        void create_app_directories();
};
