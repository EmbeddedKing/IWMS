#ifndef VIDEODIALOG_H
#define VIDEODIALOG_H

#include <QtWidgets>
#include <QtNetwork>
#include "smart_types.h"

class videodialog : public QDialog
{
    Q_OBJECT
public:
    explicit videodialog(QString ip, qint16 port, QWidget *parent = 0);
    ~videodialog();

private:
    QLabel *video_label;
    QPushButton *video_pushbutton;
    QTcpSocket *video_socket;
    QHBoxLayout *video_mainlayout;
    QVBoxLayout *video_rightlayout;
    smart_video_stream_t rgb_streamdata;

signals:
    void video_close_signal();

private slots:
    void video_conncted_slot();
    void video_disconncted_slot();
    void video_stream_slot();
    void video_tack_slot();
};

#endif // VIDEODIALOG_H
