#include "TableWindow.hpp"

#include "../ClientController.hpp"

#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

namespace client {
TableWindow::TableWindow(ClientController *controller, QWidget *parent)
    : QMainWindow(parent), controller_(controller) {
    setupUi();

    connect(controller_, &ClientController::tableStateUpdated, this, &TableWindow::applyState);
    connect(controller_, &ClientController::tableActionReceived, this, &TableWindow::appendAction);
}

void TableWindow::setupUi() {
    auto *central = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(central);

    potLabel_ = new QLabel("Pot: 0");
    gameStatusLabel_ = new QLabel("Stage: Waiting for players");
    timerLabel_ = new QLabel("Timer: 30");
    myCardsLabel_ = new QLabel("My cards: [??] [??]");
    boardCardsLabel_ = new QLabel("Board: [??] [??] [??] [??] [??]");

    playersList_ = new QListWidget();
    actionLog_ = new QTextEdit();
    actionLog_->setReadOnly(true);

    checkBtn_ = new QPushButton("Check");
    callBtn_ = new QPushButton("Call");
    foldBtn_ = new QPushButton("Fold");
    raiseBtn_ = new QPushButton("Raise");
    leaveBtn_ = new QPushButton("Leave table");
    leaveBtn_->setProperty("role", "danger");
    raiseSpin_ = new QSpinBox();
    raiseSpin_->setMinimum(20);
    raiseSpin_->setMaximum(100000);
    raiseSpin_->setValue(20);

    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(checkBtn_);
    actionsLayout->addWidget(callBtn_);
    actionsLayout->addWidget(foldBtn_);
    actionsLayout->addWidget(raiseSpin_);
    actionsLayout->addWidget(raiseBtn_);
    actionsLayout->addWidget(leaveBtn_);

    mainLayout->addWidget(potLabel_);
    mainLayout->addWidget(gameStatusLabel_);
    mainLayout->addWidget(timerLabel_);
    mainLayout->addWidget(myCardsLabel_);
    mainLayout->addWidget(boardCardsLabel_);
    mainLayout->addWidget(new QLabel("Players:"));
    mainLayout->addWidget(playersList_);
    mainLayout->addLayout(actionsLayout);
    mainLayout->addWidget(new QLabel("Action log:"));
    mainLayout->addWidget(actionLog_);

    setCentralWidget(central);
    setWindowTitle("Online Poker - Table");
    resize(850, 670);

    connect(checkBtn_, &QPushButton::clicked, controller_, &ClientController::sendCheck);
    connect(callBtn_, &QPushButton::clicked, controller_, &ClientController::sendCall);
    connect(foldBtn_, &QPushButton::clicked, controller_, &ClientController::sendFold);
    connect(raiseBtn_, &QPushButton::clicked, this, [this]() {
        controller_->sendRaise(raiseSpin_->value());
    });
    connect(leaveBtn_, &QPushButton::clicked, this, [this]() {
        controller_->leaveTable();
        emit leaveRequested();
    });

    updateActionControls();

    localTimer_ = new QTimer(this);
    connect(localTimer_, &QTimer::timeout, this, &TableWindow::tickLocalTimer);
    localTimer_->start(1000);
}

void TableWindow::setCardLabels(const QJsonArray &board, const QJsonArray &holeCards) {
    QStringList my;
    for (const QJsonValue &v : holeCards) {
        my << v.toString("??");
    }
    while (my.size() < 2) my << "??";
    myCardsLabel_->setText(QString("My cards: [%1] [%2]").arg(my[0], my[1]));

    QStringList boardList;
    for (const QJsonValue &v : board) {
        boardList << v.toString("??");
    }
    while (boardList.size() < 5) boardList << "??";
    boardCardsLabel_->setText(
        QString("Board: [%1] [%2] [%3] [%4] [%5]")
            .arg(boardList[0], boardList[1], boardList[2], boardList[3], boardList[4]));
}

void TableWindow::applyState(const QJsonObject &state) {
    potLabel_->setText(QString("Pot: %1").arg(state.value("pot").toInt(0)));
    currentBet_ = state.value("currentBet").toInt(0);
    myRoundBet_ = state.value("myRoundBet").toInt(0);
    callAmount_ = state.value("callAmount").toInt(0);
    currentGameState_ = state.value("gameState").toInt(0);
    smallBlindPlayerId_ = static_cast<quint64>(state.value("smallBlindPlayerId").toInteger(0));
    bigBlindPlayerId_ = static_cast<quint64>(state.value("bigBlindPlayerId").toInteger(0));
    smallBlindAmount_ = state.value("smallBlindAmount").toInt(20);
    bigBlindAmount_ = state.value("bigBlindAmount").toInt(40);

    const int gameState = currentGameState_;
    QString gameStageText;
    switch (gameState) {
        case 1:
            gameStageText = "Pre-flop";
            break;
        case 2:
            gameStageText = "Flop";
            break;
        case 3:
            gameStageText = "Turn";
            break;
        case 4:
            gameStageText = "River";
            break;
        case 5:
            gameStageText = "Showdown";
            break;
        default:
            gameStageText = "Waiting for players";
            break;
    }
    gameStatusLabel_->setText(QString("Stage: %1").arg(gameStageText));
    currentTurnPlayerId_ = static_cast<quint64>(state.value("currentTurnPlayerId").toInteger(0));
    remainingSeconds_ = 30;
    updateActionControls();

    const QJsonArray board = state.value("board").toArray();
    const QJsonArray holeCards = state.value("myCards").toArray();
    setCardLabels(board, holeCards);

    playersList_->clear();
    playerNamesById_.clear();
    const QJsonArray players = state.value("players").toArray();
    for (const QJsonValue &value : players) {
        const QJsonObject p = value.toObject();
        const quint64 playerId = static_cast<quint64>(p.value("playerId").toInteger(0));
        const QString nickname = p.value("nickname").toString("player");
        playerNamesById_.insert(playerId, nickname);
        const QString stateText = p.value("stateText").toString(QString::number(p.value("state").toInt()));
        const QString dealerMark = p.value("isDealer").toBool() ? " [D]" : "";
        const QString botMark = p.value("isBot").toBool() ? " [BOT]" : "";
        const QString turnMark = playerId == currentTurnPlayerId_ ? " <- turn" : "";
        playersList_->addItem(QString("%1 (#%2)%3 | chips=%4 | %5%6%7")
                                  .arg(nickname)
                                  .arg(playerId)
                                  .arg(botMark)
                                  .arg(p.value("chips").toInt())
                                  .arg(stateText, dealerMark, turnMark));
    }
}

void TableWindow::updateActionControls() {
    const bool isMyTurn = controller_ && controller_->playerId() != 0 &&
                          controller_->playerId() == currentTurnPlayerId_;
    const bool isBlindPostingTurn = currentGameState_ == 1 &&
                                    (currentTurnPlayerId_ == smallBlindPlayerId_ || currentTurnPlayerId_ == bigBlindPlayerId_);
    const int blindAmount = currentTurnPlayerId_ == smallBlindPlayerId_ ? smallBlindAmount_ : bigBlindAmount_;
    const bool facingRaise = callAmount_ > 0;

    checkBtn_->setEnabled(isMyTurn && !isBlindPostingTurn && !facingRaise);
    callBtn_->setEnabled(isMyTurn && !isBlindPostingTurn && facingRaise);
    foldBtn_->setEnabled(isMyTurn && !isBlindPostingTurn);
    raiseBtn_->setEnabled(isMyTurn);
    raiseBtn_->setText(isBlindPostingTurn ? "Bet" : "Raise");
    if (isBlindPostingTurn) {
        raiseSpin_->setValue(blindAmount);
        raiseSpin_->setEnabled(false);
    } else {
        const int minRaiseAmount = facingRaise ? (callAmount_ + 1) : 20;
        raiseSpin_->setMinimum(minRaiseAmount);
        if (raiseSpin_->value() < minRaiseAmount) {
            raiseSpin_->setValue(minRaiseAmount);
        }
        raiseSpin_->setEnabled(isMyTurn);
    }
}

void TableWindow::appendAction(const QJsonObject &action) {
    const QString event = action.value("event").toString("action");
    const quint64 playerId = static_cast<quint64>(action.value("playerId").toInteger(0));
    const QString nickname = playerNamesById_.value(playerId, QString("unknown (#%1)").arg(playerId));
    const int amount = action.value("amount").toInt(0);
    if (amount > 0) {
        actionLog_->append(QString("[%1] %2 amount %3").arg(event).arg(nickname).arg(amount));
    } else {
        actionLog_->append(QString("[%1] %2").arg(event).arg(nickname));
    }
}

void TableWindow::tickLocalTimer() {
    if (remainingSeconds_ > 0) {
        --remainingSeconds_;
    }
    timerLabel_->setText(QString("Timer: %1").arg(remainingSeconds_));
}
}
