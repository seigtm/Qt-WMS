#include "RecordDialog.h"
#include "ui_RecordDialog.h"
#include "Enums.h"

RecordDialog::RecordDialog(TableTypes tableType, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::RecordDialog),
  tt(tableType)
{
  ui->setupUi(this);

  // По умолчанию ставим в поле с датой текущую дату.
  ui->dateEdit->setDate(QDate::currentDate());

  // Если тип таблицы, с которым мы работаем - не отпуск товара.
  if (tt != TableTypes::SHIPMENTS) {

    // У прихода товара нет "покупателя".
    // По этой причине - скрываем эту информацию с формы.
    ui->buyerComboBox->hide();
    ui->buyerLabel->hide();
    ui->newBuyerButton->hide();
  }
}

RecordDialog::~RecordDialog()
{
  delete ui;
}

void RecordDialog::on_buttonBox_accepted() {
    if (ui->dateEdit->date() > QDate::currentDate()) {
      QMessageBox::warning(this,
                           "Внимание!",
                           "Не удалось сохранить запись.\n"
                           "Введённая вами дата превышает текущую.");
    }
}
