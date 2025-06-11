#pragma once

#include <QDialog>

#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMap>

#include "config_file_handler.hpp"
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

        QStringList get_json_names();

    private slots:
        void on_select_folder_button_clicked();

        void on_confirm_button_clicked();

    private:
        Ui::First_Open_Config *ui;

        const QString m_app_dir_name = "MushcFlow";
        const QString m_songs_dir_name = "Songs";
        const QString m_thumbnails_dir_name = "Thumbnails";

        const QString m_app_dir_json = "app_dir";
        const QString m_songs_dir_json = "songs_dir";
        const QString m_thumbnails_dir_json = "thumbnails_dir";
        const QString m_use_thumbnail_json = "use_thumbnail";
        const QString m_thumbnail_format_json = "thumbnail_format";

        QString m_app_dir_path;
        QString m_songs_dir_path;
        QString m_thumbnails_dir_path;

        void create_config_file();
        void create_app_directories();
};
