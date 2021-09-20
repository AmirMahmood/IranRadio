#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QButtonGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsDialog; }
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

    ~SettingsDialog();

private:
    void onExitToTryCheckBoxChanged(int state);
    void onMinimizeToTryCheckBoxChanged(int state);

    void onTryClickBtGroupClicked(int id);
    void onLanBtGroupClicked(int id);

    Ui::SettingsDialog *ui;

    QButtonGroup *lanBtGroup;
    QButtonGroup *tryClickBtGroup;
};

#endif // SETTINGSDIALOG_H
