#pragma once

#include <QMainWindow>
#include <QMessageBox>

class TableWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  // Обработчик нажатия на кнопку "Приход товара".
  void on_warehouseButton_clicked();

  // Обработчик нажатия на кнопку "Отпуск товара".
  void on_shipmentsButton_clicked();

  // Обработчик нажатия на кнопку "Список товаров на складе".
  void on_stockButton_clicked();

private:
  Ui::MainWindow *ui;
  TableWindow *tableWindow;
};
