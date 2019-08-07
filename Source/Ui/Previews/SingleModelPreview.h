#ifndef SINGLEMODELPREVIEW_H
#define SINGLEMODELPREVIEW_H

#include "../../Converters/IModelConverter.h"
#include <QMouseEvent>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QWheelEvent>
#include <glut.h>

struct Mouse {
    int X;
    int Y;
};

struct Camera {
    float X;
    float Y;
    float Z;
    float angleX;
    float angleY;
};

class SingleModelPreview : public QOpenGLWidget {
    Q_OBJECT
public:
    SingleModelPreview(Model *model, QWidget *parent = 0);
    ~SingleModelPreview();

private:
    Model *model;
    Camera camera;
    Mouse mouse;
    void initializeGL();
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

private slots:
    void onReplaced(Model *model);
};

#endif // SINGLEMODELPREVIEW_H
