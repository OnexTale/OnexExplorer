#ifndef SINGLEMODELPREVIEW_H
#define SINGLEMODELPREVIEW_H

#include <QVector3D>
#include <QWidget>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPerVertexColorMaterial>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DInput/QInputAspect>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QRenderAspect>

class SingleModelPreview : public QWidget {
    Q_OBJECT

public:
    explicit SingleModelPreview(QVector<QVector3D> vertices, QVector<QVector3D> faces, QVector<QVector3D> normals,
                                QWidget *parent = 0);
    ~SingleModelPreview();

private:
    QVector<QVector3D> vertices;
    QVector<QVector3D> faces;
    QVector<QVector3D> normals;
    Qt3DRender::QGeometryRenderer *getGeometry();
    Qt3DCore::QEntity *createScene();
    Qt3DExtras::Qt3DWindow *getView();

private slots:
    void onReplaced();
};

#endif // SINGLEMODELPREVIEW_H
