#ifndef IRANRADIO_RADIOPLAYER_H
#define IRANRADIO_RADIOPLAYER_H

#include <QObject>
#include <QMediaPlayer>

#include "PlayList.h"

class RadioPlayer : public QObject {
Q_OBJECT

public:
    static RadioPlayer *getInstance();

    void play();

    void stop();

    QMediaPlayer::State getRadioPlayerState();

public slots:

    void toggleRadioPlayerState();

    void changeStreamSource(const RadioStation &rs);

    void setVolume(int);
    int getVolume();

    void onAppClose();

private slots:

    void onMediaPlayerStateChanged(QMediaPlayer::State state);

signals:

    void radioPlayerStateChanged(QMediaPlayer::State state);

    void volumeChanged(int newVolume);

private:
    explicit RadioPlayer(QObject *parent = nullptr);

    static RadioPlayer *s_instance;
    QMediaPlayer *mediaPlayer;
    QString currentStream;

};


#endif //IRANRADIO_RADIOPLAYER_H
