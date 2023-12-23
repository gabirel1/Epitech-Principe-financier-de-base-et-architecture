#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Client/GUI/GestionnaireSocket.hpp"
#include "Common/Network/Socket.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GestionnaireSocket *gestionnaireSocket;

private slots:
    void slot_logOn();
    void slot_sendOrder();

};
#endif // MAINWINDOW_H
