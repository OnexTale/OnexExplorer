#ifndef ONEXNSTGDATA_H
#define ONEXNSTGDATA_H

#include "../../Openers/NosZlibOpener.h"
#include "OnexTreeZlibItem.h"
#include "../../Converters/NosModelConverter.h"
#include "../../Converters/ObjConverter.h"
#include <QVector2D>
#include <QVector3D>

class OnexNStgData : public OnexTreeZlibItem {
Q_OBJECT
public:
    OnexNStgData(QByteArray header, QString name, QByteArray content, NosZlibOpener *opener, int id,
                 int creationDate, bool compressed);
    ~OnexNStgData() override;
    QWidget *getPreview() override;
    FileInfo *getInfos() override;
    QByteArray getContent() override;
    int saveAsFile(const QString &path, QByteArray content = QByteArray()) override;
    QString getExportExtension() override;

private slots:
    int afterReplace(QByteArray content) override;
    void setXPosition(int index, float x, bool update = false);
    void setYPosition(int index, float y, bool update = false);
    void setZPosition(int index, float z, bool update = false);
    void setXRotation(int index, float x, bool update = false);
    void setYRotation(int index, float y, bool update = false);
    void setZRotation(int index, float z, bool update = false);
    void setWRotation(int index, float w, bool update = false);
    void setXScale(int index, float x, bool update = false);
    void setYScale(int index, float y, bool update = false);
    void setZScale(int index, float z, bool update = false);
    void setTexture(int index, int texture, bool update = false);
    void setUVScale(float scale, bool update = false);
signals:
    void replaceSignal(Model *newModel);
protected:
    static ObjConverter objConverter;
    static NosModelConverter nosModelConverter;
    Model *model;
    FileInfo *generateInfos() override;
};

#endif // ONEXNSTGDATA_H
