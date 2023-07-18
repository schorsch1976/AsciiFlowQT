#include "exportdialog.h"
#include "ui_exportdialog.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>

ExportDialog::ExportDialog(const QString &data, QWidget *parent)
	: QDialog(parent), ui(new Ui::ExportDialog), m_data(data)
{
	ui->setupUi(this);

	ui->textEditExport->setText(data);
	ui->textEditExport->selectAll();

	connect(ui->btnSaveToFile, &QPushButton::clicked, this,
			&ExportDialog::OnSaveToFile);
}

ExportDialog::~ExportDialog() { delete ui; }

void ExportDialog::OnSaveToFile()
{
	QString documents =
		QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	QString filename = QFileDialog::getSaveFileName(
		this, tr("Save File"), documents, tr("Text files (*.txt)"));

	if (filename.size() == 0)
	{
		return;
	}

	// save to file
	QFile file(filename, this);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox box(QMessageBox::Warning, tr("Error"),
						tr("Could not open file."), QMessageBox::Ok);
		box.exec();
		return;
	}
	QTextStream(&file) << m_data.toUtf8();
	file.close();

	// end this dialog
	this->close();
}
