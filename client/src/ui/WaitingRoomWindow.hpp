#pragma once

#include <QMainWindow>
#include <QJsonObject>

class QListWidget;
class QLabel;
class QPushButton;

namespace client {
class ClientController;

class WaitingRoomWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit WaitingRoomWindow(ClientController *controller, QWidget *parent = nullptr);

signals:
    void leaveRequested();

private slots:
    void applyState(const QJsonObject &state);

private:
    void setupUi();

    ClientController *controller_;
    QListWidget *playersList_{nullptr};
    QLabel *lastResultTitleLabel_{nullptr};
    QLabel *lastResultLabel_{nullptr};
    QPushButton *startButton_{nullptr};
    QPushButton *leaveButton_{nullptr};
};
}
