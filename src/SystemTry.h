#ifndef IRANRADIO_SYSTEMTRY_H
#define IRANRADIO_SYSTEMTRY_H


#include <QWidget>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QActionGroup>

#include "RadioPlayer.h"
#include "PlayList.h"
#include "PlayerWindow.h"

class SystemTry : public QWidget {
Q_OBJECT

public:
    explicit SystemTry(QObject *parent = nullptr);

    enum TryClickAction{
        PlayStop, ShowPlayerWin
    };
    Q_ENUM(TryClickAction)

    enum Langs{
        Fa, En
    };
    Q_ENUM(Langs)


public slots:

    void onRadioStationChanged(const RadioStation &rs);

    void onRadioPlayerStateChanged(QMediaPlayer::State state);

    void showPlayerWindow();

private slots:

    void tryIconClicked(QSystemTrayIcon::ActivationReason reason);

private:
    QMenu *nationalStationsMenu, *regionalStationsMenu, *trayMenu;
    QSystemTrayIcon *trayIcon;
    QAction *currentStationAction, *mediaControllerAction;

    PlayerWindow *playerWindow = nullptr;

    void makeStationMenus();

};


#endif //IRANRADIO_SYSTEMTRY_H
