#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
    class Settings;
}

class Settings : public QDialog {
Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings() override;
    QString getExportPath() const;
    QString getReplacePath() const;
    QString getOpenPath() const;
    QString getSavePath() const;

private slots:
    void on_btn_export_clicked();
    void on_btn_replace_clicked();
    void on_btn_open_clicked();
    void on_btn_save_clicked();
    void on_btn_reset_clicked();

protected:
    void accept() override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::Settings *ui;
    QString settingsPath;
    void save();
    void load();
};

#endif // SETTINGS_H
