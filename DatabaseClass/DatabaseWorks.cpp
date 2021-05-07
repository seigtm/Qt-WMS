#include "DatabaseWorks.h"
#include "Enums.h"

dbWorks& dbWorks::instance() {
  static dbWorks dbw;
  return dbw;
}

bool dbWorks::userExists(const QString login) {
  QSqlQuery q;
  q.prepare("SELECT EXISTS (SELECT * FROM USERS WHERE login = :login);");
  q.bindValue(":login", login);
  q.exec();
  q.next();
  return q.value(0).toBool();
}

bool dbWorks::userExists(const QString login, const QString password) {
  QSqlQuery q;
  q.prepare("SELECT EXISTS (SELECT * FROM USERS WHERE login = :login AND password = :password);");
  q.bindValue(":login", login);
  q.bindValue(":password", password);
  q.exec();
  q.next();
  return q.value(0).toBool();
}

bool dbWorks::exists(const QString table, const QString field, const QString value) {
  QSqlQuery q;
  q.prepare("SELECT EXISTS (SELECT * FROM " + table + " WHERE " + field + " = :value);");
  q.bindValue(":value", value);
  q.exec();
  q.next();
  return q.value(0).toBool();
}

bool dbWorks::insert(const QString table, const QString field, const QString value) {
  QSqlQuery q;
  q.prepare("INSERT INTO " + table + " (" + field + ") VALUES (:value);");
  q.bindValue(":value", value);
  return q.exec();
}

bool dbWorks::del(const QString table, const QString id, const size_t value) {
  QSqlQuery q;
  q.prepare("DELETE FROM " + table +
            " WHERE " + id + " = " + QString::number(value) + ";");
  return q.exec();
}

unsigned dbWorks::getID(const QString table,
                        const QString key,
                        const QString field,
                        const QString value) {

  // Проверка на существование поля с таким значением.
  assert(exists(table, field, value));

  QSqlQuery q;
  q.prepare("SELECT " + key + " FROM " + table +
         " WHERE " + field + " = :value;");
  q.bindValue(":value", value);
  q.exec();
  q.next();
  return q.value(0).toUInt();
}

bool dbWorks::userRegistration(const QString login, const QString password) {
  QSqlQuery q;
  q.prepare("INSERT INTO USERS (login, password) VALUES (:login, :password);");
  q.bindValue(":login", login);
  q.bindValue(":password", password);
  bool isUserCreated = q.exec();

  setCurrentUser(login);
  return isUserCreated;
}

void dbWorks::setCurrentUser(const QString login) {
  currentUserID = getUserID(login);
}

unsigned dbWorks::getUserID(const QString login) {
  QSqlQuery q;
  q.prepare("SELECT idUser FROM USERS WHERE login = :login;");
  q.bindValue(":login", login);
  q.exec();
  q.next();
  return q.value(0).toUInt();
}

QString dbWorks::getUserLogin() {
  QSqlQuery q;
  q.prepare("SELECT login FROM USERS WHERE idUser = :currentUserID;");
  q.bindValue(":currentUserID", currentUserID);
  q.exec();
  q.next();

  return q.value(0).toString();
}

UserRoles dbWorks::getUserRole() {
  QSqlQuery q;
  q.prepare("SELECT role FROM USERS WHERE idUser = :currentUserID;");
  q.bindValue(":currentUserID", currentUserID);
  q.exec();
  q.next();

  return static_cast<UserRoles>(q.value(0).toInt());
}

dbWorks::~dbWorks()
{
  db.close();
}

dbWorks::dbWorks()
  : currentUserID{ }
{
  // Если строка, хранящая путь до БД пустая.
  if (pathToDB.isEmpty())
  {
    qDebug() << "[BUG] Path to database string is empty! Database wasn't created.\n";
    return;
  }

  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(pathToDB);

  if (!db.open())
    qDebug() << db.lastError();
}
