#include "SingleModelPreview.h"
#include <QLayout>
#include <QLineEdit>

SingleModelPreview::SingleModelPreview(Model *model, QWidget *parent)
    : QOpenGLWidget(parent) {
    this->model = model;

    camera.X = 0;
    camera.Y = 0;
    camera.Z = -20;
    camera.angleX = 20;
    camera.angleY = 0;

    mouse.X = 0;
    mouse.Y = 0;
}

void SingleModelPreview::initializeGL() {
    glClearColor(1, 1, 1, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60, (float)width() / (float)height(), 0, 300);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_LIGHTING);
}

void SingleModelPreview::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera
    glTranslatef(camera.X, camera.Y, camera.Z);
    glRotatef(camera.angleY, 0, 1, 0);
    glRotatef(camera.angleX, 1, 0, 0);

    glPushMatrix();

    // glEnable(GL_LIGHT0);
    // glEnable(GL_COLOR_MATERIAL);
    // glColor3f(0.5f, 0.5f, 0.5f);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < model->objects.size(); i++) {
        for (int j = 0; j < model->objects[i].groups.size(); j++) {
            for (int g = 0; g < model->groups[model->objects[i].groups[j]].faces.size(); g++) {
                int p1 = model->groups[model->objects[i].groups[j]].faces[g].x();
                int p2 = model->groups[model->objects[i].groups[j]].faces[g].y();
                int p3 = model->groups[model->objects[i].groups[j]].faces[g].z();
                glNormal3f(model->normals[p1].x(), model->normals[p1].y(), model->normals[p1].z());
                glVertex3f(model->vertices[p1].x(), model->vertices[p1].y(), model->vertices[p1].z());

                glNormal3f(model->normals[p2].x(), model->normals[p2].y(), model->normals[p2].z());
                glVertex3f(model->vertices[p2].x(), model->vertices[p2].y(), model->vertices[p2].z());

                glNormal3f(model->normals[p3].x(), model->normals[p3].y(), model->normals[p3].z());
                glVertex3f(model->vertices[p3].x(), model->vertices[p3].y(), model->vertices[p3].z());
            }
        }
    }

    glEnd();
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

void SingleModelPreview::onReplaced(Model *model) {
    this->model = model;
    repaint();
}

SingleModelPreview::~SingleModelPreview() {
}