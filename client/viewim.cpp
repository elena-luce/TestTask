#include "viewim.h"
#include "ui_viewim.h"

ViewIm::ViewIm(QString dir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewIm)
{
    ui->setupUi(this);

    QPixmap pic(dir);
    ui->imLabel->setPixmap(pic);
}

ViewIm::~ViewIm()
{
    delete ui;
}
