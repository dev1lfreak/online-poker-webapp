#include "TableWindow.hpp"

#include "../ClientController.hpp"

#include <QHBoxLayout>
#include <QColor>
#include <QFont>
#include <QJsonArray>
#include <QJsonValue>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QProxyStyle>
#include <QSizePolicy>
#include <QSpinBox>
#include <QStyleOptionSpinBox>
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
    auto *topTimerLayout = new QHBoxLayout();
    auto *rootLayout = new QHBoxLayout();
    auto *leftLayout = new QVBoxLayout();
    auto *rightLayout = new QVBoxLayout();

    auto *logoLabel = new QLabel();
    QPixmap logoPixmap("./imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("src/imgs/logo_1.png");
    if (logoPixmap.isNull()) logoPixmap.load("../src/imgs/logo_1.png");
    logoLabel->setPixmap(logoPixmap.scaledToWidth(170, Qt::SmoothTransformation));

    potLabel_ = new QLabel("Pot: 0");
    gameStatusLabel_ = new QLabel("Stage: Waiting for players");
    timerBar_ = new QProgressBar();
    timerBar_->setRange(0, 30);
    timerBar_->setValue(30);
    timerBar_->setFormat("Timer: %v s");
    timerBar_->setTextVisible(true);
    timerBar_->setMinimumWidth(280);
    timerBar_->setMaximumWidth(420);
    timerBar_->setMinimumHeight(24);
    timerBar_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto *boardCardsLayout = new QHBoxLayout();
    boardCardsLayout->setSpacing(8);
    auto *myCardsLayout = new QHBoxLayout();
    myCardsLayout->setSpacing(8);

    for (QLabel *&label : boardCardLabels_) {
        label = new QLabel();
        label->setFixedSize(68, 96);
        label->setScaledContents(true);
        setCardPixmap(label, "??", false);
        boardCardsLayout->addWidget(label);
    }
    for (QLabel *&label : myCardLabels_) {
        label = new QLabel();
        label->setFixedSize(68, 96);
        label->setScaledContents(true);
        setCardPixmap(label, "??", false);
        myCardsLayout->addWidget(label);
    }

    boardCardsLayout->setAlignment(Qt::AlignHCenter);
    myCardsLayout->setAlignment(Qt::AlignHCenter);

    potLabel_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    gameStatusLabel_->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

    playersList_ = new QListWidget();
    playersList_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    playersList_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    playersList_->setStyleSheet(
        "QListWidget { background: transparent; border: none; }"
        "QListWidget::item { background: transparent; border: none; font-size: 12px; }"
    );
    actionLog_ = new QTextEdit();
    actionLog_->setReadOnly(true);
    actionLog_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    actionLog_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    actionLog_->setStyleSheet(
        "QTextEdit {"
        "  background: transparent;"
        "}"
    );

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
    raiseSpin_->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
    raiseSpin_->setStyleSheet(
        "QSpinBox {"
        "  padding-right: 28px;"
        "  min-height: 24px;"
        "  border-radius: 10px;"
        "}"
        "QSpinBox::up-button {"
        "  subcontrol-origin: border;"
        "  subcontrol-position: top right;"
        "  width: 24px;"
        "  height: 18px;"
        "  font: #ffffff;"
        "  border: none;"
        "  border-left: 1px solid #424242;"
        "  border-top-right-radius: 10px;"
        "  border-bottom: 1px solid #424242;"
        "  background: #343434;"
        "}"
        "QSpinBox::down-button {"
        "  subcontrol-origin: border;"
        "  subcontrol-position: bottom right;"
        "  width: 24px;"
        "  height: 18px;"
        "  font: #ffffff;"
        "  border: none;"
        "  border-left: 1px solid #424242;"
        "  border-top: 1px solid #424242;"
        "  border-bottom-right-radius: 10px;"
        "  background: #343434;"
        "}"
        "QSpinBox::up-button:hover, QSpinBox::down-button:hover {"
        "  background: #5e5e5e;"
        "}"
        "QSpinBox::up-arrow {"
        "  image: none;"
        "  width: 0;"
        "  height: 0;"
        "  border-left: 3px solid transparent;"
        "  border-right: 3px solid #ffffff;"
        "  border-bottom: 1px solid #ffffff;"
        "}"
        "QSpinBox::down-arrow {"
        "  image: none;"
        "  width: 0;"
        "  height: 0;"
        "  border-left: 3px solid transparent;"
        "  border-right: 3px solid transparent;"
        "  border-top: 1px solid #ffffff;"
        "}"
    );

    auto *actionsLayout = new QHBoxLayout();
    actionsLayout->addWidget(checkBtn_);
    actionsLayout->addWidget(callBtn_);
    actionsLayout->addWidget(foldBtn_);
    actionsLayout->addWidget(raiseSpin_);
    actionsLayout->addWidget(raiseBtn_);
    actionsLayout->addWidget(leaveBtn_);

    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(6);
    leftLayout->addWidget(logoLabel, 0, Qt::AlignLeft);
    leftLayout->addWidget(potLabel_);
    leftLayout->addWidget(gameStatusLabel_);
    leftLayout->addStretch(1);
    leftLayout->addLayout(boardCardsLayout);
    leftLayout->addStretch(1);
    leftLayout->addLayout(myCardsLayout);
    leftLayout->addSpacing(8);
    leftLayout->addLayout(actionsLayout);
    leftLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    auto *playersTitle = new QLabel("Players:");
    auto *actionLogTitle = new QLabel("Action log:");
    rightLayout->addWidget(playersTitle);
    rightLayout->addWidget(playersList_, 1);
    rightLayout->addWidget(actionLogTitle);
    rightLayout->addWidget(actionLog_);
    rightLayout->setAlignment(Qt::AlignTop);

    topTimerLayout->addStretch(1);
    topTimerLayout->addWidget(timerBar_);
    topTimerLayout->addStretch(1);

    rootLayout->addLayout(leftLayout, 2);
    rootLayout->addLayout(rightLayout, 1);
    mainLayout->addLayout(topTimerLayout);
    mainLayout->addLayout(rootLayout, 1);

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

void TableWindow::setCardPixmap(QLabel *label, const QString &cardCode, bool hideUnknown) {
    const QString normalized = cardCode.trimmed().toUpper();

    QString imagePath;
    if (!hideUnknown && (normalized.isEmpty() || normalized == "??")) {
        imagePath = "./imgs/deck/card.png";
    } else {
        imagePath = QString("./imgs/deck/%1.png").arg(normalized);
    }

    QPixmap pixmap(imagePath);
    if (pixmap.isNull() && !hideUnknown && (normalized.isEmpty() || normalized == "??")) {
        pixmap.load("imgs/deck/card.png");
        if (pixmap.isNull()) pixmap.load("src/imgs/deck/card.png");
        if (pixmap.isNull()) pixmap.load("../src/imgs/deck/card.png");
    }
    if (pixmap.isNull()) {
        pixmap.load(QString("imgs/deck/%1.png").arg(normalized));
        if (pixmap.isNull()) pixmap.load(QString("src/imgs/deck/%1.png").arg(normalized));
        if (pixmap.isNull()) pixmap.load(QString("../src/imgs/deck/%1.png").arg(normalized));
    }
    if (pixmap.isNull()) {
        pixmap.load("./imgs/deck/card.png");
        if (pixmap.isNull()) pixmap.load("imgs/deck/card.png");
        if (pixmap.isNull()) pixmap.load("src/imgs/deck/card.png");
        if (pixmap.isNull()) pixmap.load("../src/imgs/deck/card.png");
    }

    label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void TableWindow::setCardImages(const QJsonArray &board, const QJsonArray &holeCards) {
    for (int i = 0; i < static_cast<int>(boardCardLabels_.size()); ++i) {
        const QString cardCode = (i < board.size()) ? board.at(i).toString("??") : "??";
        setCardPixmap(boardCardLabels_[i], cardCode, false);
    }
    for (int i = 0; i < static_cast<int>(myCardLabels_.size()); ++i) {
        const QString cardCode = (i < holeCards.size()) ? holeCards.at(i).toString("??") : "??";
        setCardPixmap(myCardLabels_[i], cardCode, false);
    }
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
    timerBar_->setValue(remainingSeconds_);
    updateActionControls();

    const QJsonArray board = state.value("board").toArray();
    const QJsonArray holeCards = state.value("myCards").toArray();
    setCardImages(board, holeCards);

    playersList_->clear();
    playerNamesById_.clear();
    const QJsonArray players = state.value("players").toArray();
    for (const QJsonValue &value : players) {
        const QJsonObject p = value.toObject();
        const quint64 playerId = static_cast<quint64>(p.value("playerId").toInteger(0));
        const QString nickname = p.value("nickname").toString("player");
        playerNamesById_.insert(playerId, nickname);
        const int playerState = p.value("state").toInt();
        const QString dealerMark = p.value("isDealer").toBool() ? " [D]" : "";
        const QString botMark = p.value("isBot").toBool() ? " [BOT]" : "";
        const QString turnMark = playerId == currentTurnPlayerId_ ? " <- turn" : "";
        auto *item = new QListWidgetItem(QString("%1 (#%2)%3 | chips=%4%5%6")
                                             .arg(nickname)
                                             .arg(playerId)
                                             .arg(botMark)
                                             .arg(p.value("chips").toInt())
                                             .arg(dealerMark)
                                             .arg(turnMark));
        if (playerState == 1) {
            item->setForeground(QColor("#7d7d7d"));
        }
        playersList_->addItem(item);
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
    timerBar_->setValue(remainingSeconds_);
}
}
