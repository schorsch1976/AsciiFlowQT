#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "exportdialog.h"
#include "importdialog.h"

#include <QMessageBox>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// connect the tool handlers
	connect(ui->actionToolArrow, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolArrow);
	connect(ui->actionToolLine, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolLine);
	connect(ui->actionToolClass, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolClass);
	connect(ui->actionToolErase, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolErase);
	connect(ui->actionToolFreehand, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolFreehand);
	connect(ui->actionToolMove, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolMove);
	connect(ui->actionToolRectangle, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolRectangle);
	connect(ui->actionToolResize, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolResize);
	connect(ui->actionToolText, &QAction::triggered, ui->customArea,
			&QAsciiArt::ActivateToolText);

	connect(ui->actionRedo, &QAction::triggered, ui->customArea,
			&QAsciiArt::Redo);
	connect(ui->actionUndo, &QAction::triggered, ui->customArea,
			&QAsciiArt::Undo);

	connect(ui->customArea, &QAsciiArt::RedoAvail,
			[this](bool state) { ui->btnRedo->setEnabled(state); });
	connect(ui->customArea, &QAsciiArt::UndoAvail,
			[this](bool state) { ui->btnUndo->setEnabled(state); });

	// scrollarea
	ui->scrollArea->setBackgroundRole(QPalette::Dark);
	ui->scrollArea->setWidgetResizable(true);
	connect(ui->customArea, &QAsciiArt::DataAreaChanged,
			[this](QPoint p) { ui->scrollArea->ensureVisible(p.x(), p.y()); });

	ui->btnRedo->setEnabled(false);
	ui->btnUndo->setEnabled(false);

	// connect the tool buttons to trigger the actions
	connect(ui->btnToolArrow, &QToolButton::clicked, ui->actionToolArrow,
			&QAction::trigger);
	connect(ui->btnToolLine, &QToolButton::clicked, ui->actionToolLine,
			&QAction::trigger);
	connect(ui->btnToolClass, &QToolButton::clicked, ui->actionToolClass,
			&QAction::trigger);
	connect(ui->btnToolErase, &QToolButton::clicked, ui->actionToolErase,
			&QAction::trigger);
	connect(ui->btnToolFreehand, &QToolButton::clicked, ui->actionToolFreehand,
			&QAction::trigger);
	connect(ui->btnToolMove, &QToolButton::clicked, ui->actionToolMove,
			&QAction::trigger);
	connect(ui->btnToolRectangle, &QToolButton::clicked,
			ui->actionToolRectangle, &QAction::trigger);
	connect(ui->btnToolResize, &QToolButton::clicked, ui->actionToolResize,
			&QAction::trigger);
	connect(ui->btnToolText, &QToolButton::clicked, ui->actionToolText,
			&QAction::trigger);

	// Connect the toolbar buttons to the actions
	connect(ui->btnNew, &QToolButton::clicked, ui->actionNew,
			&QAction::trigger);
	connect(ui->btnImport, &QToolButton::clicked, ui->actionImport,
			&QAction::trigger);
	connect(ui->btnExport, &QToolButton::clicked, ui->actionExport,
			&QAction::trigger);
	connect(ui->btnUndo, &QToolButton::clicked, ui->actionUndo,
			&QAction::trigger);
	connect(ui->btnRedo, &QToolButton::clicked, ui->actionRedo,
			&QAction::trigger);
	connect(ui->btnAbout, &QToolButton::clicked, ui->actionAbout,
			&QAction::trigger);

	connect(ui->btnSettings, &QToolButton::clicked, [this]() {
		ChangePreferencesDialog dlg(m_preferences, this);
		dlg.exec();
		this->ApplySettings();
		ChangePreferencesDialog::SavePreferences(m_preferences);
	});

	// custom widget to us
	connect(ui->customArea, &QAsciiArt::ActiveTool, this,
			&MainWindow::OnToolChanged);

	// connect the new/export/import actions
	connect(ui->actionNew, &QAction::triggered, this, &MainWindow::OnNew);
	connect(ui->actionImport, &QAction::triggered, this, &MainWindow::OnImport);
	connect(ui->actionExport, &QAction::triggered, this, &MainWindow::OnExport);

	connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);

	// close
	connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(OnClose()));

	// default Tool
	ui->btnToolLine->setChecked(true);

	// load the settings and apply then
	m_preferences = ChangePreferencesDialog::LoadPreferences();
	ApplySettings();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::OnToolChanged(Tool tool)
{
	ui->btnToolArrow->setChecked(false);
	ui->btnToolLine->setChecked(false);
	ui->btnToolClass->setChecked(false);
	ui->btnToolErase->setChecked(false);
	ui->btnToolFreehand->setChecked(false);
	ui->btnToolMove->setChecked(false);
	ui->btnToolRectangle->setChecked(false);
	ui->btnToolResize->setChecked(false);
	ui->btnToolText->setChecked(false);

	switch (tool)
	{
		case Tool::Arrow:
			ui->actionToolArrow->setChecked(true);
			ui->btnToolArrow->setChecked(true);
			break;
		case Tool::Line:
			ui->actionToolLine->setChecked(true);
			ui->btnToolLine->setChecked(true);
			break;
		case Tool::Class:
			ui->actionToolClass->setChecked(true);
			ui->btnToolClass->setChecked(true);
			break;
		case Tool::Erase:
			ui->actionToolErase->setChecked(true);
			ui->btnToolErase->setChecked(true);
			break;
		case Tool::Freehand:
			ui->actionToolFreehand->setChecked(true);
			ui->btnToolFreehand->setChecked(true);
			break;
		case Tool::Move:
			ui->actionToolMove->setChecked(true);
			ui->btnToolMove->setChecked(true);
			break;
		case Tool::Rectangle:
			ui->actionToolRectangle->setChecked(true);
			ui->btnToolRectangle->setChecked(true);
			break;
		case Tool::Resize:
			ui->actionToolResize->setChecked(true);
			ui->btnToolResize->setChecked(true);
			break;
		case Tool::Text:
			ui->actionToolText->setChecked(true);
			ui->btnToolText->setChecked(true);
			break;
	}
}

void MainWindow::OnNew()
{
	QString current_data = ui->customArea->Export();

	if (current_data.size())
	{
		QMessageBox box(QMessageBox::Question, tr("New"),
						tr("New would destroy current data.\n\nContinue?"),
						QMessageBox::Yes | QMessageBox::No);
		if (box.exec() != QMessageBox::Yes)
		{
			return;
		};
	}

	ui->customArea->Clear();
}
void MainWindow::OnExport()
{
	QString exported = ui->customArea->Export();
	ExportDialog dlg(exported, this);
	dlg.exec();
}
void MainWindow::OnImport()
{
	QString current_data = ui->customArea->Export();

	if (current_data.size())
	{
		QMessageBox box(QMessageBox::Question, tr("Import"),
						tr("Import would destroy current data.\n\nContinue?"),
						QMessageBox::Yes | QMessageBox::No);
		if (box.exec() != QMessageBox::Yes)
		{
			return;
		};
	}
	ImportDialog dlg(this);
	connect(&dlg, &ImportDialog::ImportedData,
			[this](QString data) { ui->customArea->Import(data); });
	dlg.exec();
}

void MainWindow::OnAbout()
{
	About dlg(this);
	dlg.exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QString current_data = ui->customArea->Export();
	if (current_data.size())
	{
		QMessageBox box(QMessageBox::Question, tr("Close"),
						tr("Close would destroy current data.\n\nContinue?"),
						QMessageBox::Yes | QMessageBox::No);

		if (box.exec() != QMessageBox::Yes)
		{
			event->ignore();
			return;
		};
	}
	event->accept();
}

void MainWindow::ApplySettings()
{
	std::vector<QToolButton *> buttons = {
		ui->btnNew,			 ui->btnRedo,		   ui->btnUndo,
		ui->btnAbout,		 ui->btnExport,		   ui->btnImport,
		ui->btnSettings,

		ui->btnToolLine,	 ui->btnToolMove,	   ui->btnToolText,
		ui->btnToolArrow,	 ui->btnToolClass,	   ui->btnToolErase,
		ui->btnToolFreehand, ui->btnToolRectangle, ui->btnToolResize};

	QSize size(m_preferences.icon_size, m_preferences.icon_size);
	Qt::ToolButtonStyle style =
		static_cast<Qt::ToolButtonStyle>(m_preferences.button_style);
	for (auto &c : buttons)
	{
		c->setIconSize(size);
		c->setToolButtonStyle(style);
	}
}
