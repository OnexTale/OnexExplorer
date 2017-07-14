#include "OnexTreeImage.h"

OnexTreeImage::OnexTreeImage(QString name, QByteArray content) : OnexTreeItem(name, content)
{

}

QWidget *OnexTreeImage::onClicked()
{
    return nullptr;
}
