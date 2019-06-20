#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui
{
class ImportDialog;
}

class ImportDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ImportDialog(QWidget *parent = nullptr);
	~ImportDialog();

signals:
	void ImportedData(QString data);

private:
	Ui::ImportDialog *ui;
};

#endif // IMPORTDIALOG_H
