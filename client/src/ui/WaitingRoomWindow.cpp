#include "WaitingRoomWindow.hpp"

#include "../ClientController.hpp"

#include <QJsonArray>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace client {
WaitingRoomWindow::WaitingRoomWindow(ClientController *controller, QWidget *parent)
    : QMainWindow(parent), controller_(controller) {
    setupUi();
    connect(controller_, &ClientController::waitingRoomUpdated, this, &WaitingRoomWindow::applyState);
    connect(startButton_, &QPushButton::clicked, controller_, &ClientController::startGame);
    connect(leaveButton_, &QPushButton::clicked, this, [this]() {
        controller_->leaveTable();
        emit leaveRequested();
    });
}

void WaitingRoomWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    playersList_ = new QListWidget();
    startButton_ = new QPushButton("Начать игру");
    leaveButton_ = new QPushButton("Выйти из-за стола");

    layout->addWidget(playersList_);
    layout->addWidget(startButton_);
    layout->addWidget(leaveButton_);

    setCentralWidget(central);
    setWindowTitle("Online Poker - Waiting Room");
    resize(420, 360);
}

void WaitingRoomWindow::applyState(const QJsonObject &state) {
    playersList_->clear();
    const auto players = state.value("players").toArray();
    for (const auto &value : players) {
        const auto p = value.toObject();
        const QString botMark = p.value("isBot").toBool() ? " [BOT]" : "";
        playersList_->addItem(QString("%1%2 | chips=%3")
                                  .arg(p.value("nickname").toString())
                                  .arg(botMark)
                                  .arg(p.value("chips").toInt()));
    }
    startButton_->setEnabled(state.value("canStartGame").toBool(false));
}
}
