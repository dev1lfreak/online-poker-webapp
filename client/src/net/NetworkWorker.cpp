#include "NetworkWorker.hpp"

#include "MessageCodec.hpp"

#include <QJsonDocument>
#include <QJsonValue>

namespace client {
NetworkWorker::NetworkWorker(QObject *parent)
    : QObject(parent), socket_(new QTcpSocket(this)) {
    connect(socket_, &QTcpSocket::readyRead, this, &NetworkWorker::onReadyRead);
    connect(socket_, &QTcpSocket::connected, this, &NetworkWorker::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &NetworkWorker::onDisconnected);
    connect(socket_, &QTcpSocket::errorOccurred, this, &NetworkWorker::onError);
}

void NetworkWorker::connectToServer(const QString &host, quint16 port) {
    if (socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->abort();
    }
    readBuffer_.clear();
    socket_->connectToHost(host, port);
}

void NetworkWorker::disconnectFromServer() {
    if (socket_->state() == QAbstractSocket::ConnectedState) {
        socket_->disconnectFromHost();
    }
}

void NetworkWorker::sendLogin(const QString &nickname) {
    sendBytes(MessageCodec::buildLogin(nickname));
}

void NetworkWorker::sendJoin() {
    sendBytes(MessageCodec::buildJoin());
}

void NetworkWorker::sendJoinWithBots() {
    sendBytes(MessageCodec::buildJoinWithBots());
}

void NetworkWorker::sendStartGame() {
    sendBytes(MessageCodec::buildStartGame());
}

void NetworkWorker::sendLeave() {
    sendBytes(MessageCodec::buildLeave());
}

void NetworkWorker::sendDisconnect() {
    sendBytes(MessageCodec::buildDisconnect());
}

void NetworkWorker::sendCheck() {
    sendBytes(MessageCodec::buildCheck());
}

void NetworkWorker::sendCall() {
    sendBytes(MessageCodec::buildCall());
}

void NetworkWorker::sendFold() {
    sendBytes(MessageCodec::buildFold());
}

void NetworkWorker::sendRaise(int amount) {
    if (amount <= 0) return;
    sendBytes(MessageCodec::buildBet(amount));
}

void NetworkWorker::sendBytes(const QByteArray &payload) {
    if (socket_->state() != QAbstractSocket::ConnectedState) return;
    socket_->write(payload);
}

void NetworkWorker::onReadyRead() {
    readBuffer_.append(socket_->readAll());

    int newlineIndex = readBuffer_.indexOf('\n');
    while (newlineIndex >= 0) {
        QByteArray line = readBuffer_.left(newlineIndex).trimmed();
        readBuffer_.remove(0, newlineIndex + 1);
        if (!line.isEmpty()) {
            QString parseError;
            const QJsonObject message = MessageCodec::parseLine(line, &parseError);
            if (!message.isEmpty()) {
                handleIncoming(message);
            }
        }
        newlineIndex = readBuffer_.indexOf('\n');
    }
}

void NetworkWorker::handleIncoming(const QJsonObject &message) {
    emit rawMessageReceived(message);
    const QString type = message.value("type").toString();

    if (type == "login") {
        const quint64 playerId = static_cast<quint64>(message.value("playerId").toInteger());
        if (playerId != 0) emit loginAccepted(playerId);
        return;
    }

    if ((type == "join" || type == "play_with_bots") && message.contains("tableId")) {
        const int tableId = message.value("tableId").toInt(-1);
        if (tableId >= 0) emit joinedTable(tableId);
    }

    if (type == "action") {
        emit actionReceived(parsePayloadObject(message));
        return;
    }

    if (type == "state") {
        emit stateReceived(parsePayloadObject(message));
        return;
    }

    if (type == "chat") {
        emit chatReceived(message.value("payload").toString());
    }
}

QJsonObject NetworkWorker::parsePayloadObject(const QJsonObject &message) const {
    const QJsonValue payload = message.value("payload");
    if (payload.isObject()) {
        return payload.toObject();
    }
    if (payload.isString()) {
        const QByteArray payloadBytes = payload.toString().toUtf8();
        const QJsonDocument parsed = QJsonDocument::fromJson(payloadBytes);
        if (parsed.isObject()) return parsed.object();
    }
    return {};
}

void NetworkWorker::onConnected() {
    emit connected();
}

void NetworkWorker::onDisconnected() {
    emit disconnected();
}

void NetworkWorker::onError(QAbstractSocket::SocketError) {
    emit socketError(socket_->errorString());
}
}
