#ifndef SINGLEIMAGEPREVIEW_H
#define SINGLEIMAGEPREVIEW_H

#include <QWidget>

namespace Ui {
class SingleImagePreview;
}

class SingleImagePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SingleImagePreview(QWidget *parent = 0);
    ~SingleImagePreview();

private:
    Ui::SingleImagePreview *ui;
};

#endif // SINGLEIMAGEPREVIEW_H
