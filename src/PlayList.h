#ifndef IRANRADIO_PLAYLIST_H
#define IRANRADIO_PLAYLIST_H

#include <QObject>
#include <QString>

enum class RadioStationType {
    National, Regional
};

struct RadioStation {
    QString name;
    QString streamURL;
    QString iconURL;
    RadioStationType type;
    int channelID;
};

class PlayList : public QObject {
Q_OBJECT
public:
    RadioStation *selectedRadioStation;

    QVector<RadioStation> getNationalRadioStations();

    QVector<RadioStation> getRegionalRadioStations();

    static PlayList *getInstance();

public slots:

    void setCurrentRadioStation(RadioStationType type, int channelID);
    void setCurrentRadioStation(int channelID);

    void onAppClose();

signals:

    void radioStationChanged(RadioStation newRadioStation);


private:
    PlayList();

    static PlayList *s_instance;
    QVector<QVector<RadioStation>> *stationsList;
};


#endif //IRANRADIO_PLAYLIST_H
