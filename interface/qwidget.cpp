#include "qwidget.h"
#include "ui_qwidget.h"

QWidget::QWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWidget)
{
    ui->setupUi(this);
}

QWidget::~QWidget()
{
    delete ui;
}
