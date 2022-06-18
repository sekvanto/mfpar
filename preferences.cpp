#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences),
    settings(settings)
{
    ui->setupUi(this);

    if (settings->value(ALWAYS_MANUALLY_ALGS).toBool() == true) {
        this->ui->chooseAlgsManuallyCheckBox->setChecked(true);
    }
    if (settings->value(LIGHT_THEME_DEFAULT).toBool() == false) {
        this->ui->useLightThemeCheckBox->setChecked(false);
    }
    this->ui->historyEntriesNumSpinBox->setValue(settings->value(HISTORY_ENTRIES_NUM).toInt());
    this->ui->savingFilesDirLineEdit->setText(settings->value(PREFER_DIR_SAVING).toString());
    this->ui->preferredCompressionAlgComboBox->setCurrentIndex(settings->value(PREFER_ALG).toInt());
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::saveSettings()
{
    settings->setValue(ALWAYS_MANUALLY_ALGS, this->ui->chooseAlgsManuallyCheckBox->isChecked());
    settings->setValue(LIGHT_THEME_DEFAULT, this->ui->useLightThemeCheckBox->isChecked());
    settings->setValue(HISTORY_ENTRIES_NUM, this->ui->historyEntriesNumSpinBox->value());
    settings->setValue(PREFER_DIR_SAVING, this->ui->savingFilesDirLineEdit->text());
    settings->setValue(PREFER_ALG, this->ui->preferredCompressionAlgComboBox->currentIndex());
    settings->sync();
}

void Preferences::on_cancelPushButton_clicked()
{
    this->close();
}


void Preferences::on_closeButton_clicked()
{
    saveSettings();
    this->close();
}


void Preferences::on_applyButton_clicked()
{
    saveSettings();
}

