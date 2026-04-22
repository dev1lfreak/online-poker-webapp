#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>

namespace client {
class NetworkWorker : public QObject {
    Q_OBJECT
public:
    explicit NetworkWorker(QObject *parent = nullptr);

public slots:
    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();

    void sendLogin(const QString &nickname);
    void sendJoin();
    void sendJoinWithBots();
    void sendStartGame();
    void sendLeave();
    void sendDisconnect();
    void sendCheck();
    void sendCall();
    void sendFold();
    void sendRaise(int amount);

signals:
    void connected();
    void disconnected();
    void socketError(const QString &error);

    void loginAccepted(quint64 playerId);
    void joinedTable(int tableId);
    void actionReceived(const QJsonObject &event);
    void stateReceived(const QJsonObject &state);
    void chatReceived(const QString &text);
    void rawMessageReceived(const QJsonObject &message);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    void sendBytes(const QByteArray &payload);
    void handleIncoming(const QJsonObject &message);
    QJsonObject parsePayloadObject(const QJsonObject &message) const;

    QTcpSocket *socket_{nullptr};
    QByteArray readBuffer_;
};
}
