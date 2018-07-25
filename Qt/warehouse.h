#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <QtWidgets>
#include <QtSql>
#include "smart_types.h"

class warehouse : public QDialog
{
    Q_OBJECT
public:
    warehouse(QWidget *parent = 0);
    ~warehouse();

private:
    QTableWidget *wrhouse_table;
    QVBoxLayout *right_layout;
    QHBoxLayout *main_layout;
    QPushButton *refresh_pushbutton;
    int num;
signals:
    void wrhouse_close_signal();

private slots:
    void wrhouse_refresh_slot();
    void wrhouse_add_record(record_t);


};

#endif // WAREHOUSE_H
