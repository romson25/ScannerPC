#include "model.h"

Model::Model(QOpenGLFunctions* function) : f{function}
{}

void Model::init    (QString vertexShaderPath,
                     QString fragmentShaderPath)
{
    initShader(vertexShaderPath, fragmentShaderPath);
    initObject();
}

void Model::paint   (const QMatrix4x4& mvpMatrix,
                     const QMatrix4x4& modelViewMatrix,
                     const QMatrix3x3& normalMatrix)
{
    vao.bind();
    vbo.bind();
    shader.bind();

    shader.setUniformValue("modelViewMatrix", modelViewMatrix);
    shader.setUniformValue("mvp",             mvpMatrix);
    shader.setUniformValue("normalMatrix",    normalMatrix);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), 0);
    f->glEnableVertexAttribArray(0);

    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), reinterpret_cast<void *>(3*sizeof(GL_FLOAT)));
    f->glEnableVertexAttribArray(1);

    ibo->bind();
    f->glDrawElements(GL_TRIANGLES, indices.length(), GL_UNSIGNED_INT, 0);

    ibo->release();
    shader.release();
    vbo.release();
    vao.release();
}

void Model::clear   ()
{
    data.clear();
    indices.clear();
}

void Model::addData     (const QVector<QVector3D> &nextData,
                         const QVector<unsigned int> &nextIndices)
{
    data = std::move(nextData);
    vbo.bind();
    f->glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(QVector3D), data.data(), GL_STATIC_DRAW);
    vbo.release();

    indices = std::move(nextIndices);
    ibo->bind();
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.length()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    ibo->release();
}

void Model::initObject  ()
{
    vao.create();
    vao.bind();

    vbo.create();
    ibo->create();

    vao.release();
}
void Model::initShader  (QString vertexShaderPath,
                         QString fragmentShaderPath)
{
    shader.create();
    shader.addShaderFromSourceFile(QOpenGLShader::Vertex,    vertexShaderPath);
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment,  fragmentShaderPath);
    shader.bindAttributeLocation("position", 0);
    shader.bindAttributeLocation("normal",   1);
    shader.link();
    shader.bind();
}
