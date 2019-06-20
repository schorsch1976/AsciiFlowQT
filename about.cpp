#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    connect(ui->btnOk,  &QPushButton::clicked, [this]()
    {
        this->close();
    });
}

About::~About()
{
    delete ui;
}
