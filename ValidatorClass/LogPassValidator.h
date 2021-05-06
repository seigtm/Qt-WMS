#pragma once
#include <QValidator>
#include <QLineEdit>

class LogPassValidator {

public:
  // Singleton для получения инстанса класса валидатора.
  static LogPassValidator& instance();

  // Устанавливает валидатор для логина и пароля в переданный QLineEdit.
  void setLineEditValidator(QLineEdit* lineEdit);

  // Проверяет строку на валидность в соответствии с регулярным выражением для логинов и паролей.
  // Вернёт true, если Acceptable и false, если Intermediate или Invalid.
  bool isValid(QString str);


private:
  LogPassValidator();
  ~LogPassValidator();

  // Валидатор с регулярным выражением.
  QRegularExpressionValidator* lineEditValidator;
};
