#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>

#include "PlayList.h"
#include "NetworkManager.h"

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

    getNewPlaylistFromAPI();

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

void PlayList::getNewPlaylistFromAPI() {
    QNetworkReply *reply = NetworkManager::getInstance()->getJson("http://api.iranseda.ir/radio/");
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(); // wait until QNetworkReply emits finished signal

    QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
    reply->close();
    reply->deleteLater();
    QVector<RadioStation> vec1;
    QVector<RadioStation> vec2;
    for (const auto &element: json["channels"].toArray()) {
        auto jo = element.toObject();
        QString iconURL = jo["logo"].toString();
        QString tmpIconName = iconURL.right(iconURL.length() - iconURL.lastIndexOf("/") - 1);
        QString stationIconName = tmpIconName.left(tmpIconName.indexOf("?"));
        RadioStation rs{
                jo["title"].toString(),
                jo["logo"].toString(),
                RadioStationType::National,
                jo["id"].toString().toInt(),
                //jo["name"].toString(),
        };
        if (jo["type"].toString().compare("ostani", Qt::CaseSensitivity::CaseInsensitive) == 0) {
            rs.type = RadioStationType::Regional;
            vec2.push_back(rs);
        } else {
            vec1.push_back(rs);
        }
    }
    stationsList->push_back(vec1);
    stationsList->push_back(vec2);
}
