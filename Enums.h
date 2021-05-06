#pragma once

// Список перечисления для трёх видов таблиц,
//  которые можно отобразить на форме.
enum class TableTypes {
  WAREHOUSE,
  SHIPMENTS,
  STOCK
};

// Список перечисления для трёх типов кнопок "Новый"
//  на форме добавления товара.
enum class ButtonNewTypes {
  TYPE,
  COUNTRY,
  SUPPLIER
};
