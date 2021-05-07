#include "ManifestForm.h"
#include "ui_ManifestForm.h"
#include "Enums.h"

ManifestForm::ManifestForm(TableTypes type,
                           uint rowCount,
                           QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ManifestForm),
  tt(type)
{
  ui->setupUi(this);

  // Создаём модель таблицы для накладной.
  model = new QSqlTableModel();

  // В зависимости от типа накладной, выполняем запрос.
  if (tt == TableTypes::WAREHOUSE) {
    model->setQuery("SELECT PRODUCTS.name, "
                    "WAREHOUSE.quantity, "
                    "WAREHOUSE.price, "
                    "WAREHOUSE.quantity * WAREHOUSE.price "
                    "FROM WAREHOUSE "
                    "INNER JOIN PRODUCTS ON "
                    "PRODUCTS.idProduct = WAREHOUSE.idProduct "
                    "ORDER BY idPosition DESC "
                    "LIMIT " + QString::number(rowCount) + ";");
  }
  else if (tt == TableTypes::SHIPMENTS) {
    model->setQuery("SELECT PRODUCTS.name, "
                    "SHIPMENTS.quantity, "
                    "SHIPMENTS.price, "
                    "SHIPMENTS.quantity * SHIPMENTS.price "
                    "FROM SHIPMENTS "
                    "INNER JOIN PRODUCTS ON "
                    "PRODUCTS.idProduct = SHIPMENTS.idProduct "
                    "ORDER BY idPosition DESC "
                    "LIMIT " + QString::number(rowCount) + ";");
  }
  else {
    QMessageBox::critical(this,
                          "Критическая ошибка!",
                          "Такого типа таблицы не существует.\n"
                          "Ошибка при установке модели для таблицы.");
    return;
  }

  // Заполяем заголовки колонок таблицы.
  QStringList horizontalHeaders;
  horizontalHeaders << "НАИМЕНОВАНИЕ"
                    << "Кол-во"
                    << "Цена"
                    << "Сумма";

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

  // Запрещаем редактирование ячеек таблицы.
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  // Запрещаем выделение в таблице.
  ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);

  // Задаём режим изменения размеров для секции горизонтальных заголовков.
  ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

  // Сохранение формы в PNG.
  saveFormAsPNG();
}

ManifestForm::~ManifestForm()
{
  delete model;
  delete ui;
}

void ManifestForm::saveFormAsPNG() {

  // Вызываем FileDialog на сохранение
  //  и получаем путь, куда сохранить картинку накладной.
  QString fileName =
      QFileDialog::getSaveFileName(this,
                                   "Выберите путь сохранения накладной",
                                   QDir::currentPath(),
                                   "*.png");

  if (fileName.isEmpty())
    return;

  // Захватываем в Pixmap всё, что есть на форме.
  QPixmap pixmap = this->grab();

  // Сохраняем картину по указанному пути.
  pixmap.save(fileName);
}
