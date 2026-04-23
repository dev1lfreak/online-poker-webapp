#include "WaitingRoomWindow.hpp"

#include "../ClientController.hpp"

#include <QJsonArray>
#include <QLabel>
#include <QListWidget>
#include <QPixmap>
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

    auto *logoLabel = new QLabel();
    QPixmap logoPixmap("./imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("src/imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("../src/imgs/logo_1.png");
    logoLabel->setPixmap(logoPixmap.scaledToWidth(180, Qt::SmoothTransformation));

    playersList_ = new QListWidget();
    lastResultTitleLabel_ = new QLabel("Last Hand Result:");
    lastResultLabel_ = new QLabel("No games played at this table yet");
    lastResultLabel_->setWordWrap(true);
    startButton_ = new QPushButton("Start Game");
    leaveButton_ = new QPushButton("Leave Table");
    leaveButton_->setProperty("role", "danger");

    layout->addWidget(logoLabel, 0, Qt::AlignHCenter);
    layout->addWidget(playersList_);
    layout->addWidget(lastResultTitleLabel_);
    layout->addWidget(lastResultLabel_);
    layout->addWidget(startButton_);
    layout->addWidget(leaveButton_);

    setCentralWidget(central);
    setWindowTitle("Online Poker - Waiting Room");
    resize(420, 400);
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

    if (state.value("hasLastHandResult").toBool(false)) {
        const QString winner = state.value("lastWinnerNickname").toString("unknown");
        const QString combo = state.value("lastWinnerCombination").toString("n/a");
        const int winAmount = state.value("lastWinAmount").toInt(0);
        lastResultLabel_->setText(QString("Winner: %1\nCombination: %2\nWin Amount: %3")
                                      .arg(winner)
                                      .arg(combo)
                                      .arg(winAmount));
        lastResultTitleLabel_->setVisible(true);
    } else {
        lastResultLabel_->setText("No games played at this table yet");
        lastResultTitleLabel_->setVisible(false);
    }

    startButton_->setEnabled(state.value("canStartGame").toBool(false));
}
}
