#ifndef IRANRADIO_CUSTOMLISTWIDGETITEM_H
#define IRANRADIO_CUSTOMLISTWIDGETITEM_H

#include <QObject>
#include <QListWidgetItem>

class CustomListWidgetItem : public QObject, public QListWidgetItem {
    Q_OBJECT
public:
    CustomListWidgetItem(const QUrl &iconURL, const QString &stationName);
};


#endif //IRANRADIO_CUSTOMLISTWIDGETITEM_H
