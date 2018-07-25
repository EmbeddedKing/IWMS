#include "warehouse.h"

warehouse::warehouse(QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    wrhouse_table = new QTableWidget();
    wrhouse_table->setColumnCount(5);
    wrhouse_table->setHorizontalHeaderLabels(QStringList() << "货物编码" << "出/入库数量" << "出/入库时间" << "出/入库" << "库存");
    wrhouse_table->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止修改
    wrhouse_table->horizontalHeader()->setStretchLastSection(true); //就是这个地方
    wrhouse_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    wrhouse_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    refresh_pushbutton = new QPushButton("刷新数据");

    num = 0;
    /* 布局 */
    right_layout = new QVBoxLayout();
    main_layout = new QHBoxLayout();
    right_layout->addWidget(refresh_pushbutton);
    right_layout->addStretch();
    main_layout->addWidget(wrhouse_table);
    main_layout->addLayout(right_layout);
    setLayout(main_layout);

    /* 数据库操作 */
    connect(refresh_pushbutton, SIGNAL(clicked(bool)), this, SLOT(wrhouse_update_slot()));

    resize(700, 300);

}

warehouse::~warehouse()
{
    emit wrhouse_close_signal();
}

void warehouse::wrhouse_add_record(record_t record)
{
    qDebug() << "新增表项";
    int count;
    count = wrhouse_table->rowCount();
    wrhouse_table->insertRow(count);//增加一行
    QDateTime dt = QDateTime::fromTime_t(record.time); //将时间戳转为时间
    QString time = dt.toString("yyyy-MM-dd dddd hh:mm:ss");
    wrhouse_table->setItem(num, 0, new QTableWidgetItem(QString::number(record.id)));
    wrhouse_table->setItem(num, 1, new QTableWidgetItem(QString::number(record.num)));
    wrhouse_table->setItem(num, 2, new QTableWidgetItem(time));
    wrhouse_table->setItem(num, 3, new QTableWidgetItem(QString(record.dir)));
    wrhouse_table->setItem(num, 4, new QTableWidgetItem(QString::number(record.stock)));
    num++;
}

void warehouse::wrhouse_refresh_slot()
{

}

