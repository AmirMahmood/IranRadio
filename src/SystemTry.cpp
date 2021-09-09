#include <QApplication>

#include "SystemTry.h"


SystemTry::SystemTry(QObject *parent) {
    trayIcon = new QSystemTrayIcon(QIcon(":/img/icon"));

    trayMenu = new QMenu(nullptr);
    currentStationAction = trayMenu->addAction("");
    currentStationAction->setDisabled(true);
    trayMenu->addSeparator();
    mediaControllerAction = trayMenu->addAction(QIcon(":/img/play"), "Play", RadioPlayer::getInstance(),
                                                &RadioPlayer::toggleRadioPlayerState);
    trayMenu->addSeparator();
    nationalStationsMenu = trayMenu->addMenu("Nationals");
    regionalStationsMenu = trayMenu->addMenu("Regionals");
    makeStationMenus();
    trayMenu->addSeparator();
    auto volumeMenu = trayMenu->addMenu("Change volume");
    volumeMenu->addAction(
            "100%",
            RadioPlayer::getInstance(),
            [=]() { RadioPlayer::getInstance()->setVolume(100); }
    );
    volumeMenu->addAction(
            "75%",
            RadioPlayer::getInstance(),
            [=]() { RadioPlayer::getInstance()->setVolume(75); }
    );
    volumeMenu->addAction(
            "50%",
            RadioPlayer::getInstance(),
            [=]() { RadioPlayer::getInstance()->setVolume(50); }
    );
    volumeMenu->addAction(
            "25%",
            RadioPlayer::getInstance(),
            [=]() { RadioPlayer::getInstance()->setVolume(25); }
    );
    trayMenu->addAction("Show player", this, &SystemTry::showPlayerWindow);
    QAction *exitAction = trayMenu->addAction("Exit");

    connect(exitAction, &QAction::triggered, this, &qApp->quit);
    connect(RadioPlayer::getInstance(), &RadioPlayer::radioPlayerStateChanged, this,
            &SystemTry::onRadioPlayerStateChanged);
    // init mediaControllerAction
    onRadioPlayerStateChanged(RadioPlayer::getInstance()->getRadioPlayerState());
    connect(PlayList::getInstance(), &PlayList::radioStationChanged, this, &SystemTry::onRadioStationChanged);
    // init currentStationAction text
    onRadioStationChanged(*(PlayList::getInstance()->selectedRadioStation));

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
}

void SystemTry::onRadioStationChanged(const RadioStation &rs) {
    currentStationAction->setText(rs.name);
    trayIcon->setToolTip(rs.name);
}

void SystemTry::makeStationMenus() {
    PlayList *playList = PlayList::getInstance();
    for (const auto &element: playList->getNationalRadioStations()) {
        nationalStationsMenu->addAction(
                QIcon(":/img/icon"),
                element.name,
                PlayList::getInstance(),
                [=]() { PlayList::getInstance()->setCurrentRadioStation(element.type, element.channelID); }
        );
    }

    for (const auto &element: playList->getRegionalRadioStations()) {
        regionalStationsMenu->addAction(
                QIcon(":/img/icon"),
                element.name,
                PlayList::getInstance(),
                [=]() { PlayList::getInstance()->setCurrentRadioStation(element.type, element.channelID); }
        );
    }
}

void SystemTry::onRadioPlayerStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::State::PlayingState) {
        mediaControllerAction->setText("Stop");
        mediaControllerAction->setIcon(QIcon(":/img/stop"));
    } else {
        mediaControllerAction->setText("Play");
        mediaControllerAction->setIcon(QIcon(":/img/play"));
    }
}

void SystemTry::showPlayerWindow() {
    if (playerWindow == nullptr) {
        qDebug() << "New PlayerWindow object is created";
        playerWindow = new PlayerWindow();
    }
    playerWindow->show();
}
