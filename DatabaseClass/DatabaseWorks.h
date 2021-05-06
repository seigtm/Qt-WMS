#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QDir>

class dbWorks {
  // TODO: Fix this path to database.
  const QString pathToDB = "C:/Users/seigt/OneDrive/Documents/GitHub/PRACTICE/SportLuxApp/Databases/SportLux.db";
  // const QString pathToDB = QDir::currentPath() + "/Databases/SportLux.db";

public:
  // Синглтон инстанс для БД.
  static dbWorks& instance();

  // Проверка на существование пользователя с данным login в БД.
  bool userExists(const QString login);

  // Проверка на существование пользователя с данными login и password в БД.
  bool userExists(const QString login, const QString password);

  // Проверка на существование записи в таблице.
  // table - название таблицы,
  // field - проверяемое поле,
  // value - его значение.
  bool exists(const QString table,
              const QString field,
              const QString value);

  // Вставка записи в таблицу.
  // table - название таблицы,
  // field - вставляемое поле,
  // value - его значение.
  bool insert(const QString table,
              const QString field,
              const QString value);

  // Удаление записи из таблицы.
  // table - название таблицы.
  // id    - название ключевого поля.
  // value - его значение.
  bool del(const QString table,
           const QString id,
           const size_t  value);

  // Получение ID записи в таблице.
  // table - таблица, в которой производится поиск.
  // key   - имя ключевого поля.
  // field - имя поля, с помощью которого осуществляется поиск.
  // value - значение этого поля.
  unsigned getID(const QString table,
                 const QString key,
                 const QString field,
                 const QString value);

  // Регистрация пользователя с данными login и password в БД.
  bool userRegistration(const QString login, const QString password);

  // Устанавливает currentUserID в соответствии с login пользователя.
  void setCurrentUser(const QString login);

  // Получение ID пользователя по его логину.
  unsigned getUserID(const QString login);

  // Получение логина текущего пользователя (по полю currentUserID).
  QString getUserLogin();

  // Деструктор.
  ~dbWorks();

private:
  // Приватный конструктор (так как синглтон).
  dbWorks();
  dbWorks(dbWorks const&) = delete;
  dbWorks& operator=(dbWorks const&) = delete;

  QSqlDatabase db;
  unsigned currentUserID;
};
