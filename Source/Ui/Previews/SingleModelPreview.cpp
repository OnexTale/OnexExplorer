#include "SingleModelPreview.h"
#include <QLayout>
#include <QLineEdit>

SingleModelPreview::SingleModelPreview(QVector<QVector3D> vertices, QVector<QVector3D> faces, QVector<QVector3D> normals, QWidget *parent)
    : QWidget(parent) {
    this->vertices = vertices;
    this->faces = faces;
    this->normals = normals;
    QLayout *l = new QGridLayout();
    setLayout(l);
    Qt3DExtras::Qt3DWindow *view = getView();
    l->addWidget(QWidget::createWindowContainer(view));
}

void SingleModelPreview::onReplaced() {
}

SingleModelPreview::~SingleModelPreview() {
}

// QVector<QVector3D> SingleModelPreview::calculateNormals(QVector<QVector3D> vertices, QVector<QVector3D> faces) {
//     QVector<QVector3D> normals;
//     for (int i = 0; i < vertices.size(); i++)
//         normals.append(QVector3D(0, 0, 0));

//     for (int i = 0; i < faces.size(); i++) {
//         QVector3D fn =
//             QVector3D::normal(vertices.at(faces.at(i).x()), vertices.at(faces.at(i).y()), vertices.at(faces.at(i).z()));
//         normals[(int)faces.at(i).x()] += fn;
//         normals[(int)faces.at(i).y()] += fn;
//         normals[(int)faces.at(i).z()] += fn;
//     }

//     for (int i = 0; i < normals.size(); i++) {
//         normals[i].normalize();
//     }
//     return normals;
// }

Qt3DRender::QGeometryRenderer *SingleModelPreview::getGeometry() {
    Qt3DRender::QGeometryRenderer *customMeshRenderer = new Qt3DRender::QGeometryRenderer;
    Qt3DRender::QGeometry *customGeometry = new Qt3DRender::QGeometry(customMeshRenderer);
    Qt3DRender::QBuffer *vertexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, customGeometry);
    Qt3DRender::QBuffer *indexDataBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer, customGeometry);

    QByteArray vertexBufferData;
    vertexBufferData.resize(this->vertices.size() * 6 * sizeof(float));
    float *rawVertexArray = reinterpret_cast<float *>(vertexBufferData.data());
    int idx = 0;
    for (int i = 0; i < this->vertices.size(); i++) {
        rawVertexArray[idx++] = this->vertices.at(i).x();
        rawVertexArray[idx++] = this->vertices.at(i).y();
        rawVertexArray[idx++] = this->vertices.at(i).z();

        rawVertexArray[idx++] = this->normals.at(i).x();
        rawVertexArray[idx++] = this->normals.at(i).y();
        rawVertexArray[idx++] = this->normals.at(i).z();
    }

    QByteArray indexBufferData;
    indexBufferData.resize(this->faces.size() * 3 * sizeof(short));
    short *rawFaceArray = reinterpret_cast<short *>(indexBufferData.data());
    idx = 0;
    for (const QVector3D f : this->faces) {
        rawFaceArray[idx++] = f.x();
        rawFaceArray[idx++] = f.y();
        rawFaceArray[idx++] = f.z();
    }

    vertexDataBuffer->setData(vertexBufferData);
    indexDataBuffer->setData(indexBufferData);

    // Attributes
    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexDataBuffer);
    positionAttribute->setDataType(Qt3DRender::QAttribute::Float);
    positionAttribute->setDataSize(3);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(6 * sizeof(float));
    positionAttribute->setCount(this->vertices.size());
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    Qt3DRender::QAttribute *normalAttribute = new Qt3DRender::QAttribute();
    normalAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    normalAttribute->setBuffer(vertexDataBuffer);
    normalAttribute->setDataType(Qt3DRender::QAttribute::Float);
    normalAttribute->setDataSize(3);
    normalAttribute->setByteOffset(3 * sizeof(float));
    normalAttribute->setByteStride(6 * sizeof(float));
    normalAttribute->setCount(this->normals.size());
    normalAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexDataBuffer);
    indexAttribute->setDataType(Qt3DRender::QAttribute::UnsignedShort);
    indexAttribute->setDataSize(1);
    indexAttribute->setByteOffset(0);
    indexAttribute->setByteStride(0);
    indexAttribute->setCount(this->faces.size());

    customGeometry->addAttribute(positionAttribute);
    customGeometry->addAttribute(normalAttribute);
    customGeometry->addAttribute(indexAttribute);

    customMeshRenderer->setInstanceCount(1);
    customMeshRenderer->setFirstVertex(0);
    customMeshRenderer->setFirstInstance(0);
    customMeshRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Triangles);
    customMeshRenderer->setGeometry(customGeometry);
    customMeshRenderer->setVertexCount(this->faces.size() * 3);

    return customMeshRenderer;
}

Qt3DCore::QEntity *SingleModelPreview::createScene() {
    Qt3DCore::QEntity *root = new Qt3DCore::QEntity;
    Qt3DCore::QEntity *torus = new Qt3DCore::QEntity(root);

    Qt3DRender::QGeometryRenderer *mesh = getGeometry();
    Qt3DCore::QTransform *transform = new Qt3DCore::QTransform;
    transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1, 0, 0), 25.f));
    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(root);

    torus->addComponent(mesh);
    torus->addComponent(transform);
    torus->addComponent(material);

    return root;
}

Qt3DExtras::Qt3DWindow *SingleModelPreview::getView() {
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    Qt3DCore::QEntity *scene = createScene();
    // camera
    Qt3DRender::QCamera *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 80.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));
    // manipulator
    Qt3DExtras::QOrbitCameraController *manipulator = new Qt3DExtras::QOrbitCameraController(scene);
    manipulator->setLinearSpeed(50.f);
    manipulator->setLookSpeed(180.f);
    manipulator->setCamera(camera);
    // light
    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(scene);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);
    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(camera->position());
    lightEntity->addComponent(lightTransform);

    view->setRootEntity(scene);
    return view;
}
