#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QObject>
#include <QNetworkDiskCache>

class NetworkManager : public QObject {
Q_OBJECT
public:
    QNetworkReply *getJson(const QString &url);
    QNetworkReply *getJson(const QUrl &url);

    static NetworkManager *getInstance();

private:
    NetworkManager();
    static NetworkManager *s_instance;

    QNetworkAccessManager *restClient;
    QNetworkDiskCache *diskCache;
};

#endif // NETWORKMANAGER_H
