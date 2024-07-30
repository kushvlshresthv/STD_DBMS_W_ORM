#include "STD_DBMS_W_ORM.h"
#include <QtWidgets/QApplication>
void mainKushal();
int main(int argc, char *argv[])
{
    mainKushal();
    QApplication a(argc, argv);
    STD_DBMS_W_ORM w;
    w.show();
    return a.exec();
}
