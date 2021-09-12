#include <QNetworkRequest>
#include <QStandardPaths>

#include "NetworkManager.h"

NetworkManager *NetworkManager::s_instance = nullptr;

NetworkManager *NetworkManager::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new NetworkManager();
        qDebug() << "NetworkManager is created";
    }
    return s_instance;
}

NetworkManager::NetworkManager() {
    restClient = new QNetworkAccessManager(this);
    diskCache = new QNetworkDiskCache(this);
    diskCache->setCacheDirectory(
            QStandardPaths::writableLocation(QStandardPaths::StandardLocation::AppDataLocation)
    );
    restClient->setCache(diskCache);
}

QNetworkReply *NetworkManager::getJson(const QString &url) {
    /*
     * always call close and deleteLater functions after using QNetworkReply object
     */
    return getJson(QUrl(url));
}

QNetworkReply *NetworkManager::getJson(const QUrl &url) {
    /*
     * always call close and deleteLater functions after using QNetworkReply object
     */
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application / json");
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork);
    qDebug() << "New HTTP request is sent";
    return restClient->get(request);
}

QNetworkReply *NetworkManager::getFile(const QUrl &url) {
    QNetworkRequest request{url};
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    return restClient->get(request);
}
