#pragma once

#include <QMainWindow>
#include <QJsonArray>
#include <QJsonObject>

class QLabel;
class QListWidget;
class QPushButton;
class QSpinBox;
class QTextEdit;
class QTimer;

namespace client {
class ClientController;

class TableWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit TableWindow(ClientController *controller, QWidget *parent = nullptr);

signals:
    void leaveRequested();

private slots:
    void applyState(const QJsonObject &state);
    void appendAction(const QJsonObject &action);
    void tickLocalTimer();

private:
    void setupUi();
    void setCardLabels(const QJsonArray &board, const QJsonArray &holeCards);

    ClientController *controller_;

    QLabel *potLabel_{nullptr};
    QLabel *timerLabel_{nullptr};
    QLabel *myCardsLabel_{nullptr};
    QLabel *boardCardsLabel_{nullptr};
    QLabel *handsLabel_{nullptr};
    QListWidget *playersList_{nullptr};
    QTextEdit *actionLog_{nullptr};

    QPushButton *checkBtn_{nullptr};
    QPushButton *callBtn_{nullptr};
    QPushButton *foldBtn_{nullptr};
    QPushButton *raiseBtn_{nullptr};
    QPushButton *leaveBtn_{nullptr};
    QSpinBox *raiseSpin_{nullptr};

    QTimer *localTimer_{nullptr};
    int remainingSeconds_{30};
    quint64 currentTurnPlayerId_{0};
};
}
