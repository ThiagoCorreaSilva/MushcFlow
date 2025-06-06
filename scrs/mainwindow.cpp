#include "../headers/mainwindow.hpp"
#include "uis/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize( 500, 250 );

    open_configuration_window();
    set_tab_widgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::centralize_window( QApplication &app )
{
    int width = frameGeometry().width();
    int height = frameGeometry().height();

    QScreen *screen = app.primaryScreen();
    int screen_width = screen->geometry().width();
    int screen_height = screen->geometry().height();

    setGeometry( (screen_width / 2) - (width / 2), (screen_height / 2) - (height / 2), width, height );
}

void MainWindow::open_configuration_window()
{
    if (QFile::exists( "configs.json" ))
    {
        return;
    }

    First_Open_Config first_time_window;
    first_time_window.setModal( true );
    first_time_window.setFixedSize( 600, 250 );
    first_time_window.exec();
}

void MainWindow::set_tab_widgets()
{
    ui->tabWidget->addTab( new Playlist(), "Player" );
    ui->tabWidget->addTab( new Downloader(), "Downloader" );
}
