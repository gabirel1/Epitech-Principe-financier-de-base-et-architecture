#include "Client/GUI/GestionnaireSocket.hpp"
#include "Client/GUI/mainwindow.hpp"
#include "Client/Core/Core.hpp"
#include <QApplication>


int main(int _ac, char **_av)
{
    QApplication a(_ac, _av);
    MainWindow w;
    w.show();
    return a.exec();
}