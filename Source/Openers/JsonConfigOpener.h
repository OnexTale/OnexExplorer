#ifndef JSONCONFIGOPENER_H
#define JSONCONFIGOPENER_H


#include <QtCore/QFile>
#include <Source/Ui/OnexTreeItem.h>
#include "NosZlibOpener.h"
#include "NosTextOpener.h"

class JsonConfigOpener {
public:
    OnexTreeItem *load(QFile &file, const QString &directory);
    void load(OnexTreeItem *root, QFile &file, const QString &directory);
private:
    NosTextOpener textOpener;
    NosZlibOpener zlibOpener;
    OnexTreeItem *generateRoot(int headerNumber, const QJsonObject &jo);
    OnexTreeItem *generateItem(int headerNumber, const QJsonObject &jo, const QString &directory);
    int getNTHeaderNumber(const QString &header);
    int findChildByName(OnexTreeItem *item, const QString &searched);
};


#endif //JSONCONFIGOPENER_H
