#ifndef FILEINFO_H
#define FILEINFO_H

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QJsonObject>
#include <QJsonArray>

class FileInfo : public QWidget {
Q_OBJECT
public:
    QGridLayout *grid;
    explicit FileInfo(QWidget *parent = nullptr);
    QLineEdit *addStringLineEdit(const QString &title, const QString &value);
    QLineEdit *addIntLineEdit(const QString &title, int value);
    QLineEdit *addFloatLineEdit(const QString &title, float value);
    QCheckBox *addCheckBox(const QString &title, bool checked);
    QComboBox *addSelection(const QString &title, const QStringList &items, int value);
    QPushButton *addReplaceButton(const QString &title);
    QPushButton *addReplaceRawButton(const QString &title);
    QJsonObject toJson();
public slots:
    void update(const QString &title, const QString &value);
    void update(const QString &title, int value);
    void update(const QString &title, float value);
    void update(const QString &title, bool value);
    void replace(FileInfo *newInfo);
signals:
    void updateInfo(FileInfo *info);
    void replaceFile();
    void replaceRawFile();
private:
    int rows;
    QLineEdit *addLineEdit(const QString &title, const QString &value);
};

#endif // FILEINFO_H
