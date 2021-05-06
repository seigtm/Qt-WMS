#pragma once

#include <QWidget>

class signUpWindow;
class MainWindow;
// class LogPassValidator;
// class dbWorks;

namespace Ui { class signInWindow; }

class signInWindow : public QWidget {
  Q_OBJECT

public:
  explicit signInWindow(QWidget *parent = nullptr);
  ~signInWindow();

private slots:
  void on_signUpButton_clicked();

  void on_signInButton_clicked();

private:
  Ui::signInWindow *ui;
  signUpWindow *SignUpWindow;
  MainWindow *mainWindow;
};

