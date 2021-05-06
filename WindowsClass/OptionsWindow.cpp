#include "OptionsWindow.h"
#include "ui_OptionsWindow.h"
#include "DatabaseClass/DatabaseWorks.h"
#include "WindowsClass/TableWindow.h"
#include "Enums.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  ui->label->setText("Добро пожаловать, " + dbWorks::instance().getUserLogin() + "!");
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_warehouseButton_clicked() {
  tableWindow = new TableWindow(TableTypes::WAREHOUSE);
  tableWindow->show();
}

void MainWindow::on_shipmentsButton_clicked() {
  tableWindow = new TableWindow(TableTypes::SHIPMENTS);
  tableWindow->show();
}

void MainWindow::on_stockButton_clicked() {
    tableWindow = new TableWindow(TableTypes::STOCK);
    tableWindow->show();
}
