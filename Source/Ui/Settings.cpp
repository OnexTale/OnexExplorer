#include "Settings.h"
#include "ui_settings.h"
#include <QDebug>
#include <QtWidgets/QFileDialog>
#include <QSettings>

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);
    settingsPath = QApplication::applicationDirPath().left(1) + ":/Settings.ini";
    load();
}

Settings::~Settings() {
    delete ui;
}

QString Settings::getExportPath() const {
    return ui->label_export->text() + "/";
}

QString Settings::getReplacePath() const {
    return ui->label_replace->text() + "/";
}

QString Settings::getOpenPath() const {
    return ui->label_open->text() + "/";
}

QString Settings::getSavePath() const {
    return ui->label_save->text() + "/";
}

void Settings::on_btn_export_clicked() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"));
    if (dir.isEmpty())
        return;
    ui->label_export->setText(dir);
    if (ui->label_replace->text().isEmpty())
        ui->label_replace->setText(dir);
}

void Settings::on_btn_replace_clicked() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"));
    if (dir.isEmpty())
        return;
    ui->label_replace->setText(dir);
    if (ui->label_export->text().isEmpty())
        ui->label_export->setText(dir);
}

void Settings::on_btn_open_clicked() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"));
    if (dir.isEmpty())
        return;
    ui->label_open->setText(dir);
    if (ui->label_save->text().isEmpty())
        ui->label_save->setText(dir);
}

void Settings::on_btn_save_clicked() {
    QString dir = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"));
    if (dir.isEmpty())
        return;
    ui->label_save->setText(dir);
    if (ui->label_open->text().isEmpty())
        ui->label_open->setText(dir);
}

void Settings::on_btn_reset_clicked() {
    ui->label_export->clear();
    ui->label_replace->clear();
    ui->label_open->clear();
    ui->label_save->clear();
}

void Settings::accept() {
    save();
    this->close();
}

void Settings::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    load();
}

void Settings::save() {
    QSettings settingsFile(settingsPath, QSettings::IniFormat);
    settingsFile.setValue("ExportPath", ui->label_export->text());
    settingsFile.setValue("ReplacePath", ui->label_replace->text());
    settingsFile.setValue("OpenPath", ui->label_open->text());
    settingsFile.setValue("SavePath", ui->label_save->text());
}

void Settings::load() {
    QSettings settingsFile(settingsPath, QSettings::IniFormat);
    ui->label_export->setText(settingsFile.value("ExportPath", "").toString());
    ui->label_replace->setText(settingsFile.value("ReplacePath", "").toString());
    ui->label_open->setText(settingsFile.value("OpenPath", "").toString());
    ui->label_save->setText(settingsFile.value("SavePath", "").toString());
}