#include <QtTest/QtTest>

#include "../src/net/MessageCodec.hpp"

using client::MessageCodec;

class ClientProtocolTest : public QObject {
    Q_OBJECT
private slots:
    void buildJoinMessageContainsType() {
        const QByteArray line = MessageCodec::buildJoin();
        QVERIFY(line.contains("\"type\":\"join\""));
        QVERIFY(line.endsWith('\n'));
    }

    void buildRaiseMessageContainsAmount() {
        const QByteArray line = MessageCodec::buildBet(250);
        QVERIFY(line.contains("\"type\":\"bet\""));
        QVERIFY(line.contains("\"amount\":250"));
    }

    void buildStartGameMessageContainsType() {
        const QByteArray line = MessageCodec::buildStartGame();
        QVERIFY(line.contains("\"type\":\"start_game\""));
    }

    void parseLineParsesJsonObject() {
        QString error;
        const auto obj = MessageCodec::parseLine(R"({"type":"state","tableId":2})", &error);
        QVERIFY(error.isEmpty());
        QCOMPARE(obj.value("type").toString(), QString("state"));
        QCOMPARE(obj.value("tableId").toInt(), 2);
    }
};

QTEST_MAIN(ClientProtocolTest)
#include "ClientProtocolTest.moc"
