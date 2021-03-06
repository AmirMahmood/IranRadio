#include <QApplication>
#include <QSettings>
#include <QDebug>
#include <QLabel>
#include <QNetworkReply>
#include <QMenu>

#include "PlayerWindow.h"
#include "SettingsDialog.h"
#include "./ui_player_window.h"
#include "NetworkManager.h"


PlayerWindow::PlayerWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::PlayerWindow) {
    ui->setupUi(this);

    auto menu = new QMenu(this);
    ui->menuToolButton->setMenu(menu);
    menu->addAction(tr("Settings"), this, &PlayerWindow::showSettingsDialog);
    menu->addAction(tr("Exit"),  this, &qApp->quit);

    connect(RadioPlayer::getInstance(), &RadioPlayer::radioPlayerStateChanged, this,
            &PlayerWindow::onRadioPlayerStateChanged);
    onRadioPlayerStateChanged(RadioPlayer::getInstance()->getRadioPlayerState());  // init
    connect(PlayList::getInstance(), &PlayList::radioStationChanged, this, &PlayerWindow::onRadioStationChanged);
    onRadioStationChanged(*(PlayList::getInstance()->selectedRadioStation));  // init
    connect(qApp, &QApplication::aboutToQuit, this, &PlayerWindow::onAppClose);
    connect(ui->playerControlButton, &QPushButton::clicked, RadioPlayer::getInstance(),
            &RadioPlayer::toggleRadioPlayerState);
    connect(ui->nationalListWidget, &QListWidget::itemClicked, this, &PlayerWindow::selectNewRadioStation);
    connect(ui->regionalListWidget, &QListWidget::itemClicked, this, &PlayerWindow::selectNewRadioStation);
    connect(ui->volumeSlider, &QSlider::valueChanged, RadioPlayer::getInstance(), &RadioPlayer::setVolume);
    connect(RadioPlayer::getInstance(), &RadioPlayer::volumeChanged, ui->volumeSlider, &QSlider::setValue);
    connect(RadioPlayer::getInstance(), &RadioPlayer::volumeChanged, ui->volumeLabel,
            QOverload<int>::of(&QLabel::setNum));

    ui->volumeSlider->setValue(RadioPlayer::getInstance()->getVolume());
    ui->volumeLabel->setNum(RadioPlayer::getInstance()->getVolume());

    // style regional and national list widget
    ui->nationalListWidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    ui->regionalListWidget->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    ui->nationalListWidget->setIconSize(QSize(48, 48));
    ui->regionalListWidget->setIconSize(QSize(48, 48));
    QFont font = ui->nationalListWidget->font();
    font.setPointSize(10);
    ui->nationalListWidget->setFont(font);
    ui->regionalListWidget->setFont(font);

    PlayList *playList = PlayList::getInstance();
    for (const auto &element: playList->getNationalRadioStations()) {
        auto listWidgetItem = new CustomListWidgetItem(QUrl(element.iconURL), element.name);
        listWidgetItem->setData(Qt::UserRole, element.channelID);
        ui->nationalListWidget->addItem(listWidgetItem);
    }

    for (const auto &element: playList->getRegionalRadioStations()) {
        auto listWidgetItem = new CustomListWidgetItem(QUrl(element.iconURL), element.name);
        listWidgetItem->setData(Qt::UserRole, element.channelID);
        ui->regionalListWidget->addItem(listWidgetItem);
    }
}

PlayerWindow::~PlayerWindow() {
    delete ui;
}

void PlayerWindow::onTryIconClicked() {
    if (isVisible()) {
        close();
    } else {
        show();
    }
}

void PlayerWindow::saveWindowGeometry() {
    QSettings settings;
    settings.beginGroup("player-window");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
    qDebug() << "Player window's geometry is saved";
}

void PlayerWindow::closeEvent(QCloseEvent *event) {
    saveWindowGeometry();
    QMainWindow::closeEvent(event);

    QSettings settings;
    settings.beginGroup("settings");
    if (!settings.value("exit-to-try", true).toBool()){
        qApp->quit();
    }
    settings.endGroup();
}

void PlayerWindow::changeEvent(QEvent *event) {
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMinimized) {
            QSettings settings;
            settings.beginGroup("settings");
            if (settings.value("minimize-to-try", true).toBool()){
                saveWindowGeometry();
                event->ignore();
                hide();
            } else {
                QMainWindow::changeEvent(event);
            }
            settings.endGroup();
        }
    }
}

void PlayerWindow::onAppClose() {
    close();  // to save geometry
}

void PlayerWindow::showEvent(QShowEvent *event) {
    QSettings settings;
    settings.beginGroup("player-window");
    if (settings.contains("size") && settings.contains("pos")) {
        resize(settings.value("size").toSize());
        move(settings.value("pos").toPoint());
    }
    settings.endGroup();

    QMainWindow::showEvent(event);
    setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
    event->accept();
}

void PlayerWindow::selectNewRadioStation(QListWidgetItem *item) {
    PlayList::getInstance()->setCurrentRadioStation(item->data(Qt::UserRole).toInt());
}

void PlayerWindow::onRadioStationChanged(const RadioStation &rs) {
    ui->currentStationLabel->setText(rs.name);
    QNetworkReply *reply = NetworkManager::getInstance()->getFile(QUrl(rs.iconURL));
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QPixmap p;
    p.loadFromData(reply->readAll());
    ui->currentStationIconLabel->setPixmap(
            p.scaled(48, 48, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)
    );
}

void PlayerWindow::onRadioPlayerStateChanged(QMediaPlayer::State state) {
    if (state == QMediaPlayer::State::PlayingState) {
        ui->playerControlButton->setText(tr("Stop"));
        ui->playerControlButton->setIcon(QIcon(":/img/stop"));
    } else {
        ui->playerControlButton->setText(tr("Play"));
        ui->playerControlButton->setIcon(QIcon(":/img/play"));
    }
}

void PlayerWindow::showSettingsDialog() {
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}
