#include <QApplication>
#include <QFont>

#include "ClientController.hpp"
#include "ui/ConnectionWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setFont(QFont("Segoe UI", 12));
    app.setStyleSheet(
        "QMainWindow, QWidget {"
        "  background-color: #181818;"
        "  color: #f3ecff;"
        "}"
        "QLabel {"
        "  color: #f3ecff;"
        "  font-size: 14px;"
        "}"
        "QLineEdit, QTextEdit, QListWidget, QSpinBox {"
        "  background-color: #1d1333;"
        "  color: #f7f1ff;"
        "  border: 1px solid #323132;"
        "  border-radius: 8px;"
        "  padding: 6px 8px;"
        "  font-size: 14px;"
        "}"
        "QPushButton {"
        "  background-color: #372766;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 8px 12px;"
        "  font-size: 14px;"
        "  font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "  background-color: #3c2f66;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2d2351;"
        "}"
        "QPushButton:disabled {"
        "  background-color: #2b2441;"
        "  color: #9f94bf;"
        "}"
        "QPushButton[role=\"danger\"] {"
        "  background-color: #64212a;"
        "}"
        "QPushButton[role=\"danger\"]:hover {"
        "  background-color: #781927;"
        "}"
        "QPushButton[role=\"danger\"]:pressed {"
        "  background-color: #551821cf;"
        "}"
    );

    client::ClientController controller;
    client::ConnectionWindow window(&controller);
    window.show();

    return app.exec();
}
