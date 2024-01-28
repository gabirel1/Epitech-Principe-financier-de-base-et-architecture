#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>

#include <QMainWindow>
#include <QStandardItemModel>

#include "Client/Data/OderBook.hpp"
#include "Client/Data/UDPHandler.hpp"
#include "Client/GUI/GestionnaireSocket.hpp"
#include "Common/Core/Order.hpp"
#include "Common/Network/Socket.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    NetIO m_netio;

    GestionnaireSocket m_gsocket;
    Ui::MainWindow *m_ui;

    std::unordered_map<std::string, std::pair<Order, Price>> m_orderList; // Real Need with the orderHistory ? 

    std::string getDate();
    void setupValidator();
    void buildMessageTypeLayout();
    void buildNewOrderSingleWidget();
    void buildOrderCancelRequestWidget();
    void buildOrderCancelReplaceRequestWidget();

    size_t m_msgId = 1;

    OrderBook m_ob;

private slots:
    void slot_log();
    void slot_send();
    void slot_cancelOrder();
    void slot_modifyOrder();
    void slot_sendNewOrderSingle();
    void slot_messageTypeLayout(int p_index);
};

#endif // MAINWINDOW_H
