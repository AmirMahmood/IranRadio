#include <QNetworkRequest>

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
}

QNetworkReply *NetworkManager::getJson(const QString &url) {
    /*
     * always call close and deleteLater functions after using QNetworkReply object
     */
    QNetworkRequest request{QUrl(url)};
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application / json");
    qDebug() << "New HTTP request is sent";
    return restClient->get(request);
}
