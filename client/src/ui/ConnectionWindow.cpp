#include "ConnectionWindow.hpp"

#include "../ClientController.hpp"
#include "MainMenuWindow.hpp"

#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace client {
ConnectionWindow::ConnectionWindow(ClientController *controller, QWidget *parent)
    : QMainWindow(parent), controller_(controller) {
    setupUi();

    connect(connectButton_, &QPushButton::clicked, this, &ConnectionWindow::onConnectClicked);
    connect(exitButton_, &QPushButton::clicked, this, &ConnectionWindow::onExitClicked);
    connect(controller_, &ClientController::statusChanged, this, [this](const QString &status) {
        statusLabel_->setText(status);
    });
    connect(controller_, &ClientController::loginCompleted, this, &ConnectionWindow::onLoginCompleted);
}

void ConnectionWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    hostEdit_ = new QLineEdit("127.0.0.1");
    portEdit_ = new QLineEdit("8080");
    nicknameEdit_ = new QLineEdit();
    nicknameEdit_->setPlaceholderText("Nickname");
    portEdit_->setValidator(new QIntValidator(1, 65535, portEdit_));

    connectButton_ = new QPushButton("Подключиться");
    exitButton_ = new QPushButton("Выход");
    statusLabel_ = new QLabel("Disconnected");

    layout->addWidget(new QLabel("IP:"));
    layout->addWidget(hostEdit_);
    layout->addWidget(new QLabel("Port:"));
    layout->addWidget(portEdit_);
    layout->addWidget(new QLabel("Nick:"));
    layout->addWidget(nicknameEdit_);
    layout->addWidget(connectButton_);
    layout->addWidget(exitButton_);
    layout->addWidget(statusLabel_);

    setCentralWidget(central);
    setWindowTitle("Online Poker - Connect");
    resize(360, 240);
}

void ConnectionWindow::onConnectClicked() {
    const QString nick = nicknameEdit_->text().trimmed();
    if (nick.isEmpty()) {
        QMessageBox::warning(this, "Nickname", "Введите ник");
        return;
    }
    const QString host = hostEdit_->text().trimmed();
    const quint16 port = static_cast<quint16>(portEdit_->text().toUShort());

    controller_->setNickname(nick);
    controller_->connectToServer(host, port);
    controller_->login();
}

void ConnectionWindow::onExitClicked() {
    controller_->sendDisconnectAndClose();
    close();
}

void ConnectionWindow::onLoginCompleted(quint64) {
    if (!mainMenu_) {
        mainMenu_ = new MainMenuWindow(controller_);
    }
    mainMenu_->show();
    hide();
}
}
