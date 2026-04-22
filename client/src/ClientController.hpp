#pragma once

#include <QObject>
#include <QThread>
#include <QJsonObject>

namespace client {
class NetworkWorker;

class ClientController : public QObject {
    Q_OBJECT
public:
    explicit ClientController(QObject *parent = nullptr);
    ~ClientController() override;

    void connectToServer(const QString &host, quint16 port);
    void disconnectFromServer();

    void setNickname(const QString &nickname);
    QString nickname() const;

    void login();
    void findGame();
    void findGameWithBots();
    void startGame();
    void leaveTable();
    void sendDisconnectAndClose();

    void sendCheck();
    void sendCall();
    void sendFold();
    void sendRaise(int amount);

    quint64 playerId() const;
    int tableId() const;

signals:
    void statusChanged(const QString &status);
    void loginCompleted(quint64 playerId);
    void tableJoined(int tableId);
    void tableStateUpdated(const QJsonObject &state);
    void waitingRoomUpdated(const QJsonObject &state);
    void tableActionReceived(const QJsonObject &action);
    void chatMessageReceived(const QString &message);
    void gameStarted();
    void gameEnded();

    void requestConnect(const QString &host, quint16 port);
    void requestDisconnect();
    void requestLogin(const QString &nickname);
    void requestFindGame();
    void requestFindGameWithBots();
    void requestStartGame();
    void requestLeaveTable();
    void requestSendDisconnect();
    void requestCheck();
    void requestCall();
    void requestFold();
    void requestRaise(int amount);

private:
    NetworkWorker *worker_{nullptr};
    QThread networkThread_;
    QString nickname_;
    quint64 playerId_{0};
    int tableId_{-1};
    bool waitingForTable_{false};
    bool loginAfterConnect_{false};
    bool gameInProgress_{false};
};
}
