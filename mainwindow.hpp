#pragma once

#include <QMainWindow>
#include <QScreen>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>

#include "first_open_config.hpp"
#include "downloader.hpp"
#include "playlist.hpp"
#include "player.hpp"
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
        void open_configuration_window();
        void set_tab_widgets();
};
