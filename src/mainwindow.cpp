#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "about.h"
#include "exportdialog.h"
#include "importdialog.h"

#include <QButtonGroup>

#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  Q_INIT_RESOURCE(resource);

  // connect the tool handlers
  connect(ui->actionToolArrow, &QAction::triggered, ui->customArea,
          &QAsciiArt::ActivateToolArrow);
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
  connect(ui->actionToolText, &QAction::triggered, ui->customArea,
          &QAsciiArt::ActivateToolText);

  connect(ui->actionRedo, &QAction::triggered, ui->customArea,
          &QAsciiArt::Redo);
  connect(ui->actionUndo, &QAction::triggered, ui->customArea,
          &QAsciiArt::Undo);

  connect(ui->customArea, &QAsciiArt::RedoAvail,
          [this](bool state) { ui->actionRedo->setEnabled(state); });
  connect(ui->customArea, &QAsciiArt::UndoAvail,
          [this](bool state) { ui->actionUndo->setEnabled(state); });

  // scrollarea
  ui->scrollArea->setBackgroundRole(QPalette::Dark);
  connect(ui->customArea, &QAsciiArt::DataAreaChanged,
          [this](QPoint p) { ui->scrollArea->ensureVisible(p.x(), p.y()); });

  ui->actionRedo->setEnabled(false);
  ui->actionUndo->setEnabled(false);

  // connect the tool buttons to trigger the actions
  connect(ui->btnToolArrow, &QPushButton::clicked, ui->actionToolArrow,
          &QAction::trigger);
  connect(ui->btnToolClass, &QPushButton::clicked, ui->actionToolClass,
          &QAction::trigger);
  connect(ui->btnToolErase, &QPushButton::clicked, ui->actionToolErase,
          &QAction::trigger);
  connect(ui->btnToolFreehand, &QPushButton::clicked, ui->actionToolFreehand,
          &QAction::trigger);
  connect(ui->btnToolMove, &QPushButton::clicked, ui->actionToolMove,
          &QAction::trigger);
  connect(ui->btnToolRectangle, &QPushButton::clicked, ui->actionToolRectangle,
          &QAction::trigger);
  connect(ui->btnToolText, &QPushButton::clicked, ui->actionToolText,
          &QAction::trigger);

  // custom widget to us
  connect(ui->customArea, &QAsciiArt::ActiveTool, this,
          &MainWindow::OnToolChanged);

  // connect the new/export/import actions
  connect(ui->actionNew, &QAction::triggered, this, &MainWindow::OnNew);
  connect(ui->actionImport, &QAction::triggered, this, &MainWindow::OnImport);
  connect(ui->actionExport, &QAction::triggered, this, &MainWindow::OnExport);

  connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::OnAbout);

  // default
  ui->btnToolMove->setChecked(true);

  // icons
#if 0
    ui->btnToolArrow->setIcon(*new QIcon(":/ico/Arrow.png"));
    ui->btnToolClass->setIcon(*new QIcon(":/ico/class.png"));
    ui->btnToolErase->setIcon(*new QIcon(":/ico/Erase.png"));
    ui->btnToolFreehand->setIcon(*new QIcon(":/ico/Freehand.png"));
    ui->btnToolMove->setIcon(*new QIcon(":/ico/Move.png"));
    ui->btnToolRectangle->setIcon(*new QIcon(":/ico/rectangle.png"));
    ui->btnToolText->setIcon(*new QIcon(":/ico/Text.png"));

    ui->actionNew->setIcon(*new QIcon(":/ico/New.png"));
    ui->actionImport->setIcon(*new QIcon(":/ico/Import.png"));
    ui->actionExport->setIcon(*new QIcon(":/ico/Export.png"));
    ui->actionAbout->setIcon(*new QIcon(":/ico/About.png"));

    ui->actionUndo->setIcon(*new QIcon(":/ico/Undo.png"));
    ui->actionRedo->setIcon(*new QIcon(":/ico/Redo.png"));




    QSize ico_size(50,50);
    ui->btnToolArrow->setIconSize(ico_size);
    ui->btnToolClass->setIconSize(ico_size);
    ui->btnToolErase->setIconSize(ico_size);
    ui->btnToolFreehand->setIconSize(ico_size);
    ui->btnToolMove->setIconSize(ico_size);
    ui->btnToolRectangle->setIconSize(ico_size);
    ui->btnToolText->setIconSize(ico_size);
#endif
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::OnToolChanged(Tool tool) {
  ui->btnToolArrow->setChecked(false);
  ui->btnToolClass->setChecked(false);
  ui->btnToolErase->setChecked(false);
  ui->btnToolFreehand->setChecked(false);
  ui->btnToolMove->setChecked(false);
  ui->btnToolRectangle->setChecked(false);
  ui->btnToolText->setChecked(false);

  switch (tool) {
  case Tool::Arrow:
    ui->actionToolArrow->setChecked(true);
    ui->btnToolArrow->setChecked(true);
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
  case Tool::Text:
    ui->actionToolText->setChecked(true);
    ui->btnToolText->setChecked(true);
    break;
  }
}

void MainWindow::OnNew() { ui->customArea->Clear(); }
void MainWindow::OnExport() {
  QString exported = ui->customArea->Export();
  ExportDialog dlg(exported, this);
  dlg.exec();
}
void MainWindow::OnImport() {
  ImportDialog dlg(this);
  connect(&dlg, &ImportDialog::ImportedData,
          [this](QString data) { ui->customArea->Import(data); });
  dlg.exec();
}

void MainWindow::OnAbout() {
  About dlg(this);
  dlg.exec();
}
