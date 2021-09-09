#include <QApplication>

#include "SystemTry.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);
    QCoreApplication::setOrganizationName("IranRadio");
    QCoreApplication::setOrganizationDomain("https://github.com/AmirMahmood");
    QCoreApplication::setApplicationName("IranRadio");;

    PlayList::getInstance();  // init PlayList;
    RadioPlayer::getInstance();  // init RadioPlayer

    SystemTry systemTry;

    return QApplication::exec();
}
