#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>
#include <QSettings>
#include <defs.h>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT

public:
    explicit Preferences(QSettings *settings, QWidget *parent = nullptr);
    ~Preferences();

private slots:
    void on_cancelPushButton_clicked();

    void on_closeButton_clicked();

    void on_applyButton_clicked();

private:
    Ui::Preferences *ui;
    QSettings *settings;

    void saveSettings();
};

#endif // PREFERENCES_H
