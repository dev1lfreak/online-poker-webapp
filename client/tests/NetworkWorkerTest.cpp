#include <QCoreApplication>
#include <QSignalSpy>
#include <QTcpServer>
#include <QTcpSocket>

#include "../src/net/NetworkWorker.hpp"

using client::NetworkWorker;

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    QTcpServer server;
    if (!server.listen(QHostAddress::LocalHost, 0)) return 1;

    NetworkWorker worker;
    QSignalSpy stateSpy(&worker, &NetworkWorker::stateReceived);
    QSignalSpy connectedSpy(&worker, &NetworkWorker::connected);

    worker.connectToServer("127.0.0.1", static_cast<quint16>(server.serverPort()));
    if (!server.waitForNewConnection(1000)) return 2;
    if (!connectedSpy.wait(1000)) return 3;

    QTcpSocket *peer = server.nextPendingConnection();
    if (!peer) return 4;

    const QByteArray msg = R"({"type":"state","payload":"{\"tableId\":1,\"pot\":120}"})" "\n";
    peer->write(msg);
    peer->flush();

    if (!stateSpy.wait(1000)) return 5;
    if (stateSpy.count() != 1) return 6;

    const auto stateObj = qvariant_cast<QJsonObject>(stateSpy.takeFirst().at(0));
    if (stateObj.value("tableId").toInt() != 1) return 7;
    if (stateObj.value("pot").toInt() != 120) return 8;

    peer->disconnectFromHost();
    peer->deleteLater();
    return 0;
}
