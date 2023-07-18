#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui
{
class ExportDialog;
}

class ExportDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ExportDialog(const QString &data, QWidget *parent = nullptr);
	~ExportDialog();

public slots:
	void OnSaveToFile();

private:
	Ui::ExportDialog *ui;
	QString m_data;
};

#endif // EXPORTDIALOG_H
