#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qasciiart.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
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

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
