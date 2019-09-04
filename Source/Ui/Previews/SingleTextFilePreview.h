#ifndef SINGLETEXTFILEPREVIEW_H
#define SINGLETEXTFILEPREVIEW_H

#include "../TreeItems/OnexTreeText.h"
#include <QTextCodec>
#include <QWidget>
#include <iostream>

namespace Ui {
    class SingleTextFilePreview;
}
class SingleTextFilePreview : public QWidget {
Q_OBJECT
public:
    explicit SingleTextFilePreview(QByteArray &item, const QString &encoding = "Windows-1250", QWidget *parent = nullptr);
    ~SingleTextFilePreview() override;
private slots:
    void onReplaced(const QByteArray &text);
private:
    Ui::SingleTextFilePreview *ui;
    QTextCodec *codec;
};

#endif // SINGLETEXTFILEPREVIEW_H
