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


public slots:

    void onRadioStationChanged(const RadioStation &rs);

    void onRadioPlayerStateChanged(QMediaPlayer::State state);

private slots:

    void showPlayerWindow();

    void tryIconClicked(QSystemTrayIcon::ActivationReason reason);

private:
    QMenu *nationalStationsMenu, *regionalStationsMenu, *trayMenu;
    QSystemTrayIcon *trayIcon;
    QAction *currentStationAction, *mediaControllerAction;

    PlayerWindow *playerWindow = nullptr;

    void makeStationMenus();

};


#endif //IRANRADIO_SYSTEMTRY_H
