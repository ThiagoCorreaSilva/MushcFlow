#pragma once

#include <QMainWindow>
#include <QScreen>

#include "first_open_config.hpp"
#include "downloader.hpp"
#include "playlist.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();
        void centralize_window( QApplication &app );

    private:
        Ui::MainWindow *ui;
        void make_configurations();
        void set_tab_widgets();
};
