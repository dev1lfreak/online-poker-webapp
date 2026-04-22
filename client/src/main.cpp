#include <QApplication>

#include "ClientController.hpp"
#include "ui/ConnectionWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    client::ClientController controller;
    client::ConnectionWindow window(&controller);
    window.show();

    return app.exec();
}
