#include "TableWindow.h"
#include "ui_TableWindow.h"
#include "WindowsClass/TableActionsClass/ProductDialog.h"
// #include "WindowsClass/TableActionsClass/RecordDialog.h"
#include "WindowsClass/TableActionsClass/ManifestForm.h"
#include "DatabaseClass/DatabaseWorks.h"
#include "Enums.h"

TableWindow::TableWindow(TableTypes tableType, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TableWindow),
  tt(tableType)
{
  ui->setupUi(this);

  // Создаём модель таблицы.
  model = new QSqlRelationalTableModel();

  // Выбираем из БД таблицу как источник данных
  //  в зависимости от выбранного пункта меню.
  if (tt == TableTypes::WAREHOUSE)
    model->setTable("WAREHOUSE");
  else if (tt == TableTypes::SHIPMENTS)
    model->setTable("SHIPMENTS");
  else if (tt == TableTypes::STOCK)
    model->setTable("PRODUCTS");
  else {
    QMessageBox::critical(this,
                          "Критическая ошибка!",
                          "Такого типа таблицы не существует.\n"
                          "Ошибка при установке модели для таблицы.");
    return;
  }


  // Если не удалось заполнить модель данными из указанной таблицы.
  if (!model->select()) {
    // Выводим сообщение об ошибке и прекращаем конструирование окна.
    QMessageBox::critical(this,
                          "Фатальная ошибка!",
                          "Не удалось выбрать таблицу как источник данных.\n"
                          "Возможно, нарушена структура базы данных.\n"
                          "Обратитесь к оператору БД вашей фирмы.");
    return;
  }

  // Уточняем связи с другими таблицами и выводимые поля.
  if (tt == TableTypes::WAREHOUSE) {
    model->setRelation(1, QSqlRelation("PRODUCTS", "idProduct", "name"));
    model->setRelation(2, QSqlRelation("USERS",    "idUser",    "login"));
  }
  else if (tt == TableTypes::SHIPMENTS) {
    model->setRelation(1, QSqlRelation("PRODUCTS", "idProduct", "name"));
    model->setRelation(2, QSqlRelation("USERS",    "idUser",    "login"));
    model->setRelation(3, QSqlRelation("BUYERS",   "idBuyer",   "name"));
  }
  else {
    model->setRelation(1, QSqlRelation("PRODUCT_TYPES", "idProductType", "name"));
    model->setRelation(2, QSqlRelation("COUNTRIES",     "idCountry",     "name"));
    model->setRelation(3, QSqlRelation("SUPPLIERS",     "idSupplier",    "name"));
  }

  // Заполяем заголовки колонок таблицы.
  QStringList horizontalHeaders;
  if (tt == TableTypes::WAREHOUSE) {
    horizontalHeaders << "id"
                      << "Товар"
                      << "Принял"
                      << "Цена"
                      << "Кол-во"
                      << "Дата";
  }
  else if (tt == TableTypes::SHIPMENTS){
    horizontalHeaders << "id"
                      << "Товар"
                      << "Отправил"
                      << "Покупатель"
                      << "Цена"
                      << "Кол-во"
                      << "Дата";
  }
  else {
    horizontalHeaders << "id"
                      << "Тип"
                      << "Страна"
                      << "Поставщик"
                      << "Название"
                      << "Кол-во";
  }

  // Заполяем заголовки колонок в цикле из списка.
  for (int currentColumn = 0;
       currentColumn < model->columnCount();
       ++currentColumn) {
    model->setHeaderData(currentColumn,
                         Qt::Horizontal,
                         horizontalHeaders.at(currentColumn));
  }

  // Устанавливаем модель для табличного представления на форме.
  ui->tableView->setModel(model);

  // Включаем возможность сортировки.
  ui->tableView->setSortingEnabled(true);

  // Скрываем вертикальные заголовки представления.
  ui->tableView->verticalHeader()->hide();

  // Запрещаем редактирование ячеек таблицы.
  //   ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Устанавливаем тип выделения в таблице на "Выделять один ряд".
  //   ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  //   ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

  // Устанавливаем делегат для табличного представления.
  ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));

  // Задаём режим изменения размеров для секции горизонтальных заголовков.
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

TableWindow::~TableWindow()
{
  delete model;
  delete ui;
}

void TableWindow::on_addButton_clicked() {

  // Открываем форму добавления нового товара, если tt = STOCK.
  if (tt == TableTypes::STOCK) {
    addProductDialog = new ProductDialog();

    // Обновить табличную модель после закрытия диалогового окна.
    if (addProductDialog->exec()) {
      model->select();
    }
  }

  /*
  // В ином случае, открываем форму добавления записи о поставке/отпуске товара.
  else {
    addRecordDialog = new RecordDialog(tt);

    // Обновить табличную модель после закрытия диалогового окна.
    if (addRecordDialog->exec()) {
      model->select();
    }
  }
  */

  else {
    model->select();
    model->insertRow(0);
  }
}

void TableWindow::on_deleteButton_clicked() {

  // Получаем id выбранной записи в таблице.
  QModelIndex currentIndex = ui->tableView->currentIndex();
  int id = currentIndex.sibling(currentIndex.row(), 0).data().toInt();

  // qDebug() << "id = " << id;

  // Выводим диалоговое окно для подтверждения операции.
  int ret = QMessageBox::question(this,
                                  "Вы уверены?",
                                  "Удалить запись с id = " + QString::number(id) + "?");

  // Если пользователь нажал "Нет в диалоговом окне".
  if (ret != QMessageBox::Yes)
    return;


  // Если записи с таким id по какой-то причине не существует.
  bool isRecordExists = dbWorks::instance().exists(getTableName(),
                                                   getKeyFieldName(),
                                                   QString::number(id));
  if (!isRecordExists) {
    QMessageBox::warning(0,
                         "Внимание!",
                         "Записи с таким id не существует");
    return;
  }

  // qDebug() << "isRecordExists = " << isRecordExists;

  // Если запись в таблице с заданным id не удалилась.
  bool isDeleted = dbWorks::instance().del(getTableName(),
                                           getKeyFieldName(),
                                           id);
  if (!isDeleted) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Не удалось удалить запись!\n"
                          "Проверьте ваше соединение с БД и повторите позже.");
  }

  // qDebug() << "isDeleted = " << isDeleted;

  // Обновляем данные в таблице.
  model->select();
}

QString TableWindow::getTableName() {
  switch (tt) {
    case TableTypes::WAREHOUSE:
      return "WAREHOUSE";
      break;

    case TableTypes::SHIPMENTS:
      return "SHIPMENTS";
      break;

    case TableTypes::STOCK:
      return "PRODUCTS";
      break;

    default:
      return "INVALID";
      break;
  }
}

QString TableWindow::getKeyFieldName() {
  switch (tt) {
    case TableTypes::WAREHOUSE:
      return "idPosition";
      break;

    case TableTypes::SHIPMENTS:
      return "idShipment";
      break;

    case TableTypes::STOCK:
      return "idProduct";
      break;

    default:
      return "INVALID";
      break;
  }
}

void TableWindow::on_manifestButton_clicked() {

  // Булева переменная.
  // Была ли нажата клавиша "ОК" в InputDialog.
  bool ok = false;

  // Кол-во записей, которое нужно отобразить в накладной.
  int rowCount =
      QInputDialog::getInt(this,
                           "Введите число",
                           "Введите количество записей",
                           1,
                           1,
                           INT_MAX,
                           1,
                           &ok,
                           Qt::MSWindowsFixedSizeDialogHint);

  if (!ok)
    return;

  manifestForm = new ManifestForm(tt, rowCount);
  manifestForm->show();
}
