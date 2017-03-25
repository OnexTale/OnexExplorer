#ifndef SINGLETEXTFILEPREVIEW_H
#define SINGLETEXTFILEPREVIEW_H

#include <QWidget>

namespace Ui {
class SingleTextFilePreview;
}

class SingleTextFilePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SingleTextFilePreview(QByteArray content, QWidget *parent = 0);
    ~SingleTextFilePreview();

private:
    Ui::SingleTextFilePreview *ui;
};

#endif // SINGLETEXTFILEPREVIEW_H
