#include "importdialog.h"
#include "ui_importdialog.h"

#include <QString>

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>

ImportDialog::ImportDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::ImportDialog)
{
	ui->setupUi(this);

	connect(ui->btnOk, &QPushButton::clicked,
			[this]()
			{
				emit ImportedData(ui->textEditImport->toPlainText());
				this->close();
			});
	connect(ui->btnCancel, &QPushButton::clicked, [this]() { this->close(); });

	connect(ui->btnImportFromFile, &QPushButton::clicked, this,
			&ImportDialog::OnImportFromFile);
}

ImportDialog::~ImportDialog() { delete ui; }

void ImportDialog::OnImportFromFile()
{
	QString documents =
		QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	QString filename = QFileDialog::getOpenFileName(
		this, tr("Load File"), documents, tr("Text files (*.txt)"));
	if (filename.size() == 0)
	{
		return;
	}

	// Load from file
	QFile file(filename, this);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox box(QMessageBox::Warning, tr("Error"),
						tr("Could not open file."), QMessageBox::Ok);
		box.exec();
		return;
	}

	QString raw = QTextStream(&file).readAll();

	// just use the latin1/print part
	QString tmp;
	int curr_x = 0;
	int max_x = 0;
	int max_y = 0;
	for (auto &&i : raw)
	{
		if ((i.toLatin1() && i.isPrint()) || i == QChar::Space)
		{
			tmp += i;
			++curr_x;
		}
		else if (i == '\n')
		{
			tmp += i;
			max_x = std::max(max_x, curr_x);
			curr_x = 0;
			++max_y;
		}
	}

	qDebug() << "Imported " << tmp.size() << " bytes.";
	qDebug() << "Imported " << max_x << " x " << max_y << " area.";
	ui->textEditImport->setText(tmp);
}
