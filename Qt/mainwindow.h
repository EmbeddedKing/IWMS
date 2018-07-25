#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QtNetWork>
#include <QtSql>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "smart_types.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *cli_socket;
    QSerialPort *serial;

    void data_message_handler(message_t msg);
    void data_record_handler(record_t record);
    void delay_ms(unsigned int msec);

private slots:
    void server_connect_slot();
    /* 视频槽 */
    void video_open_slot();
    void video_close_slot();

    /* 数据流按键 */
    void data_open_slot();
    void data_close_slot();

    /* 打开仓库 */
    void wrhouse_open_slot();
    void wrhouse_close_slot();

    /* serial */
    void serial_readdata_slot();

    /* socket */
    void socket_connected_slot();
    void socket_disconnected_slot();
    void socket_readdata_slot();

    /* 控制按键 */
    void led_on_slot();
    void led_off_slot();
    void fan_on_slot();
    void fan_off_slot();
    void sevenled_on_slot();
    void sevenled_off_slot();
    void speaker_on_slot();
    void speaker_off_slot();
signals:
    void wrhouse_record_signal(record_t);
};


#endif // MAINWINDOW_H
