#include "importdialog.h"
#include "ui_importdialog.h"

#include <QString>

ImportDialog::ImportDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::ImportDialog) {
  ui->setupUi(this);

  connect(ui->btnOk, &QPushButton::clicked, [this]() {
    emit ImportedData(ui->textEditImport->toPlainText());
    this->close();
  });
  connect(ui->btnCancel, &QPushButton::clicked, [this]() { this->close(); });
}

ImportDialog::~ImportDialog() { delete ui; }
