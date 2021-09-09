#include <QSettings>
#include <QApplication>
#include "RadioPlayer.h"


RadioPlayer *RadioPlayer::s_instance = nullptr;

RadioPlayer *RadioPlayer::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new RadioPlayer();
        qDebug() << "RadioPlayer is created";
    }
    return s_instance;
}

RadioPlayer::RadioPlayer(QObject *parent) {
    mediaPlayer = new QMediaPlayer();
    connect(mediaPlayer, &QMediaPlayer::stateChanged, this, &RadioPlayer::onMediaPlayerStateChanged);
    connect(PlayList::getInstance(), &PlayList::radioStationChanged, this, &RadioPlayer::changeStreamSource);
    connect(qApp, &QApplication::aboutToQuit, this, &RadioPlayer::onAppClose);
    connect(
            mediaPlayer,
            &QMediaPlayer::volumeChanged,
            this,
            [=](int volume) { qDebug() << "emit RadioPlayer volumeChanged signal"; emit volumeChanged(volume); }
    );

    QSettings settings;
    settings.beginGroup("radio-player");
    setVolume(settings.value("volume", 100).toInt());
    settings.endGroup();

    // init mediaPlayer media and play
    changeStreamSource(*(PlayList::getInstance()->selectedRadioStation));
}

void RadioPlayer::onAppClose() {
    QSettings settings;
    settings.beginGroup("radio-player");
    settings.setValue("volume", mediaPlayer->volume());
    settings.endGroup();
}

void RadioPlayer::play() {
    if (mediaPlayer->state() == QMediaPlayer::State::PlayingState) {
        return;
    }
    mediaPlayer->setMedia(QUrl(currentStream));
    mediaPlayer->play();
}

void RadioPlayer::changeStreamSource(const RadioStation &rs) {
    stop();
    currentStream = rs.streamURL;
    play();
}

void RadioPlayer::onMediaPlayerStateChanged(QMediaPlayer::State state) {
    emit radioPlayerStateChanged(state);
}

QMediaPlayer::State RadioPlayer::getRadioPlayerState() {
    return mediaPlayer->state();
}

void RadioPlayer::stop() {
    mediaPlayer->stop();
    mediaPlayer->setMedia(nullptr);

}

void RadioPlayer::toggleRadioPlayerState() {
    if (mediaPlayer->state() == QMediaPlayer::State::PlayingState) {
        qDebug() << "Stop player";
        stop();
    } else {
        qDebug() << "Play player";
        play();
    }
}

void RadioPlayer::setVolume(int volume) {
    mediaPlayer->setVolume(volume);
    qDebug() << "RadioPlayer Volume changed";
}

int RadioPlayer::getVolume(){
    return mediaPlayer->volume();
}