#include "videodialog.h"

videodialog::videodialog(QString ip, qint16 port, QWidget *parent) : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("仓库监控");
    video_label = new QLabel("正在获取视频中...");
    video_pushbutton = new QPushButton("抓拍");

    video_rightlayout = new QVBoxLayout();
    video_mainlayout = new QHBoxLayout();

    video_rightlayout->addWidget(video_pushbutton);
    video_rightlayout->addStretch();
    video_mainlayout->addWidget(video_label);
    video_mainlayout->addLayout(video_rightlayout);
    setLayout(video_mainlayout);

    video_socket = new QTcpSocket(this);
    video_socket->connectToHost(QHostAddress(ip), port);
    connect(video_socket, SIGNAL(connected()), this, SLOT(video_conncted_slot()));
    connect(video_socket, SIGNAL(readyRead()), this, SLOT(video_stream_slot()));
    connect(video_pushbutton, SIGNAL(clicked(bool)), this, SLOT(video_tack_slot()));
}

void videodialog::video_conncted_slot()
{
    qDebug() << "连接成功";
}

void videodialog::video_disconncted_slot()
{
    qDebug() << "断开连接";
}

void videodialog::video_stream_slot()
{
    int len;
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray bytes;
    len = socket->bytesAvailable();
    if (len < sizeof(smart_video_stream_t)) {
        return;
    }

    /* 将图像显示在窗口中 */
    bytes = socket->read(sizeof(smart_video_stream_t));
    smart_video_stream_t *pake = (smart_video_stream_t *)bytes.data();
    memcpy(&rgb_streamdata, pake, sizeof(smart_video_stream_t));
    QImage image(rgb_streamdata.pake_rgb_buf, rgb_streamdata.pake_rgb_width, rgb_streamdata.pake_rgb_height, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(image);
    video_label->setPixmap(pixmap);
}

void videodialog::video_tack_slot()
{
    /* 以时间戳来命名图片名称 */
    QDateTime time = QDateTime::currentDateTime();
    int timeT = time.toTime_t();
    QImage image(rgb_streamdata.pake_rgb_buf, rgb_streamdata.pake_rgb_width, rgb_streamdata.pake_rgb_height, QImage::Format_RGB888);
    image.save(QString("C:/Users/EmbeddedKing/Desktop/%1.jpg").arg(timeT), "JPEG");
}


videodialog::~videodialog()
{
    emit video_close_signal();
}
