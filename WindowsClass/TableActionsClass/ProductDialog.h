#pragma once

#include <QDialog>
#include <QSqlQueryModel>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>

enum class ButtonNewTypes;

namespace Ui { class ProductDialog; }

class ProductDialog : public QDialog {
  Q_OBJECT

public:
  explicit ProductDialog(QWidget *parent = nullptr);
  ~ProductDialog();

private slots:
  void on_buttonBox_accepted();

  void on_newTypeButton_clicked();

  void on_newCountryButton_clicked();

  void on_newSupplierButton_clicked();

private:
  // Обновляет данные в указанном ComboBox
  //  в зависимости от значения ButtonNewTypes.
  // (От типа выбранной кнопки "Новый".)
  void updateComboBox(const ButtonNewTypes type);

  // Обновляет данные во всех ComboBox на форме
  void updateAllComboBoxes();

  // Вставляет новый элемент в ComboBox
  //  в зависимости от значения ButtonNewTypes.
  // (От типа выбранной кнопки "Новый".)
  void insertComboBoxItem(const ButtonNewTypes type);

  // Функция, возвращающая имя таблицы БД
  //  в зависимости от значения ButtonNewTypes.
  // (От типа выбранной кнопки "Новый".)
  QString getTableName(ButtonNewTypes type);

  // Функция, определяющая изменяемые элементы на форме
  //  в зависимости от значения ButtonNewTypes.
  // (От типа выбранной кнопки "Новый".)
  void setUpdatingElements(ButtonNewTypes type);


  Ui::ProductDialog *ui;
  QSqlQueryModel *typeModel;
  QSqlQueryModel *countryModel;
  QSqlQueryModel *supplierModel;

  // Указатель на модель, которую необходимо обновлять.
  QSqlQueryModel *updatingModel;

  // Указатель на ComboBox на форме, который необходимо обновлять.
  QComboBox *updatingComboBox;
};
