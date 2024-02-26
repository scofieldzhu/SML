#include "ply_reader.h"
#include <QFile>
#include <QDebug>

bool ply_reader::LoadFile(const QString& filename, QVector<glm::vec3>& vertices)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Failed to open file:" << filename << endl;
        return false;
    }
    QTextStream in(&file);
    QString line;
    bool end_header = false;
    int vertex_count = 0;
    int face_count = 0;
    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith("element vertex")) {
            vertex_count = line.split(" ")[2].toInt();
        } else if (line.startsWith("element face")) {
            face_count = line.split(" ")[2].toInt();
        } else if (line.startsWith("end_header")) {
            end_header = true;
            break;
        }
    }
    if (!end_header || vertex_count == 0) {
        qDebug() << "Invalid or unsupported PLY file:" << filename << endl;
        return false;
    }
    vertices.clear();
    for (int i = 0; i < vertex_count; ++i) {
        if (in.atEnd()) {
            qDebug() << "Unexpected end of file while reading vertices";
            return false;
        }
        line = in.readLine();
        QStringList coords = line.split(" ");
        if (coords.size() >= 3) {
            float x = coords[0].toFloat();
            float y = coords[1].toFloat();
            float z = coords[2].toFloat();
            vertices.append({x, y, z});
        }
    }
    QVector<QVector<int>> faces;
    for (int i = 0; i < face_count; ++i) {
        if (in.atEnd()) {
            qDebug() << "Unexpected end of file while reading faces";
            return false;
        }
        line = in.readLine();
        QStringList indices = line.split(" ");
        int numIndices = indices[0].toInt();
        QVector<int> face;
        for (int j = 1; j <= numIndices; ++j) {
            face.append(indices[j].toInt());
        }
        faces.append(face);
    }
    return true;
}