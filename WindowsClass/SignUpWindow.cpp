#include "SignUpWindow.h"
#include "ui_SignUpWindow.h"
#include "OptionsWindow.h"
#include "ValidatorClass/LogPassValidator.h"
#include "DatabaseClass/DatabaseWorks.h"


signUpWindow::signUpWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::signUpWindow)
{
  ui->setupUi(this);
  // Устанавливаем валидаторы на поля ввода логина и паролей.
  LogPassValidator::instance().setLineEditValidator(ui->loginLineEdit);
  LogPassValidator::instance().setLineEditValidator(ui->passwordLineEdit);
  LogPassValidator::instance().setLineEditValidator(ui->confirmationLineEdit);
}

signUpWindow::~signUpWindow()
{
  delete ui;
}

void signUpWindow::on_returnButton_clicked() {
  hide();
  emit showSignInWindow();
}

void signUpWindow::on_signUpButton_clicked() {
  QString login = ui->loginLineEdit->text();
  QString password = ui->passwordLineEdit->text();
  QString confirmation = ui->confirmationLineEdit->text();

  if (login.isEmpty() || password.isEmpty() || confirmation.isEmpty()) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Проверьте поля ввода!\n"
                          "Одно или более полей - пусты.");
    return;
  }

  // Проверяем поля ввода на валидность.
  bool isLoginValid = LogPassValidator::instance().isValid(login);
  bool isPasswordValid = LogPassValidator::instance().isValid(password);
  bool isConfirmationValid = LogPassValidator::instance().isValid(confirmation);

  if (!(isLoginValid && isPasswordValid && isConfirmationValid)) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Проверьте правильность введённых данных!\n"
                          "Логин и пароль должен состоять из 5-15 символов (буквы латиницы или цифры)");
    return;
  }

  if (password != confirmation) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Пароль и подтверждение пароля не совпадают!");
    return;
  }

  // Если пользователь с такими логином уже существует в БД.
  if (dbWorks::instance().userExists(login)) {
    QMessageBox::information(this,
                             "Внимание!",
                             "Пользователь с таким логином уже существует.");
    return;
  }

  // Если регистрация пользователя в БД не прошла успешно.
  if (!dbWorks::instance().userRegistration(login, password)) {
    QMessageBox::warning(this,
                         "Что-то пошло не так...",
                         "Не удалось создать ваш аккаунт. Повторите попытку позже.");
    return;
  }

  // Скрываем окно регистрации и открываем главное окно программы.
  hide();
  mainWindow = new MainWindow;
  mainWindow->show();
}
