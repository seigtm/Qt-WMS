#pragma once

#include <QWidget>
#include <QtSql>
#include <QSqlTableModel>
#include <QTableView>
#include <QMessageBox>

class ProductDialog;
// class RecordDialog;
class ManifestForm;
enum class TableTypes;

namespace Ui { class TableWindow; }

class TableWindow : public QWidget {
  Q_OBJECT

public:
  explicit TableWindow(TableTypes tableType, QWidget *parent = nullptr);
  ~TableWindow();

private slots:
  void on_addButton_clicked();

  void on_deleteButton_clicked();

  void on_manifestButton_clicked();

private:
  // Функция, возвращающая имя таблицы БД
  //  в зависимости от значения поля TableTypes tt.
  // (От типа таблицы, с которой мы работаем на форме.)
  QString getTableName();

  // Функция, возвращающая имя ключевого поля таблицы БД
  //  в зависимости от значения поля TableTypes tt.
  // (От типа таблицы, с которой мы работаем на форме.)
  QString getKeyFieldName();

  Ui::TableWindow *ui;

  // Редактируемая модель данных для таблицы БД.
  QSqlRelationalTableModel *model;

  // Тип таблицы, с которой необходимо работать в данной форме.
  TableTypes tt;

  // Диалоговое окно добавления нового продукта.
  ProductDialog *addProductDialog;

  // Диалоговое окно добавления новой записи о поставке/отпуске товара.
  // RecordDialog *addRecordDialog;

  // Форма накладной.
  ManifestForm *manifestForm;
};
