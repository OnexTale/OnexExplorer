#ifndef SINGLETEXTFILEPREVIEW_H
#define SINGLETEXTFILEPREVIEW_H

#include <QWidget>
#include <iostream>
#include "OnexTreeText.h"

namespace Ui {
class SingleTextFilePreview;
}

class SingleTextFilePreview : public QWidget
{
    Q_OBJECT

public:
    explicit SingleTextFilePreview(std::shared_ptr<OnexTreeItem> item, QWidget *parent = 0);
    ~SingleTextFilePreview();

private:
    Ui::SingleTextFilePreview *ui;
};

#endif // SINGLETEXTFILEPREVIEW_H
