#include "SignInWindow.h"
#include "ui_SignInWindow.h"
#include "SignUpWindow.h"
#include "OptionsWindow.h"
#include "ValidatorClass/LogPassValidator.h"
#include "DatabaseClass/DatabaseWorks.h"

signInWindow::signInWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::signInWindow)
{
  ui->setupUi(this);

  // Организовываем связь сигналом между окнами регистрации и авторизации.
  // Для возвращения из окна регистрации в окно авторизации.
  SignUpWindow = new signUpWindow;
  connect(SignUpWindow, &signUpWindow::showSignInWindow, this, &signInWindow::show);

  // Устанавливаем валидаторы на поля ввода логина и пароля.
  LogPassValidator::instance().setLineEditValidator(ui->loginLineEdit);
  LogPassValidator::instance().setLineEditValidator(ui->passwordLineEdit);
}

signInWindow::~signInWindow()
{
  // Отвязываем сигнал при вызове деструктора signInWindow.
  disconnect(SignUpWindow, &signUpWindow::showSignInWindow, this, &signInWindow::show);
  delete ui;
}

void signInWindow::on_signUpButton_clicked() {
  hide();
  SignUpWindow->show();
}

void signInWindow::on_signInButton_clicked() {
  QString login = ui->loginLineEdit->text();
  QString password = ui->passwordLineEdit->text();

  if (login.isEmpty() || password.isEmpty()) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Проверьте поля ввода!\n"
                          "Одно или оба поля - пусты.");
    return;
  }

  // Проверяем поля ввода на валидность.
  bool isLoginValid = LogPassValidator::instance().isValid(login);
  bool isPasswordValid = LogPassValidator::instance().isValid(password);
  if (!(isLoginValid && isPasswordValid)) {
    QMessageBox::critical(this,
                          "Внимание!",
                          "Проверьте правильность введённых данных!\n"
                          "Логин и пароль должен состоять из 5-15 символов (буквы латиницы или цифры)");
    return;
  }


  // Если пользователь с такими логином и паролем не существует в БД.
  if (!(dbWorks::instance().userExists(login, password))) {
    QMessageBox::critical(this,
                          "Неудачная авторизация!",
                          "Проверьте правильность введённых данных.");
    return;
  }

  // Устанавливаем текущего пользователя.
  dbWorks::instance().setCurrentUser(login);

  // Скрываем окно авторизации и открываем главное окно программы.
  hide();
  mainWindow = new MainWindow;
  mainWindow->show();
}
