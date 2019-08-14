#include "SingleModelPreview.h"
#include <QLayout>
#include <QLineEdit>
#include <QQuaternion>

SingleModelPreview::SingleModelPreview(Model *model, QWidget *parent) : QOpenGLWidget(parent) {
    this->model = model;
    camera.X = 0;
    camera.Y = 0;
    camera.Z = -20;
    camera.angleX = 20;
    camera.angleY = 0;
    mouse.X = 0;
    mouse.Y = 0;
    setMinimumHeight(600);
    setMaximumHeight(600);
    setMinimumWidth(650);
    setMaximumWidth(650);
}

SingleModelPreview::~SingleModelPreview() = default;

void SingleModelPreview::onReplaced(Model *model) {
    this->model = model;
    repaint();
}

void SingleModelPreview::initializeGL() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(65, (float) width() / (float) height(), 0.1, 300);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_LIGHTING);
}

void SingleModelPreview::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    // Camera
    glTranslatef(camera.X, camera.Y, camera.Z);
    glRotatef(camera.angleY, 0, 1, 0);
    glRotatef(camera.angleX, 1, 0, 0);

    // glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    // glColor3f(0.5f, 0.5f, 0.5f);

    for (auto &object : model->objects) {
        glPushMatrix();
        QQuaternion qr(object.rotation);
        float xAxis, yAxis, zAxis, angle;
        qr.getAxisAndAngle(&xAxis, &yAxis, &zAxis, &angle);
        glTranslatef(object.position.x(), object.position.y(), object.position.z());
        glRotatef(angle, xAxis, yAxis, zAxis);
        glScalef(object.scale.x(), object.scale.y(), object.scale.z());
        for (int group : object.groups) {
            for (auto &face : model->groups[group].faces) {
                glBegin(GL_TRIANGLES);
                glPushMatrix();
                int p1 = face.x();
                int p2 = face.y();
                int p3 = face.z();
                glNormal3f(model->normals[p1].x(), model->normals[p1].y(), model->normals[p1].z());
                glVertex3f(model->vertices[p1].x(), model->vertices[p1].y(), model->vertices[p1].z());
                glNormal3f(model->normals[p2].x(), model->normals[p2].y(), model->normals[p2].z());
                glVertex3f(model->vertices[p2].x(), model->vertices[p2].y(), model->vertices[p2].z());
                glNormal3f(model->normals[p3].x(), model->normals[p3].y(), model->normals[p3].z());
                glVertex3f(model->vertices[p3].x(), model->vertices[p3].y(), model->vertices[p3].z());
                glPopMatrix();
                glEnd();
            }
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void SingleModelPreview::mousePressEvent(QMouseEvent *event) {
    mouse.X = event->x();
    mouse.Y = event->y();
}

void SingleModelPreview::mouseMoveEvent(QMouseEvent *event) {
    int x = event->x();
    int y = event->y();
    Qt::MouseButtons buttons = event->buttons();
    if (buttons & Qt::LeftButton) {
        if (y < mouse.Y) {
            camera.angleX -= 1;
        } else if (y > mouse.Y) {
            camera.angleX += 1;
        }
        if (x < mouse.X) {
            camera.angleY -= 2;
        } else if (x > mouse.X) {
            camera.angleY += 2;
        }
    } else if (buttons & Qt::RightButton) {
        camera.X += (x - mouse.X) / 5;
        camera.Y -= (y - mouse.Y) / 5;
    }
    mouse.X = x;
    mouse.Y = y;
    repaint();
}

void SingleModelPreview::wheelEvent(QWheelEvent *event) {
    camera.Z += event->delta() / 120;
    repaint();
}

