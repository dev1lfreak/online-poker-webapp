#pragma once

#include <QJsonObject>
#include <QString>

namespace client {
class MessageCodec {
public:
    static QByteArray buildLogin(const QString &nickname);
    static QByteArray buildJoin();
    static QByteArray buildJoinWithBots();
    static QByteArray buildStartGame();
    static QByteArray buildLeave();
    static QByteArray buildDisconnect();
    static QByteArray buildCheck();
    static QByteArray buildCall();
    static QByteArray buildFold();
    static QByteArray buildBet(int amount);

    static QJsonObject parseLine(const QByteArray &line, QString *error = nullptr);

private:
    static QByteArray withType(const QString &type, const QJsonObject &extra = {});
};
}
