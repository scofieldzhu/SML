
#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include <QTimer>
#include <spdlog/spdlog.h>
#include "render_window.h"
#include "ply_reader.h"
#include "mesh_cloud.h"

std::unique_ptr<RenderWindow> gRenderWindow;

void LoadMeshData()
{
    QString data_file = QCoreApplication::applicationDirPath() + "/Axle shaft.ply";
    MeshCloudSPtr mesh_cloud = std::make_shared<MeshCloud>();
    ply_reader::LoadFile(data_file, mesh_cloud->vertex_list);
    gRenderWindow->loadMeshCloud(mesh_cloud);
}

int main(int argc, char * argv[])
{
    spdlog::set_level(spdlog::level::trace);

    QApplication app(argc, argv);

    QSurfaceFormat format;
#ifdef __APPLE__
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setVersion(3, 1);
#endif
    format.setDepthBufferSize(16);

    gRenderWindow = std::make_unique<RenderWindow>(app, format);

    QMainWindow window;
    window.setMinimumSize(640, 480);
    window.setWindowTitle("globjects and Qt");
    window.setCentralWidget(QWidget::createWindowContainer(gRenderWindow.get()));

    QTimer::singleShot(2000, LoadMeshData);

    window.show();

    return app.exec();
}

