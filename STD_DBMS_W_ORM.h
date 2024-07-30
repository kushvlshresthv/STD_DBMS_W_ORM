#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_STD_DBMS_W_ORM.h"

class STD_DBMS_W_ORM : public QMainWindow
{
    Q_OBJECT

public:
    STD_DBMS_W_ORM(QWidget *parent = nullptr);
    ~STD_DBMS_W_ORM();

private:
    Ui::STD_DBMS_W_ORMClass ui;
};
