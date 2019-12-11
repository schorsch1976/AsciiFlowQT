#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "changepreferencesdialog.h"
#include "qasciiart.h"

#include <QCloseEvent>
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void OnToolChanged(Tool tool);
	void OnNew();
	void OnExport();
	void OnImport();

	void OnAbout();

protected:
	void closeEvent(QCloseEvent *event) override;

private:
	void ApplySettings();
	Preferences m_preferences;

	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
