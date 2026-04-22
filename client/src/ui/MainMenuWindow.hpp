#pragma once

#include <QMainWindow>

class QPushButton;
class QLabel;

namespace client {
class ClientController;
class TableWindow;
class WaitingRoomWindow;

class MainMenuWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainMenuWindow(ClientController *controller, QWidget *parent = nullptr);

private slots:
    void onPlayClicked();
    void onPlayWithBotsClicked();
    void onExitClicked();
    void onTableJoined(int tableId);
    void onWaitingRoomLeave();
    void onGameStarted();
    void onGameEnded();

private:
    void setupUi();

    ClientController *controller_;
    TableWindow *tableWindow_{nullptr};
    WaitingRoomWindow *waitingRoom_{nullptr};

    QPushButton *playButton_{nullptr};
    QPushButton *playBotsButton_{nullptr};
    QPushButton *exitButton_{nullptr};
};
}
