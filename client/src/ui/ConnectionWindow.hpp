#pragma once

#include <QMainWindow>

class QLineEdit;
class QLabel;
class QPushButton;

namespace client {
class ClientController;
class MainMenuWindow;

class ConnectionWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit ConnectionWindow(ClientController *controller, QWidget *parent = nullptr);

private slots:
    void onConnectClicked();
    void onExitClicked();
    void onLoginCompleted(quint64 playerId);

private:
    void setupUi();

    ClientController *controller_;
    MainMenuWindow *mainMenu_{nullptr};

    QLineEdit *hostEdit_{nullptr};
    QLineEdit *portEdit_{nullptr};
    QLineEdit *nicknameEdit_{nullptr};
    QLabel *statusLabel_{nullptr};
    QPushButton *connectButton_{nullptr};
    QPushButton *exitButton_{nullptr};
};
}
