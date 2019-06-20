#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QString data, QWidget *parent = nullptr);
    ~ExportDialog();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
