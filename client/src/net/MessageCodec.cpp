#include "MessageCodec.hpp"

#include <QJsonDocument>
#include <QJsonParseError>

namespace client {
QByteArray MessageCodec::withType(const QString &type, const QJsonObject &extra) {
    QJsonObject root = extra;
    root["type"] = type;
    const QByteArray data = QJsonDocument(root).toJson(QJsonDocument::Compact);
    return data + "\n";
}

QByteArray MessageCodec::buildLogin(const QString &nickname) {
    QJsonObject obj;
    obj["payload"] = nickname;
    return withType("login", obj);
}

QByteArray MessageCodec::buildJoin() {
    return withType("join");
}

QByteArray MessageCodec::buildJoinWithBots() {
    return withType("play_with_bots");
}

QByteArray MessageCodec::buildStartGame() {
    return withType("start_game");
}

QByteArray MessageCodec::buildLeave() {
    return withType("leave");
}

QByteArray MessageCodec::buildDisconnect() {
    return withType("disconnect");
}

QByteArray MessageCodec::buildCheck() {
    return withType("check");
}

QByteArray MessageCodec::buildCall() {
    return withType("call");
}

QByteArray MessageCodec::buildFold() {
    return withType("fold");
}

QByteArray MessageCodec::buildBet(int amount) {
    QJsonObject obj;
    obj["amount"] = amount;
    return withType("bet", obj);
}

QJsonObject MessageCodec::parseLine(const QByteArray &line, QString *error) {
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(line, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        if (error) {
            *error = parseError.errorString();
        }
        return {};
    }
    return doc.object();
}
}
