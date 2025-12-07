#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtsimplegui.h"

class qtsimplegui : public QMainWindow
{
    Q_OBJECT

public:
    qtsimplegui(QWidget *parent = nullptr);
    ~qtsimplegui();

private:
    Ui::qtsimpleguiClass ui;
};

