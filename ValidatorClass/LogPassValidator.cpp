#include "LogPassValidator.h"

LogPassValidator& LogPassValidator::instance() {
  static LogPassValidator validator;
  return validator;
}

void LogPassValidator::setLineEditValidator(QLineEdit* lineEdit) {
  lineEdit->setValidator(lineEditValidator);
}

bool LogPassValidator::isValid(QString str) {
  int pos{};
  return (lineEditValidator->validate(str, pos) == QValidator::State::Acceptable);
}

LogPassValidator::LogPassValidator() {
  lineEditValidator = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9]{5,15}"));
}

LogPassValidator::~LogPassValidator() {
  delete lineEditValidator;
}
