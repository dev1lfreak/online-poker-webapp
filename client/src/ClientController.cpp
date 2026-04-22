#include "ClientController.hpp"

#include "net/NetworkWorker.hpp"

#include <QMetaObject>

namespace client {
ClientController::ClientController(QObject *parent)
    : QObject(parent), worker_(new NetworkWorker) {
    worker_->moveToThread(&networkThread_);

    connect(&networkThread_, &QThread::finished, worker_, &QObject::deleteLater);

    connect(this, &ClientController::requestConnect, worker_, &NetworkWorker::connectToServer, Qt::QueuedConnection);
    connect(this, &ClientController::requestDisconnect, worker_, &NetworkWorker::disconnectFromServer, Qt::QueuedConnection);
    connect(this, &ClientController::requestLogin, worker_, &NetworkWorker::sendLogin, Qt::QueuedConnection);
    connect(this, &ClientController::requestFindGame, worker_, &NetworkWorker::sendJoin, Qt::QueuedConnection);
    connect(this, &ClientController::requestFindGameWithBots, worker_, &NetworkWorker::sendJoinWithBots, Qt::QueuedConnection);
    connect(this, &ClientController::requestStartGame, worker_, &NetworkWorker::sendStartGame, Qt::QueuedConnection);
    connect(this, &ClientController::requestLeaveTable, worker_, &NetworkWorker::sendLeave, Qt::QueuedConnection);
    connect(this, &ClientController::requestSendDisconnect, worker_, &NetworkWorker::sendDisconnect, Qt::QueuedConnection);
    connect(this, &ClientController::requestCheck, worker_, &NetworkWorker::sendCheck, Qt::QueuedConnection);
    connect(this, &ClientController::requestCall, worker_, &NetworkWorker::sendCall, Qt::QueuedConnection);
    connect(this, &ClientController::requestFold, worker_, &NetworkWorker::sendFold, Qt::QueuedConnection);
    connect(this, &ClientController::requestRaise, worker_, &NetworkWorker::sendRaise, Qt::QueuedConnection);

    connect(worker_, &NetworkWorker::connected, this, [this]() {
        emit statusChanged("Connected to server");
        if (loginAfterConnect_ && !nickname_.isEmpty()) {
            loginAfterConnect_ = false;
            emit requestLogin(nickname_);
        }
    });
    connect(worker_, &NetworkWorker::disconnected, this, [this]() {
        emit statusChanged("Disconnected from server");
        tableId_ = -1;
        waitingForTable_ = false;
    });
    connect(worker_, &NetworkWorker::socketError, this, [this](const QString &error) {
        emit statusChanged(QString("Network error: %1").arg(error));
    });

    connect(worker_, &NetworkWorker::loginAccepted, this, [this](quint64 id) {
        playerId_ = id;
        emit loginCompleted(id);
        emit statusChanged(QString("Logged in as #%1").arg(id));
    });

    connect(worker_, &NetworkWorker::joinedTable, this, [this](int id) {
        if (id < 0) return;
        tableId_ = id;
        waitingForTable_ = false;
        emit tableJoined(id);
    });

    connect(worker_, &NetworkWorker::stateReceived, this, [this](const QJsonObject &state) {
        if (state.contains("tableId")) {
            const int id = state.value("tableId").toInt(-1);
            if (id >= 0 && (waitingForTable_ || tableId_ < 0)) {
                tableId_ = id;
                waitingForTable_ = false;
                emit tableJoined(id);
            }
        }
        emit waitingRoomUpdated(state);
        const bool nextGameInProgress = state.value("gameInProgress").toBool(false);
        if (nextGameInProgress && !gameInProgress_) {
            gameInProgress_ = true;
            emit gameStarted();
        } else if (!nextGameInProgress && gameInProgress_) {
            gameInProgress_ = false;
            emit gameEnded();
        }
        emit tableStateUpdated(state);
    });
    connect(worker_, &NetworkWorker::actionReceived, this, &ClientController::tableActionReceived);
    connect(worker_, &NetworkWorker::chatReceived, this, &ClientController::chatMessageReceived);

    networkThread_.start();
}

ClientController::~ClientController() {
    emit requestDisconnect();
    networkThread_.quit();
    networkThread_.wait();
}

void ClientController::connectToServer(const QString &host, quint16 port) {
    emit statusChanged(QString("Connecting to %1:%2 ...").arg(host).arg(port));
    emit requestConnect(host, port);
}

void ClientController::disconnectFromServer() {
    emit requestDisconnect();
}

void ClientController::setNickname(const QString &nickname) {
    nickname_ = nickname.trimmed();
}

QString ClientController::nickname() const {
    return nickname_;
}

void ClientController::login() {
    if (nickname_.isEmpty()) {
        emit statusChanged("Nickname is empty");
        return;
    }
    loginAfterConnect_ = true;
    emit requestLogin(nickname_);
}

void ClientController::findGame() {
    waitingForTable_ = true;
    emit statusChanged("Searching free table ...");
    emit requestFindGame();
}

void ClientController::findGameWithBots() {
    waitingForTable_ = true;
    emit statusChanged("Searching table with bots ...");
    emit requestFindGameWithBots();
}

void ClientController::startGame() {
    emit requestStartGame();
}

void ClientController::leaveTable() {
    emit requestLeaveTable();
    tableId_ = -1;
    gameInProgress_ = false;
}

void ClientController::sendDisconnectAndClose() {
    emit requestSendDisconnect();
    emit requestDisconnect();
}

void ClientController::sendCheck() {
    emit requestCheck();
}

void ClientController::sendCall() {
    emit requestCall();
}

void ClientController::sendFold() {
    emit requestFold();
}

void ClientController::sendRaise(int amount) {
    emit requestRaise(amount);
}

quint64 ClientController::playerId() const {
    return playerId_;
}

int ClientController::tableId() const {
    return tableId_;
}
}
