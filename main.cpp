#include "mainwindow.hpp"

#include <QApplication>

#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.centralize_window( a );
    w.setWindowIcon( QIcon( ":/logo/MushcFlowLogo.png" ) );

    QSplashScreen *splash_screen = new QSplashScreen;
    splash_screen->setPixmap( QPixmap( ":/logo/MushcFlowLogo.png" ) );
    splash_screen->show();

    QTimer::singleShot( 2000, splash_screen, &QSplashScreen::close );
    QTimer::singleShot( 2000, &w, &QMainWindow::show );

    return a.exec();
}
