#include "WindowsClass/SignInWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  qApp->setStyle(QStyleFactory::create("Fusion"));

  signInWindow w;
  w.show();

  return a.exec();
}
