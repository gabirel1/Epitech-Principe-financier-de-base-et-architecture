#include "Client/GUI/mainwindow.hpp"
#include "Client/GUI/ui_FinanceUI.h"
#include "Common/Message/Logon.hpp"
#include "Common/Message/Logout.hpp"
#include "Common/Message/NewOrderSingle.hpp"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : gestionnaireSocket(nullptr)
    , QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gestionnaireSocket = new GestionnaireSocket("127.0.0.1", 8080, 8081);
    gestionnaireSocket->startThread();

    connect(ui->btn_logOn, SIGNAL(pressed()), this, SLOT(slot_logOn()));
    connect(ui->btn_sendOrder, SIGNAL(pressed()), this, SLOT(slot_sendOrder()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::slot_logOn()
{

    if (ui->btn_logOn->text() == "Log On") {
        fix::Logon logon;

        logon.header.setSeqNum(1);
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt("30");

        gestionnaireSocket->sendTcpSocket(logon.to_string());

        ui->btn_logOn->setText("Log Out");
    } else {

        fix::Logout logout;

        logout.header.setSeqNum(1);
        logout.set58_Text("c'est ciao !");

        gestionnaireSocket->sendTcpSocket(logout.to_string());

        ui->btn_logOn->setText("Log On");
    }
}

void MainWindow::slot_sendOrder()
{
    fix::NewOrderSingle newOrderSingle;

    newOrderSingle.header.setSeqNum(1);

    newOrderSingle.set11_ClOrdID("");
    newOrderSingle.set21_HandlInst("");
    newOrderSingle.set40_OrdType("");
    newOrderSingle.set54_Side("");
    newOrderSingle.set55_Symbol("");
    newOrderSingle.set60_TransactTime("");

    gestionnaireSocket->sendTcpSocket(newOrderSingle.to_string());
}