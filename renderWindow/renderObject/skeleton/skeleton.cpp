//#include "skeleton.h"
#include "../skeleton/skeleton.h"

Skeleton::Skeleton(QOpenGLFunctions* function) : f{function}
{}

void Skeleton::init (QString vertexShaderPath,
                     QString fragemntShaderPath)
{
    initShader(vertexShaderPath, fragemntShaderPath);
    initObject();
}

void Skeleton::paint(const QMatrix4x4 &mvpMatrix,
                     const QMatrix4x4 &,
                     const QMatrix3x3 &)
{
    vao.bind();
    shader.bind();

    shader.setUniformValue("mvp", mvpMatrix);

    f->glDrawArrays(GL_POINTS, 0, vertices.size());

    shader.release();
    vao.release();
}

void Skeleton::addVertices(std::vector<QVector3D> &nextVertices)
{
    std::move(nextVertices.begin(), nextVertices.end(), std::back_inserter(vertices));
    vbo.bind();
    f->glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(QVector3D), vertices.data(), GL_STATIC_DRAW);
    vbo.release();
}
void Skeleton::clear()
{
    vertices.clear();
}
const QVector<QVector3D>& Skeleton::getVertices()
{
    return vertices;
}

void Skeleton::initObject   ()
{
    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), 0);

    vbo.release();
    vao.release();
}
void Skeleton::initShader   (QString vertexShaderPath,
                             QString fragemntShaderPath)
{
    shader.create();
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,   vertexShaderPath);
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragemntShaderPath);
    shader.bindAttributeLocation("position", 0);
    shader.link();
}
