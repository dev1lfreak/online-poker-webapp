#include "MainMenuWindow.hpp"

#include "../ClientController.hpp"
#include "TableWindow.hpp"
#include "WaitingRoomWindow.hpp"

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace client {
MainMenuWindow::MainMenuWindow(ClientController *controller, QWidget *parent)
    : QMainWindow(parent), controller_(controller) {
    setupUi();

    connect(playButton_, &QPushButton::clicked, this, &MainMenuWindow::onPlayClicked);
    connect(playBotsButton_, &QPushButton::clicked, this, &MainMenuWindow::onPlayWithBotsClicked);
    connect(exitButton_, &QPushButton::clicked, this, &MainMenuWindow::onExitClicked);
    connect(controller_, &ClientController::tableJoined, this, &MainMenuWindow::onTableJoined);
    connect(controller_, &ClientController::gameStarted, this, &MainMenuWindow::onGameStarted);
    connect(controller_, &ClientController::gameEnded, this, &MainMenuWindow::onGameEnded);
}

void MainMenuWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *layout = new QVBoxLayout(central);

    playButton_ = new QPushButton("Играть");
    playBotsButton_ = new QPushButton("Играть с ботами");
    exitButton_ = new QPushButton("Выйти");

    layout->addWidget(playButton_);
    layout->addWidget(playBotsButton_);
    layout->addWidget(exitButton_);

    setCentralWidget(central);
    setWindowTitle("Online Poker - Main Menu");
    resize(320, 180);
}

void MainMenuWindow::onPlayClicked() {
    controller_->findGame();
}

void MainMenuWindow::onPlayWithBotsClicked() {
    controller_->findGameWithBots();
}

void MainMenuWindow::onExitClicked() {
    controller_->sendDisconnectAndClose();
    close();
}

void MainMenuWindow::onTableJoined(int tableId) {
    if (!waitingRoom_) {
        waitingRoom_ = new WaitingRoomWindow(controller_);
        connect(waitingRoom_, &WaitingRoomWindow::leaveRequested, this, &MainMenuWindow::onWaitingRoomLeave);
    }
    if (!tableWindow_) {
        tableWindow_ = new TableWindow(controller_);
        connect(tableWindow_, &TableWindow::leaveRequested, this, &MainMenuWindow::onWaitingRoomLeave);
    }
    waitingRoom_->setWindowTitle(QString("Online Poker - Waiting Room #%1").arg(tableId));
    waitingRoom_->show();
    hide();
}

void MainMenuWindow::onWaitingRoomLeave() {
    if (tableWindow_) tableWindow_->hide();
    if (waitingRoom_) waitingRoom_->hide();
    show();
}

void MainMenuWindow::onGameStarted() {
    if (!tableWindow_) return;
    if (waitingRoom_) waitingRoom_->hide();
    tableWindow_->show();
}

void MainMenuWindow::onGameEnded() {
    if (tableWindow_) {
        tableWindow_->hide();
    }
    if (waitingRoom_) {
        waitingRoom_->show();
    }
}
}
