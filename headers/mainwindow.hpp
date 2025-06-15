#pragma once

#include <QMainWindow>
#include <QScreen>

#include "first_open_config.hpp"
#include "downloader.hpp"
#include "playlist.hpp"
#include "theme.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        MainWindow( QWidget *parent = nullptr );
        ~MainWindow();

        void set_app( QApplication *app );
        void centralize_window();

    private:
        Ui::MainWindow *ui;
        QApplication *app;

        void make_configurations();
        void set_tab_widgets();
};
