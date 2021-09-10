#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

#include "PlayList.h"

PlayList *PlayList::s_instance = nullptr;

PlayList *PlayList::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new PlayList();
        qDebug() << "PlayList is created";
    }
    return s_instance;
}

PlayList::PlayList() {
    selectedRadioStation = new RadioStation;
    stationsList = new QVector<QVector<RadioStation>>;

    QFile file("./resources/other/stations.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonObject data = QJsonDocument::fromJson(file.readAll()).object();

    QJsonArray nationalsData = data["Nationals"].toArray();
    QVector<RadioStation> vec1;
    for (const auto &element: nationalsData) {
        auto jo = element.toObject();
        QString iconURL = jo["icon-url"].toString();
        QString tmpIconName = iconURL.right(iconURL.length() - iconURL.lastIndexOf("/") - 1);
        QString stationIconName = tmpIconName.left(tmpIconName.indexOf("?"));
        RadioStation rs{
                jo["name"].toString(),
                jo["stream-url"].toString(),
                //jo["icon-url"].toString(),
                "./resources/other/stations_icon/" + stationIconName,
                RadioStationType::National,
                jo["channel-id"].toInt(),
        };
        vec1.push_back(rs);
    }

    QJsonArray regionalsData = data["Regionals"].toArray();
    QVector<RadioStation> vec2;
    for (const auto &element: regionalsData) {
        auto jo = element.toObject();
        QString iconURL = jo["icon-url"].toString();
        QString tmpIconName = iconURL.right(iconURL.length() - iconURL.lastIndexOf("/") - 1);
        QString stationIconName = tmpIconName.left(tmpIconName.indexOf("?"));
        RadioStation rs{
                jo["name"].toString(),
                jo["stream-url"].toString(),
                //jo["icon-url"].toString(),
                "./resources/other/stations_icon/" + stationIconName,
                RadioStationType::Regional,
                jo["channel-id"].toInt(),
        };
        vec2.push_back(rs);
    }

    stationsList->push_back(vec1);
    stationsList->push_back(vec2);

    connect(qApp, &QApplication::aboutToQuit, this, &PlayList::onAppClose);

    QSettings settings;
    settings.beginGroup("playlist");
    setCurrentRadioStation(
            (RadioStationType) settings.value("last-station-type", (int) RadioStationType::National).toInt(),
            settings.value("last-station-ID", 11).toInt()
    );
    settings.endGroup();
}

void PlayList::onAppClose() {
    QSettings settings;
    settings.beginGroup("playlist");
    settings.setValue("last-station-ID", selectedRadioStation->channelID);
    settings.setValue("last-station-type", (int) selectedRadioStation->type);
    settings.endGroup();
}

void PlayList::setCurrentRadioStation(RadioStationType type, int channelID) {
    if (channelID == selectedRadioStation->channelID) {
        return;
    }

    QVector<RadioStation> vect;
    if (type == RadioStationType::National)
        vect = stationsList->at(0);
    else if (type == RadioStationType::Regional)
        vect = stationsList->at(1);

    for (const auto &element: vect) {
        if (element.channelID == channelID) {
            *selectedRadioStation = element;
            qDebug() << "New RadioStation is selected";
            emit radioStationChanged(element);
            break;
        }
    }
}

void PlayList::setCurrentRadioStation(int channelID) {
    if (channelID == selectedRadioStation->channelID) {
        return;
    }

    for (const auto &vectElement: *stationsList) {
        for (const auto &element: vectElement) {
            if (element.channelID == channelID) {
                *selectedRadioStation = element;
                qDebug() << "New RadioStation is selected";
                emit radioStationChanged(element);
                break;
            }
        }
    }
}

QVector<RadioStation> PlayList::getNationalRadioStations() {
    return stationsList->at(0);
}

QVector<RadioStation> PlayList::getRegionalRadioStations() {
    return stationsList->at(1);
}
