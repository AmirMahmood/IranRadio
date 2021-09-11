#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QObject>

class NetworkManager : public QObject {
Q_OBJECT
public:
    QNetworkReply *getJson(const QString &url);

    static NetworkManager *getInstance();

private:
    NetworkManager();
    static NetworkManager *s_instance;

    QNetworkAccessManager *restClient;
};

#endif // NETWORKMANAGER_H
