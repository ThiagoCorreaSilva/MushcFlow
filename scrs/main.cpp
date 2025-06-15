#include "../headers/mainwindow.hpp"

#include <QApplication>

#include <QSplashScreen>
#include <QTimer>

const int DURATION = 1500;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.set_app( &a );
    w.centralize_window();
    w.setWindowIcon( QIcon( "MushcFlowLogo.png" ) );

    QSplashScreen *splash_screen = new QSplashScreen;
    splash_screen->setPixmap( QPixmap( "MushcFlowLogo.png" ) );
    splash_screen->show();

    QTimer::singleShot( DURATION, splash_screen, &QSplashScreen::close );
    QTimer::singleShot( DURATION, &w, &QMainWindow::show );

    return a.exec();
}
