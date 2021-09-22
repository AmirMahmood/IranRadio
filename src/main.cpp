#include <QApplication>
#include <QTranslator>
#include <QSettings>

#include "SystemTry.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);
    QCoreApplication::setOrganizationName("IranRadio");
    QCoreApplication::setOrganizationDomain("https://github.com/AmirMahmood");
    QCoreApplication::setApplicationName("IranRadio");

    QSettings settings;
    settings.beginGroup("settings");
    settings.setValue("__current_lang", SystemTry::Langs::En);

    QTranslator translator;
    if (settings.value("lang", SystemTry::Langs::En) == SystemTry::Langs::Fa){
        if (translator.load("IranRadio_fa_IR")){
            a.installTranslator(&translator);
            settings.setValue("__current_lang", SystemTry::Langs::Fa);
        }
    }

    settings.endGroup();

    PlayList::getInstance();  // init PlayList;
    RadioPlayer::getInstance();  // init RadioPlayer

    SystemTry systemTry;

    return QApplication::exec();
}
