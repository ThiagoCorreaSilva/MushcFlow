#pragma once

#include <QMainWindow>
#include <QScreen>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

#include "logs.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void centralize_window( const QApplication &app );

    private:
        Ui::MainWindow *ui;
        void config_app_directories();
        void create_config_file();

        const QString m_config_file_name = "configs.json";
        const QString m_app_dir_name = "MushcFlow";
        const QString m_songs_dir_name = "Songs";

        Logs log;
        QFile m_config_file;
};
