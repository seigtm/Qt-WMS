#pragma once

#include <QDialog>
#include <QMessageBox>

enum class TableTypes;

namespace Ui { class RecordDialog; }

class RecordDialog : public QDialog {
  Q_OBJECT

public:
  explicit RecordDialog(TableTypes tableType, QWidget *parent = nullptr);
  ~RecordDialog();

private slots:
  void on_buttonBox_accepted();

private:
  Ui::RecordDialog *ui;

  // В зависимости от таблицы
  //  меняется логика работы диалогового
  //  окна добавления новой записи.
  // tt - тип таблицы, с которой
  //  необходимо работать в данной форме.
  TableTypes tt;
};
