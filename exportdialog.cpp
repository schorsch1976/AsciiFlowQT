#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(QString data, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    ui->textEditExport->setText(data);
    ui->textEditExport->selectAll();
}

ExportDialog::~ExportDialog()
{
    delete ui;
}
