#include "axis.h"

Axis::Axis(QOpenGLFunctions* function) : f{function}
{}

void Axis::init (QString vertexShaderPath,
                 QString fragmentShaderPath)
{
    initShader(vertexShaderPath, fragmentShaderPath);
    initObject();
}

void Axis::paint (const QMatrix4x4& mvpMatrix,
                  const QMatrix4x4&,
                  const QMatrix3x3&)
{
    vao.bind();
    shader.bind();
    shader.setUniformValue("mvp", mvpMatrix);
    f->glDrawArrays(GL_LINES, 0, vertices.length());
    shader.release();
    vao.release();
}


void Axis::initObject   ()
{
    vertices.push_back(QVector3D{ 0, 0, 0});
    vertices.push_back(QVector3D{ 1, 0, 0});
    vertices.push_back(QVector3D{10, 0, 0});
    vertices.push_back(QVector3D{ 1, 0, 0});
    vertices.push_back(QVector3D{ 0, 0, 0});
    vertices.push_back(QVector3D{ 0, 1, 0});
    vertices.push_back(QVector3D{ 0,10, 0});
    vertices.push_back(QVector3D{ 0, 1, 0});
    vertices.push_back(QVector3D{ 0, 0, 0});
    vertices.push_back(QVector3D{ 0, 0, 1});
    vertices.push_back(QVector3D{ 0, 0,10});
    vertices.push_back(QVector3D{ 0, 0, 1});

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices.data(), vertices.length()*sizeof(QVector3D));

    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));

    vbo.release();
    vao.release();
}
void Axis::initShader   (QString vertexShaderPath, \
                         QString fragmentShaderPath)
{
    shader.create();
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,  vertexShaderPath);
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,fragmentShaderPath);
    shader.bindAttributeLocation("position",   0);
    shader.bindAttributeLocation("color",      1);
}
