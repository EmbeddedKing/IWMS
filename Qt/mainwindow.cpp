#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "videodialog.h"
#include "warehouse.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cli_socket = new QTcpSocket();

    serial = new QSerialPort();
    connect(serial, SIGNAL(readyRead()), this, SLOT(serial_readdata_slot()));
    /* COM4口 */
    serial->setPortName("COM4");
    //设置波特率
    serial->setBaudRate(9600);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if(serial->open(QIODevice::ReadWrite))
        qDebug() << serial->portName()<<endl << "打开成功";
    else
        qDebug() << serial->portName()<<endl << "打开失败";

    /* 连接服务器按键 */
    connect(ui->connect_pushbutton, SIGNAL(clicked(bool)), this, SLOT(server_connect_slot()));

    /* socket */
    connect(cli_socket, SIGNAL(connected()),    this, SLOT(socket_connected_slot()));
    connect(cli_socket, SIGNAL(disconnected()), this, SLOT(socket_disconnected_slot()));
    connect(cli_socket, SIGNAL(readyRead()),    this, SLOT(socket_readdata_slot()));

    /* 视频流按键 */
    connect(ui->openvid_pushbutton, SIGNAL(clicked(bool)), this, SLOT(video_open_slot()));

    /* 打开仓库按键 */
    connect(ui->openwrhouse_pushbutton, SIGNAL(clicked(bool)), this, SLOT(wrhouse_open_slot()));

    /* 数据流按键 */
    connect(ui->opendata_pushbutton,  SIGNAL(clicked(bool)), this, SLOT(data_open_slot()));
    connect(ui->closedata_pushbutton, SIGNAL(clicked(bool)), this, SLOT(data_close_slot()));

    /* 控制设备按键 */
    connect(ui->onled_pushbutton,       SIGNAL(clicked(bool)), this, SLOT(led_on_slot()));
    connect(ui->offled_pushbutton,      SIGNAL(clicked(bool)), this, SLOT(led_off_slot()));
    connect(ui->onfan_pushbutton,       SIGNAL(clicked(bool)), this, SLOT(fan_on_slot()));
    connect(ui->offfan_pushbutton,      SIGNAL(clicked(bool)), this, SLOT(fan_off_slot()));
    connect(ui->onsevenled_pushbutton,  SIGNAL(clicked(bool)), this, SLOT(sevenled_on_slot()));
    connect(ui->offsevenled_pushbutton, SIGNAL(clicked(bool)), this, SLOT(sevenled_off_slot()));
    connect(ui->onspeaker_pushbutton,   SIGNAL(clicked(bool)), this, SLOT(speaker_on_slot()));
    connect(ui->offspeaker_pushbutton,  SIGNAL(clicked(bool)), this, SLOT(speaker_off_slot()));

}

void MainWindow::delay_ms(unsigned int msec)
{
    QTime delayTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < delayTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::server_connect_slot()
{
    cli_socket->connectToHost(QHostAddress(ui->ip_lineedit->text()), ui->dataport_lineedit->text().toInt());
}

void MainWindow::socket_connected_slot()
{
    ui->onled_pushbutton->setEnabled(true);
    ui->onfan_pushbutton->setEnabled(true);
    ui->onsevenled_pushbutton->setEnabled(true);
    ui->onspeaker_pushbutton->setEnabled(true);
    ui->offled_pushbutton->setEnabled(true);
    ui->offfan_pushbutton->setEnabled(true);
    ui->offsevenled_pushbutton->setEnabled(true);
    ui->offspeaker_pushbutton->setEnabled(true);
    ui->opendata_pushbutton->setEnabled(true);
    ui->closedata_pushbutton->setEnabled(true);
    ui->openwrhouse_pushbutton->setEnabled(true);
}

void MainWindow::socket_disconnected_slot()
{
    ui->onled_pushbutton->setEnabled(false);
    ui->onfan_pushbutton->setEnabled(false);
    ui->onsevenled_pushbutton->setEnabled(false);
    ui->onspeaker_pushbutton->setEnabled(false);
    ui->offled_pushbutton->setEnabled(false);
    ui->offfan_pushbutton->setEnabled(false);
    ui->offsevenled_pushbutton->setEnabled(false);
    ui->offspeaker_pushbutton->setEnabled(false);
    ui->opendata_pushbutton->setEnabled(false);
    ui->closedata_pushbutton->setEnabled(false);
    ui->openwrhouse_pushbutton->setEnabled(false);
}

void MainWindow::data_message_handler(message_t msg)
{
    static int count = 0;
    ui->led1state_label->setText(QString("LED状态   ：") + (msg.data.env_msg.state.led_state ? QString("打开") : QString("关闭")));
    ui->fanstate_label->setText(QString("风扇状态  ：") + (msg.data.env_msg.state.fan_state ? QString("打开") : QString("关闭")));
    ui->sevenledstate_label->setText(QString("数码管状态：") + (msg.data.env_msg.state.seven_led_state ? QString("打开") : QString("关闭")));
    ui->speakerstate_label->setText(QString("报警器状态：") + (msg.data.env_msg.state.buzz_state ? QString("打开") : QString("关闭")));

    ui->temp_label->setText(QString("温度：") + QString::number(msg.data.env_msg.tem.htem) + "." + QString::number(msg.data.env_msg.tem.ltem));
    ui->hum_label->setText(QString("温度：") + QString::number(msg.data.env_msg.hum.hhum) + "." + QString::number(msg.data.env_msg.hum.lhum));
    ui->adc_label->setText(QString("ADC ：") + QString::number(msg.data.env_msg.adc.ad0));
    ui->x_label->setText(QString("光照：") + QString::number(msg.data.env_msg.light.light));

    ui->x_label->setText(QString("X：") + QString::number(msg.data.env_msg.acc.x));
    ui->y_label->setText(QString("Y：") + QString::number(msg.data.env_msg.acc.y));
    ui->z_label->setText(QString("Z：") + QString::number(msg.data.env_msg.acc.z));

    if (count++ < 100)
        return;

    if (msg.data.env_msg.tem.htem > ui->tempmax_spinbox->text().toInt() ||
        msg.data.env_msg.tem.htem < ui->tempmin_spinbox->text().toInt() ||
        msg.data.env_msg.hum.hhum > ui->hummax_spinbox->text().toInt() ||
        msg.data.env_msg.hum.hhum < ui->hummin_spinbox->text().toInt()) {
        serial->write("AT\r");
        delay_ms(2000);
        serial->write("AT+CMGF=1\r");
        delay_ms(2000);
        serial->write("AT+CMGS=\"17774524828\"\r");
        delay_ms(2000);
        QByteArray msgbytes = ui->message_lineedit->text().toLatin1();
        serial->write(msgbytes);
        delay_ms(2000);
        const char tail = 26;
        serial->write(&tail);
        qDebug() << "发送完成";
        qDebug() << "打开数码管";
        QByteArray bytes;
        smart_cmd_pake_t pake;
        pake.pake_cmd = DATA_CMD_ON_SEVEN_LED;
        bytes.resize(sizeof(smart_cmd_pake_t));
        memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
        cli_socket->write(bytes);
    } else {
        qDebug() << "关闭数码管";
        QByteArray bytes;
        smart_cmd_pake_t pake;
        pake.pake_cmd = DATA_CMD_OFF_SEVEN_LED;
        bytes.resize(sizeof(smart_cmd_pake_t));
        memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
        cli_socket->write(bytes);
    }
    count = 0;
}

void MainWindow::data_record_handler(record_t record)
{
    emit wrhouse_record_signal(record);
}

void MainWindow::serial_readdata_slot()
{

}

void MainWindow::socket_readdata_slot()
{
    int len;
    smart_data_pake_t pake;
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray bytes;
    len = socket->bytesAvailable();
    if (len < sizeof(smart_data_pake_t)) {
        return;
    }

    /* 将数据包解析 */
    bytes = socket->read(sizeof(smart_data_pake_t));
    memcpy(&pake, bytes.data(), sizeof(smart_data_pake_t));

    switch (pake.pake_type) {
    case DATA_PAKE_TYPE_MSG:
        data_message_handler(pake.pake_msg);
        break;
    case DATA_PAKE_TYPE_RECORD:
        data_record_handler(pake.pake_record);
        break;
    }

}

void MainWindow::video_open_slot()
{
    /* 创建一个视频窗口把IP和端口发过去 */
    videodialog *vd = new videodialog(ui->ip_lineedit->text(), ui->videoport_lineedit->text().toInt(), this);
    connect(vd, SIGNAL(video_close_signal()), this, SLOT(video_close_slot()));
    ui->openvid_pushbutton->setEnabled(false);
    vd->show();
}

void MainWindow::video_close_slot()
{
    ui->openvid_pushbutton->setEnabled(true);
}

void MainWindow::wrhouse_open_slot()
{
    /* 发送一个获取记录的控制包 */
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_GET_RECORD;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);

    warehouse *wh = new warehouse(this);
    connect(this, SIGNAL(wrhouse_record_signal(record_t)), wh, SLOT(wrhouse_add_record(record_t)));
    connect(wh, SIGNAL(wrhouse_close_signal()), this, SLOT(wrhouse_close_slot()));
    wh->show();
    ui->openwrhouse_pushbutton->setEnabled(false);
}

void MainWindow::wrhouse_close_slot()
{
    ui->openwrhouse_pushbutton->setEnabled(true);
}

void MainWindow::data_open_slot()
{
    /* 发送一个开启数据流的控制包 */
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_ON_STREAM;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::data_close_slot()
{
    /* 发送一个关闭数据流的控制包 */
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_OFF_STREAM;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);

    ui->temp_label->setText(QString("温度：0"));
    ui->hum_label->setText(QString("温度：0"));
    ui->adc_label->setText(QString("ADC ：0"));
    ui->x_label->setText(QString("光照：0"));

    ui->x_label->setText(QString("X：0"));
    ui->y_label->setText(QString("Y：0"));
    ui->z_label->setText(QString("Z：0"));

    ui->led1state_label->setText(QString("LED状态   ：未知"));
    ui->fanstate_label->setText(QString("风扇状态  ：未知"));
    ui->sevenledstate_label->setText(QString("数码管状态：未知"));
    ui->speakerstate_label->setText(QString("报警器状态：未知"));
}

void MainWindow::led_on_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_ON_LED1;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::led_off_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_OFF_LED1;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::fan_on_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_ON_FAN;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::fan_off_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_OFF_FAN;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::sevenled_on_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_ON_SEVEN_LED;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::sevenled_off_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_OFF_SEVEN_LED;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::speaker_on_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_ON_SPEAKER;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

void MainWindow::speaker_off_slot()
{
    QByteArray bytes;
    smart_cmd_pake_t pake;
    pake.pake_cmd = DATA_CMD_OFF_SPEAKER;
    bytes.resize(sizeof(smart_cmd_pake_t));
    memcpy(bytes.data(), &pake, sizeof(smart_cmd_pake_t));
    cli_socket->write(bytes);
}

MainWindow::~MainWindow()
{
    delete ui;
}
