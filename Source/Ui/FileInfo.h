#ifndef FILEINFO_H
#define FILEINFO_H

#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

class FileInfo : public QWidget {
    Q_OBJECT
private:
    int rows;

public:
    QGridLayout *grid;
    FileInfo(QWidget *parent = 0);
    QLineEdit *addStringLineEdit(QString title, QString value);
    QLineEdit *addIntLineEdit(QString title, int value);
    QLineEdit *addFloatLineEdit(QString title, float value);
    QCheckBox *addCheckBox(QString title, bool checked);

public slots:
    void update(QString title, QString value);
    void update(QString title, int value);
    void update(QString title, float value);
    void update(QString title, bool value);
    void replace(FileInfo *newInfo);

signals:
    void updateInfo(FileInfo *info);

};
#endif // FILEINFO_H
