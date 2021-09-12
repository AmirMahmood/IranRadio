#include "CustomListWidgetItem.h"
#include "NetworkManager.h"

CustomListWidgetItem::CustomListWidgetItem(const QUrl &iconURL, const QString &stationName)
        : QObject(nullptr), QListWidgetItem(QIcon(":/img/placeholder"), stationName) {
    QNetworkReply *reply = NetworkManager::getInstance()->getFile(iconURL);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        QPixmap p;
        p.loadFromData(reply->readAll());
        setIcon(QIcon(p));
    });
}
