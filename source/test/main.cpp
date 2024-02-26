
#include <QApplication>
#include <QMainWindow>
#include <QSurfaceFormat>
#include "render_window.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
#ifdef __APPLE__
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setVersion(3, 1);
#endif
    format.setDepthBufferSize(16);

    std::unique_ptr<RenderWindow> glwindow(new RenderWindow(app, format));

    QMainWindow window;
    window.setMinimumSize(640, 480);
    window.setWindowTitle("globjects and Qt");
    window.setCentralWidget(QWidget::createWindowContainer(glwindow.release()));

    window.show();

    return app.exec();
}

