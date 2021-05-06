#include "ProductDialog.h"
#include "ui_ProductDialog.h"
#include "DatabaseClass/DatabaseWorks.h"
#include "Enums.h"


ProductDialog::ProductDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ProductDialog),
  typeModel(new QSqlQueryModel),
  countryModel(new QSqlQueryModel),
  supplierModel(new QSqlQueryModel),
  updatingModel(nullptr),
  updatingComboBox(nullptr)
{
  ui->setupUi(this);

  updateAllComboBoxes();
}

ProductDialog::~ProductDialog()
{
  delete typeModel;
  delete countryModel;
  delete supplierModel;
  delete updatingComboBox;
  delete updatingModel;
  delete ui;
}

void ProductDialog::on_buttonBox_accepted() {
  QString productName = ui->nameLineEdit->text();

  // Если пользователь не ввёл название товара.
  if (productName.isEmpty()) {
    QMessageBox::information(this,
                             "Внимание!",
                             "Вы не ввели название товара.");
    return;
  }

  // Если товар с таким названием уже существует в БД.
  if (dbWorks::instance().exists("PRODUCTS",
                                 "name",
                                 productName)) {
    QMessageBox::warning(this,
                         "Внимание!",
                         "Товар с таким названием уже существует!");
    return;
  }


  // Получаем id типа продукта, страны и поставщика
  //  в зависимости от выбранных значений в ComboBox.
  QString idProductType = QString::number(
        dbWorks::instance().getID("PRODUCT_TYPES",
                                  "idProductType",
                                  "name",
                                  ui->typeComboBox->currentText())
        );

  QString idCountry = QString::number(
        dbWorks::instance().getID("COUNTRIES",
                                  "idCountry",
                                  "name",
                                  ui->countryComboBox->currentText())
        );

  QString idSupplier = QString::number(
        dbWorks::instance().getID("SUPPLIERS",
                                  "idSupplier",
                                  "name",
                                  ui->supplierComboBox->currentText())
        );


  // Подготавливаем запрос на добавление товара в таблицу товаров.
  QSqlQuery q;
  q.prepare("INSERT INTO PRODUCTS "
            "(idProductType, "
            "idCountry, "
            "idSupplier, "
            "name, "
            "quantity) VALUES ("
            + idProductType + ", "
            + idCountry     + ", "
            + idSupplier    + ", "
            + ":productName, "
            + QString::number(0)
            + ");");
  q.bindValue(":productName", productName);

  // Если не получилось занести в таблицу товаров данные о новом товаре.
  if (!q.exec()) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Не удалось занести данные о новом товаре в БД!\n"
                          "Проверьте правильность введённых данных и ваше подключение к БД.");
  }
}

void ProductDialog::on_newTypeButton_clicked() {
  insertComboBoxItem(ButtonNewTypes::TYPE);
}

void ProductDialog::on_newCountryButton_clicked() {
  insertComboBoxItem(ButtonNewTypes::COUNTRY);
}

void ProductDialog::on_newSupplierButton_clicked() {
  insertComboBoxItem(ButtonNewTypes::SUPPLIER);
}

void ProductDialog::updateComboBox(const ButtonNewTypes type) {
  QSqlQuery q;

  // Определяем изменяемые элементы
  //  в зависимости от выбранной кнопки.
  setUpdatingElements(type);

  // Выполняем запрос в зависимости от выбранной кнопки "Новый".
  q.exec("SELECT name FROM " + getTableName(type));

  // Устанавливаем для модели данных результат выполнения запроса.
  updatingModel->setQuery(q);

  // Устанавливаем модель с данными в ComboBox.
  updatingComboBox->setModel(updatingModel);

  updatingComboBox = nullptr;
  updatingModel = nullptr;
}

void ProductDialog::updateAllComboBoxes() {
  updateComboBox(ButtonNewTypes::TYPE);
  updateComboBox(ButtonNewTypes::COUNTRY);
  updateComboBox(ButtonNewTypes::SUPPLIER);
}

void ProductDialog::insertComboBoxItem(const ButtonNewTypes type) {
  bool isOKClicked = false;
  QString newType = QInputDialog::getText(this,
                                          "Новое значение",
                                          "Значение:",
                                          QLineEdit::Normal,
                                          "Введите название",
                                          &isOKClicked,
                                          Qt::MSWindowsFixedSizeDialogHint);
  // Если нажата кнопка "Cancel".
  if (!isOKClicked)
    return;

  // В зависимости от выбранной кнопки "Новый",
  //  выбираем таблицу, в которую будем вставлять значения.
  QString table = getTableName(type);

  // Проверяем, существует ли уже в БД такой тип.
  if (dbWorks::instance().exists(table,
                                 "name",
                                 newType)) {
    QMessageBox::information(this,
                             "Внимание!",
                             "Такой тип уже существует.");
    return;
  }

  // Вставляем запись о новом типе продуктов в таблицу БД.
  if (!dbWorks::instance().insert(table,
                                  "name",
                                  newType)) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Не удалось добавить новый тип в БД.\n"
                          "Проверьте ваше подключение к БД и введённый тип.");
    return;
  }

  updateComboBox(type);
}

QString ProductDialog::getTableName(ButtonNewTypes type) {
  switch (type) {
    case ButtonNewTypes::TYPE:
      return "PRODUCT_TYPES";
      break;

    case ButtonNewTypes::COUNTRY:
      return "COUNTRIES";
      break;

    case ButtonNewTypes::SUPPLIER:
      return "SUPPLIERS";
      break;

    default:
      return "INVALID";
      break;
  }
}

void ProductDialog::setUpdatingElements(ButtonNewTypes type) {
  // Определяем указатели в зависимости от выбранной кнопки.
  if (type == ButtonNewTypes::COUNTRY) {
    updatingModel = countryModel;
    updatingComboBox = ui->countryComboBox;
  }
  else if (type == ButtonNewTypes::SUPPLIER) {
    updatingModel = supplierModel;
    updatingComboBox = ui->supplierComboBox;
  }
  else {
    updatingModel = typeModel;
    updatingComboBox = ui->typeComboBox;
  }
}
