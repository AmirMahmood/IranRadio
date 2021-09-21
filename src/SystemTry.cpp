#include <QApplication>
#include <QSettings>

#include "SystemTry.h"


SystemTry::SystemTry(QObject *parent) {
    trayIcon = new QSystemTrayIcon(QIcon(":/img/icon"));

    trayMenu = new QMenu(nullptr);
    currentStationAction = trayMenu->addAction("");
    currentStationAction->setDisabled(true);
    trayMenu->addSeparator();
    mediaControllerAction = trayMenu->addAction(QIcon(":/img/play"), tr("Play"), RadioPlayer::getInstance(),
                                                &RadioPlayer::toggleRadioPlayerState);
    trayMenu->addSeparator();
    nationalStationsMenu = trayMenu->addMenu(tr("Nationals"));
    regionalStationsMenu = trayMenu->addMenu(tr("Regionals"));
    makeStationMenus();
    trayMenu->addSeparator();
    auto volumeMenu = trayMenu->addMenu(tr("Change volume"));
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
    trayMenu->addAction(tr("Show player"), this, &SystemTry::showPlayerWindow);
    QAction *exitAction = trayMenu->addAction(tr("Exit"));

    connect(exitAction, &QAction::triggered, this, &qApp->quit);
    connect(RadioPlayer::getInstance(), &RadioPlayer::radioPlayerStateChanged, this,
            &SystemTry::onRadioPlayerStateChanged);
    // init mediaControllerAction
    onRadioPlayerStateChanged(RadioPlayer::getInstance()->getRadioPlayerState());
    connect(PlayList::getInstance(), &PlayList::radioStationChanged, this, &SystemTry::onRadioStationChanged);
    // init currentStationAction text
    onRadioStationChanged(*(PlayList::getInstance()->selectedRadioStation));

    connect(trayIcon, &QSystemTrayIcon::activated, this, &SystemTry::tryIconClicked);

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
                //QIcon(element.iconURL),
                element.name,
                PlayList::getInstance(),
                [=]() { PlayList::getInstance()->setCurrentRadioStation(element.type, element.channelID); }
        );
    }

    for (const auto &element: playList->getRegionalRadioStations()) {
        regionalStationsMenu->addAction(
                //QIcon(element.iconURL),
                element.name,
                PlayList::getInstance(),
                [=]() { PlayList::getInstance()->setCurrentRadioStation(element.type, element.channelID); }
        );
    }
}

void SystemTry::onRadioPlayerStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::State::PlayingState) {
        mediaControllerAction->setText(tr("Stop"));
        mediaControllerAction->setIcon(QIcon(":/img/stop"));
    } else {
        mediaControllerAction->setText(tr("Play"));
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

void SystemTry::tryIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::ActivationReason::Trigger){
        QSettings settings;
        settings.beginGroup("settings");
        if (settings.value("try-clicked", TryClickAction::ShowPlayerWin) == TryClickAction::PlayStop){
            RadioPlayer::getInstance()->toggleRadioPlayerState();
        } else {
            if (playerWindow == nullptr){
                showPlayerWindow();
            } else {
                playerWindow->onTryIconClicked();
            }
        }
        settings.endGroup();
    }
}
