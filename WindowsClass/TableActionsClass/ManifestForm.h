#pragma once

#include <QWidget>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

enum class TableTypes;

namespace Ui { class ManifestForm; }

class ManifestForm : public QWidget {
  Q_OBJECT

public:
  explicit ManifestForm(TableTypes type,
                        uint rowCount,
                        QWidget *parent = nullptr);
  ~ManifestForm();

private:
  void saveFormAsPNG();

  Ui::ManifestForm *ui;

  TableTypes tt;

  QSqlQueryModel* model;
};
