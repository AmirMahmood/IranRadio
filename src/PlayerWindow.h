#ifndef IRANRADIO_PlayerWindow_H
#define IRANRADIO_PlayerWindow_H

#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include <QShowEvent>
#include <QMediaPlayer>

#include "CustomListWidgetItem.h"
#include "PlayList.h"
#include "RadioPlayer.h"


QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

class PlayerWindow : public QMainWindow {
Q_OBJECT

public:
    explicit PlayerWindow(QWidget *parent = nullptr);

    ~PlayerWindow() override;

public slots:

    void onRadioStationChanged(const RadioStation &rs);

    void onRadioPlayerStateChanged(QMediaPlayer::State state);

    void onAppClose();

    void selectNewRadioStation(QListWidgetItem *item);

private:

    void changeEvent(QEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

    void showEvent(QShowEvent *event) override;

    void saveWindowGeometry();

    Ui::PlayerWindow *ui;
};

#endif // IRANRADIO_PlayerWindow_H
