#include <QSettings>
#include <QApplication>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "RadioPlayer.h"
#include "NetworkManager.h"


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

    auto url = QUrl("http://api.iranseda.ir/radio/liveplayer/");
    QUrlQuery querystr;
    querystr.addQueryItem("CH", QString::number(rs.channelID));
    url.setQuery(querystr);
    QNetworkReply *reply = NetworkManager::getInstance()->getJson(url);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // wait until QNetworkReply emits finished signal
    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    reply->close();
    reply->deleteLater();
    currentStream = json["items"].toArray()[0].toObject()["playUrl"].toString();

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