#include "Client/GUI/mainwindow.hpp"
#include "Client/GUI/ui_FinanceUI.h"

#include <iostream>

#include <QDateTime>
#include <QThread>
#include <QDebug>

#include "Client/GUI/mainwindow.hpp"
#include "Client/GUI/ui_FinanceUI.h"
#include "Common/Message/Message.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_gsocket("127.0.0.1", 8080, 8081, m_netio),
        m_ui(new Ui::MainWindow), m_ob(m_netio.udp_out, m_netio.tcp_out, m_netio.tcp_in)
{
    m_ui->setupUi(this);

    m_msgId = 1;

    setupValidator();
    m_ui->messageTypeWidgetList->setEnabled(false);

    connect(m_ui->messageType, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_messageTypeLayout(int)));
    connect(m_ui->marketSymbol, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_marketTypeLayout(int)));
    connect(m_ui->btn_log, SIGNAL(pressed()), this, SLOT(slot_log()));
    connect(m_ui->btn_send, SIGNAL(pressed()), this, SLOT(slot_send()));
}

MainWindow::~MainWindow()
{
    fix::Logout logout;

    logout.header.set34_msgSeqNum(std::to_string(m_msgId));
    logout.header.set49_SenderCompId(m_userID);
    logout.header.set56_TargetCompId("MyMarket");

    m_netio.tcp_in.push(std::move(logout));
}

void MainWindow::setupValidator()
{
    m_ui->priceValue_NewOrderSingle->setValidator(new QDoubleValidator);
    m_ui->quantityValue_NewOrderSingle->setValidator(new QDoubleValidator);

    m_ui->orderIDValue_OrderCancel->setValidator(new QIntValidator);

    m_ui->orderIDValue_OrderCancelReplace->setValidator(new QIntValidator);
    m_ui->priceValue_OrderCancelReplace->setValidator(new QDoubleValidator);
    m_ui->quantityValue_OrderCancelReplace->setValidator(new QDoubleValidator);
}

void MainWindow::slot_log()
{
    if (m_ui->btn_log->text() == "Log On") {
        fix::Logon logon;

        m_userID = m_ui->userIdValue->text().toStdString();

        logon.header.set34_msgSeqNum(std::to_string(m_msgId));
        logon.header.set49_SenderCompId(m_userID);
        logon.header.set56_TargetCompId("MyMarket");
        logon.set98_EncryptMethod("0");
        logon.set108_HeartBtInt("30");

        m_netio.tcp_in.push(std::move(logon));

        m_ui->messageTypeWidgetList->setEnabled(true);
        m_ui->btn_log->setText("Log Out");

    } else {
        fix::Logout logout;

        logout.header.set34_msgSeqNum(std::to_string(m_msgId));
        logout.header.set49_SenderCompId(m_userID);
        logout.header.set56_TargetCompId("MyMarket");

        m_netio.tcp_in.push(std::move(logout));

        m_ui->messageTypeWidgetList->setEnabled(false);
        m_ui->btn_log->setText("Log On");

    }
    m_msgId++;
}

void MainWindow::slot_send()
{
    switch (m_ui->messageType->currentIndex())
    {
        case 0:
            slot_sendNewOrderSingle();
            break;
        case 1:
            slot_cancelOrder();
            break;
        case 2:
            slot_modifyOrder();
            break;
        default:
            break;
    }
}

void MainWindow::slot_sendNewOrderSingle()
{
    fix::NewOrderSingle newOrderSingle;

    std::cout << m_msgId <<std::endl;

    newOrderSingle.header.set34_msgSeqNum(std::to_string(m_msgId));
    newOrderSingle.header.set49_SenderCompId(m_userID);
    newOrderSingle.header.set56_TargetCompId("MyMarket");
    newOrderSingle.set11_clOrdID(utils::id());
    newOrderSingle.set21_handlInst("3");
    newOrderSingle.set38_orderQty(m_ui->quantityValue_NewOrderSingle->text().toStdString());
    newOrderSingle.set40_ordType("2");
    newOrderSingle.set44_price(m_ui->priceValue_NewOrderSingle->text().toStdString());
    newOrderSingle.set54_side(std::to_string(m_ui->orderType_NewOrderSingle->currentIndex() + 3));
    newOrderSingle.set55_symbol(m_ui->marketSymbol->currentText().toStdString());
    newOrderSingle.set60_transactTime(getDate());

    m_netio.tcp_in.push(std::move(newOrderSingle));

    Order orderTemp;
    orderTemp.orderId = utils::id();
    orderTemp.quantity = m_ui->quantityValue_NewOrderSingle->text().toULongLong();
    orderTemp.userId = m_ui->userIdValue->text().toULongLong();

    Price priceTemp;
    priceTemp = m_ui->priceValue_NewOrderSingle->text().toLongLong();

    std::pair<Order, Price> pair = std::make_pair(orderTemp, priceTemp);
    m_orderList.emplace(orderTemp.orderId, pair);

    int rowNbr = m_ui->orderHistory->rowCount();
    m_ui->orderHistory->insertRow(rowNbr);

    QTableWidgetItem *item_orderID = new QTableWidgetItem(orderTemp.orderId.c_str());
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

    m_msgId++;
}

void MainWindow::slot_cancelOrder()
{

    QList<QTableWidgetItem *> list = m_ui->orderHistory->findItems(m_ui->orderIDValue_OrderCancel->text(), Qt::MatchExactly);

    if (list.at(0)) {
        fix::OrderCancelRequest orderCancel;
        QTableWidgetItem *item = m_ui->orderHistory->item(list.at(0)->row(), 1);

        std::string side = (item->text() == "Bid") ? "3" : "4";

        orderCancel.header.set34_msgSeqNum(std::to_string(m_msgId));
        orderCancel.header.set49_SenderCompId(m_userID);
        orderCancel.header.set56_TargetCompId("MyMarket");
        orderCancel.set11_clOrdID(utils::id());
        orderCancel.set41_origClOrdID(m_ui->orderIDValue_OrderCancel->text().toStdString());
        orderCancel.set54_side(side);
        orderCancel.set55_symbol("3");
        orderCancel.set60_transactTime(getDate());

        m_netio.tcp_in.push(std::move(orderCancel));

        m_ui->orderHistory->removeRow(list.at(0)->row());
    }
}

void MainWindow::slot_modifyOrder()
{

    QList<QTableWidgetItem *> list = m_ui->orderHistory->findItems(m_ui->orderIDValue_OrderCancelReplace->text(), Qt::MatchExactly);

    if (list.at(0)) {

        QTableWidgetItem *item = m_ui->orderHistory->item(list.at(0)->row(), 1);

        fix::OrderCancelReplaceRequest orderCancelReplace;

        orderCancelReplace.header.set34_msgSeqNum(std::to_string(m_msgId));
        orderCancelReplace.header.set49_SenderCompId(m_userID);
        orderCancelReplace.header.set56_TargetCompId("MyMarket");
        orderCancelReplace.set11_clOrdID(m_ui->orderIDValue_OrderCancelReplace->text().toStdString());
        orderCancelReplace.set21_handlInst("3");
        orderCancelReplace.set38_orderQty(m_ui->quantityValue_OrderCancelReplace->text().toStdString());
        orderCancelReplace.set40_ordType("2");
        orderCancelReplace.set41_origClOrdID(m_ui->orderIDValue_OrderCancelReplace->text().toStdString());
        orderCancelReplace.set44_price(m_ui->priceValue_OrderCancelReplace->text().toStdString());
        orderCancelReplace.set54_side(std::to_string(m_ui->orderType_OrderCancelReplace->currentIndex() + 3));
        orderCancelReplace.set55_symbol(m_ui->marketSymbol->currentText().toStdString());
        orderCancelReplace.set60_transactTime(getDate());

        m_netio.tcp_in.push(std::move(orderCancelReplace));

        m_ui->orderHistory->item(list.at(0)->row(), 1)->setText(m_ui->orderType_OrderCancelReplace->currentText());
        if (m_ui->priceValue_OrderCancelReplace->text() != "")
            m_ui->orderHistory->item(list.at(0)->row(), 2)->setText(m_ui->priceValue_OrderCancelReplace->text());
        if (m_ui->quantityValue_OrderCancelReplace->text() != "")
            m_ui->orderHistory->item(list.at(0)->row(), 3)->setText(m_ui->quantityValue_OrderCancelReplace->text());

        m_ui->orderHistory->editItem(item);
    }
}

// void MainWindow::slot_marketData()
// {
//     fix::MarketDataRequest marketData;

//     marketData.header.set34_msgSeqNum(std::to_string(m_msgId));
//     marketData.header.set49_SenderCompId(m_userID);
//     marketData.header.set56_TargetCompId("MyMarket");
//     marketData.set262_MDReqID();
//     marketData.set263_SubscriptionRequestType("1");
//     marketData.set264_MarketDepth("0");
//     marketData.set267_NoMDEntryTypes();
//     marketData.set269_MDEntryType();
//     marketData.set146_NoRealatedSym();
//     marketData.set55_Symbol();
// }

void MainWindow::slot_heartBeat()
{
    fix::HeartBeat heartBeat;

    std::cout << "HeartBeat slot" <<std::endl;

    heartBeat.header.set34_msgSeqNum(std::to_string(m_msgId));
    heartBeat.header.set49_SenderCompId(m_userID);
    heartBeat.header.set56_TargetCompId("MyMarket");
    heartBeat.set112_testReqID("1");
}

void MainWindow::slot_messageTypeLayout(int p_index)
{
    m_ui->messageTypeWidgetList->setCurrentIndex(p_index);
    m_ui->lab_messageType->setText("MessageType : " + m_ui->messageType->currentText());
}

void MainWindow::slot_marketTypeLayout(int p_index)
{
    m_ui->marketWidgetList->setCurrentIndex(p_index);
}

std::string MainWindow::getDate()
{
    std::string date = QDateTime::currentDateTimeUtc().date().toString(Qt::DateFormat::ISODate).split("-").join("").toStdString();
    std::string time = QDateTime::currentDateTimeUtc().time().toString(Qt::DateFormat::ISODateWithMs).toStdString();
    std::string transactTime = date + '-' + time;

    return transactTime;
}