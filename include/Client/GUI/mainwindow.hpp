#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <unordered_map>

#include <QMainWindow>
#include <QStandardItemModel>

#include "Client/GUI/GestionnaireSocket.hpp"
#include "Common/Core/Order.hpp"
#include "Common/Network/Socket.hpp"


static int orderID;
static int messageID;

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

    GestionnaireSocket *m_gestionnaireSocket;
    Ui::MainWindow *m_ui;

    std::unordered_map<int, std::pair<Order, Price>> m_orderList; // Real Need with the orderHistory ? 

    std::string getDate();
    void setupValidator();
    void buildMessageTypeLayout();
    void buildNewOrderSingleWidget();
    void buildOrderCancelRequestWidget();
    void buildOrderCancelReplaceRequestWidget();

private slots:
    void slot_log();
    void slot_send();
    void slot_cancelOrder();
    void slot_modifyOrder();
    void slot_sendNewOrderSingle();
    void slot_messageTypeLayout(int p_index);
};

#endif // MAINWINDOW_H
