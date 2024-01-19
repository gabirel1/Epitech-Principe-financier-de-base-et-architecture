#include "Client/GUI/mainwindow.hpp"
#include "Client/GUI/ui_FinanceUI.h"

#include "Common/Message/Logon.hpp"
#include "Common/Message/Logout.hpp"
#include "Common/Message/NewOrderSingle.hpp"
#include "Common/Message/OrderCancelRequest.hpp"
#include "Common/Message/OrderCancelReplaceRequest.hpp"
#include "Common/Message/OrderStatusRequest.hpp"
#include "Common/Message/MarketDataRequest.hpp"
#include "Common/Message/HeartBeat.hpp"

#include <iostream>

#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : m_gestionnaireSocket(nullptr)
    , QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    messageID = 0;
    orderID = 0;

    setupValidator();

    m_gestionnaireSocket = new GestionnaireSocket("127.0.0.1", 8080, 8081);
    m_gestionnaireSocket->startThread();

    connect(m_ui->messageType, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_messageTypeLayout(int)));
    connect(m_ui->btn_log, SIGNAL(pressed()), this, SLOT(slot_log()));
    connect(m_ui->btn_send, SIGNAL(pressed()), this, SLOT(slot_send()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupValidator()
{
    m_ui->priceValue_NewOrderSingle->setValidator(new QDoubleValidator);
    m_ui->quantityValue_NewOrderSingle->setValidator(new QDoubleValidator);
    
    m_ui->priceValue_NewOrderList->setValidator(new QDoubleValidator);
    m_ui->quantityValue_NewOrderList->setValidator(new QDoubleValidator);
    
    m_ui->orderIDValue_OrderCancel->setValidator(new QIntValidator);

    m_ui->orderIDValue_OrderCancelReplace->setValidator(new QIntValidator);
    m_ui->priceValue_OrderCancelReplace->setValidator(new QDoubleValidator);
    m_ui->quantityValue_OrderCancelReplace->setValidator(new QDoubleValidator);
}

void MainWindow::slot_log()
{
    if (m_ui->btn_log->text() == "Log On") {
        fix::Logon logon;
    
        logon.header.set34_msgSeqNum(std::to_string(messageID));
        logon.header.set49_SenderCompId("1");
        logon.header.set56_TargetCompId("0");
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt("30");

        m_gestionnaireSocket->sendTcpSocket(logon.to_string());

        m_ui->messageTypeWidgetList->setEnabled(true);
        m_ui->btn_log->setText("Log Out");

    } else {
        fix::Logout logout;

        logout.header.set34_msgSeqNum(std::to_string(messageID));
        logout.header.set49_SenderCompId("1");
        logout.header.set56_TargetCompId("0");

        m_gestionnaireSocket->sendTcpSocket(logout.to_string());

        m_ui->messageTypeWidgetList->setEnabled(false);
        m_ui->btn_log->setText("Log On");

    }
    messageID ++;
}

void MainWindow::slot_send()
{
    switch (m_ui->messageType->currentIndex())
    {
        case 0:
            slot_sendNewOrderSingle();
            break;
        case 1:
            // slot_sendNewOrderList();
            break;
        case 2:
            slot_cancelOrder();
            break;
        case 3:
            slot_modifyOrder();
            break;
        case 4:
            // slot_statusOrder();
            break;
        case 5:
            // slot_marketData();
            break;
        case 6:
            // slor_heartBeat();
            break;
        default:
            break;
    }
}

void MainWindow::slot_sendNewOrderSingle()
{
    fix::NewOrderSingle newOrderSingle;

    newOrderSingle.header.set34_msgSeqNum(std::to_string(messageID));
    newOrderSingle.header.set49_SenderCompId("1");
    newOrderSingle.header.set56_TargetCompId("0");
    newOrderSingle.set11_clOrdID(std::to_string(orderID));
    newOrderSingle.set21_handlInst("3");
    newOrderSingle.set38_orderQty(m_ui->quantityValue_NewOrderSingle->text().toStdString());
    newOrderSingle.set40_ordType("2");
    newOrderSingle.set44_price(m_ui->priceValue_NewOrderSingle->text().toStdString());
    newOrderSingle.set54_side(std::to_string(m_ui->orderType_NewOrderSingle->currentIndex() + 3));
    newOrderSingle.set55_symbol("3");
    newOrderSingle.set60_transactTime(getDate());

    m_gestionnaireSocket->sendTcpSocket(newOrderSingle.to_string());

    Order orderTemp;
    orderTemp.orderId = orderID;
    orderTemp.quantity = m_ui->quantityValue_NewOrderSingle->text().toULongLong();
    orderTemp.userId = m_ui->userIdValue->text().toULongLong();

    Price priceTemp;
    priceTemp = m_ui->priceValue_NewOrderSingle->text().toLongLong();

    std::pair<Order, Price> pair = std::make_pair(orderTemp, priceTemp);
    m_orderList.emplace(orderID, pair);

    int rowNbr = m_ui->orderHistory->rowCount();
    m_ui->orderHistory->insertRow(rowNbr);

    QTableWidgetItem *item_orderID = new QTableWidgetItem(std::to_string(orderID).c_str());
    item_orderID->setFlags(item_orderID->flags() & ~Qt::ItemIsEditable);
    QTableWidgetItem *item_orderType = new QTableWidgetItem(m_ui->orderType_NewOrderSingle->currentText());
    item_orderType->setFlags(item_orderType->flags() & ~Qt::ItemIsEditable);
    QTableWidgetItem *item_orderPrice = new QTableWidgetItem(m_ui->priceValue_NewOrderSingle->text());
    item_orderPrice->setFlags(item_orderPrice->flags() & ~Qt::ItemIsEditable);
    QTableWidgetItem *item_orderQuantity = new QTableWidgetItem(m_ui->quantityValue_NewOrderSingle->text());
    item_orderQuantity->setFlags(item_orderQuantity->flags() & ~Qt::ItemIsEditable);

    m_ui->orderHistory->setItem(rowNbr, 0, item_orderID);
    m_ui->orderHistory->setItem(rowNbr, 1, item_orderType);
    m_ui->orderHistory->setItem(rowNbr, 2, item_orderPrice);
    m_ui->orderHistory->setItem(rowNbr, 3, item_orderQuantity);

    messageID ++;
    orderID ++;
}

void MainWindow::slot_cancelOrder()
{

    QList<QTableWidgetItem *> list = m_ui->orderHistory->findItems(m_ui->orderIDValue_OrderCancel->text(), Qt::MatchExactly);

    if (list.at(0)) {
        fix::OrderCancelRequest orderCancel;
        QTableWidgetItem *item = m_ui->orderHistory->item(list.at(0)->row(), 1);

        orderCancel.header.set34_msgSeqNum(std::to_string(messageID));
        orderCancel.header.set49_SenderCompId("1");
        orderCancel.header.set56_TargetCompId("0");
        orderCancel.set11_clOrdID(m_ui->orderIDValue_OrderCancel->text().toStdString());
        orderCancel.set41_origClOrdID(std::to_string(orderID));
        orderCancel.set54_side(item->text().toStdString().c_str());
        orderCancel.set55_symbol("");
        orderCancel.set60_transactTime(getDate());

        m_gestionnaireSocket->sendTcpSocket(orderCancel.to_string());

        m_ui->orderHistory->removeRow(list.at(0)->row());

        orderID ++;
    }
}

void MainWindow::slot_modifyOrder()
{

    QList<QTableWidgetItem *> list = m_ui->orderHistory->findItems(m_ui->orderIDValue_OrderCancelReplace->text(), Qt::MatchExactly);

    if (list.at(0)) {

        QTableWidgetItem *item = m_ui->orderHistory->item(list.at(0)->row(), 1);

        fix::OrderCancelReplaceRequest orderCancelReplace;

        orderCancelReplace.header.set34_msgSeqNum(std::to_string(messageID));
        orderCancelReplace.header.set49_SenderCompId("1");
        orderCancelReplace.header.set56_TargetCompId("0");
        orderCancelReplace.set11_clOrdID(m_ui->orderIDValue_OrderCancel->text().toStdString());
        orderCancelReplace.set21_handlInst("");
        orderCancelReplace.set40_ordType("");
        orderCancelReplace.set41_origClOrdID(std::to_string(orderID));
        orderCancelReplace.set54_side(item->text().toStdString().c_str());
        orderCancelReplace.set55_symbol("");
        orderCancelReplace.set60_transactTime(getDate());

        m_gestionnaireSocket->sendTcpSocket(orderCancelReplace.to_string());

        m_ui->orderHistory->item(list.at(0)->row(), 1)->setText(m_ui->orderType_OrderCancelReplace->currentText());
        if (m_ui->priceValue_OrderCancelReplace->text() != "")
            m_ui->orderHistory->item(list.at(0)->row(), 2)->setText(m_ui->priceValue_OrderCancelReplace->text());
        if (m_ui->quantityValue_OrderCancelReplace->text() != "")
            m_ui->orderHistory->item(list.at(0)->row(), 3)->setText(m_ui->quantityValue_OrderCancelReplace->text());

        m_ui->orderHistory->editItem(item);

        orderID ++;
    }
}

void MainWindow::slot_messageTypeLayout(int p_index)
{
    m_ui->messageTypeWidgetList->setCurrentIndex(p_index);
    m_ui->lab_messageType->setText("MessageType : " + m_ui->messageType->currentText());
}

std::string MainWindow::getDate()
{
    std::string date = QDateTime::currentDateTimeUtc().date().toString(Qt::DateFormat::ISODate).split("-").join("").toStdString();
    std::string time = QDateTime::currentDateTimeUtc().time().toString(Qt::DateFormat::ISODateWithMs).toStdString();
    std::string transactTime = date + '-' + time;

    return transactTime;
}