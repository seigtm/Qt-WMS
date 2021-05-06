#pragma once

#include <QWidget>

class MainWindow;
// class dbWorks;

namespace Ui { class signUpWindow; }

class signUpWindow : public QWidget {
  Q_OBJECT

public:
  explicit signUpWindow(QWidget *parent = nullptr);
  ~signUpWindow();

signals:
  void showSignInWindow();

private slots:
  void on_returnButton_clicked();

  void on_signUpButton_clicked();

private:
  Ui::signUpWindow *ui;
  MainWindow *mainWindow;
};
