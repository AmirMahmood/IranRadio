#include <QSettings>

#include "SystemTry.h"
#include "SettingsDialog.h"
#include "./ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    lanBtGroup = new QButtonGroup(this);
    tryClickBtGroup = new QButtonGroup(this);

    lanBtGroup->addButton(ui->faRadioButton, SystemTry::Langs::Fa);
    lanBtGroup->addButton(ui->enRadioButton, SystemTry::Langs::En);

    tryClickBtGroup->addButton(ui->showPlayerWinRadioButton, SystemTry::TryClickAction::ShowPlayerWin);
    tryClickBtGroup->addButton(ui->stopPlayRadioButton, SystemTry::TryClickAction::PlayStop);

    QSettings settings;
    settings.beginGroup("settings");

    if (settings.value("__current_lang", SystemTry::Langs::En) == SystemTry::Langs::Fa){
        setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    }

    if (settings.value("lang", SystemTry::Langs::En) == SystemTry::Langs::En){
        ui->enRadioButton->setChecked(true);
    } else {
        ui->faRadioButton->setChecked(true);
    }
    if (settings.value("try-clicked", SystemTry::TryClickAction::ShowPlayerWin) == SystemTry::TryClickAction::ShowPlayerWin){
        ui->showPlayerWinRadioButton->setChecked(true);
    } else {
        ui->stopPlayRadioButton->setChecked(true);
    }
    ui->exitToTryCheckBox->setChecked(settings.value("exit-to-try", true).toBool());
    ui->minimizeToTryCheckBox->setChecked(settings.value("minimize-to-try", true).toBool());
    settings.endGroup();

    connect(ui->exitToTryCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onExitToTryCheckBoxChanged);
    connect(ui->minimizeToTryCheckBox, &QCheckBox::stateChanged, this, &SettingsDialog::onMinimizeToTryCheckBoxChanged);
    connect(tryClickBtGroup, &QButtonGroup::idClicked, this, &SettingsDialog::onTryClickBtGroupClicked);
    connect(lanBtGroup, &QButtonGroup::idClicked, this, &SettingsDialog::onLanBtGroupClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::onExitToTryCheckBoxChanged(int state){
    QSettings settings;
    settings.beginGroup("settings");

    if(state == Qt::CheckState::Checked){
        settings.setValue("exit-to-try", true);
    } else {
        settings.setValue("exit-to-try", false);
    }

    settings.endGroup();
}

void SettingsDialog::onMinimizeToTryCheckBoxChanged(int state){
    QSettings settings;
    settings.beginGroup("settings");

    if(state == Qt::CheckState::Checked){
        settings.setValue("minimize-to-try", true);
    } else {
        settings.setValue("minimize-to-try", false);
    }

    settings.endGroup();
}

void SettingsDialog::onTryClickBtGroupClicked(int id){
    QSettings settings;
    settings.beginGroup("settings");
    settings.setValue("try-clicked", id);
    settings.endGroup();
}

void SettingsDialog::onLanBtGroupClicked(int id){
    QSettings settings;
    settings.beginGroup("settings");
    settings.setValue("lang", id);
    settings.endGroup();
}
