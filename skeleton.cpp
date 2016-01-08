#include "skeleton.h"

skeleton::skeleton(QOpenGLFunctions* function) : f{function}
{}

void skeleton::init (QString vertexShaderPath,
                     QString fragemntShaderPath)
{
    initShader(vertexShaderPath, fragemntShaderPath);
    initObject();
}

void skeleton::paint(const QMatrix4x4 &mvpMatrix,
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


void skeleton::initObject   ()
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
void skeleton::initShader   (QString vertexShaderPath,
                             QString fragemntShaderPath)
{
    shader.create();
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,   vertexShaderPath);
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, fragemntShaderPath);
    shader.bindAttributeLocation("position", 0);
    shader.link();
}
