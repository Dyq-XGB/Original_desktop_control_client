#include "widget.h"
#include "./ui_widget.h"

#include <Qurl>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete m_cameraWidget;
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    do_ScreenCapture();
    do_StartScreenRecoder();
}



void Widget::on_pushButton_2_clicked()
{
    do_StopScreenCapture();
    do_StopScreenRecoder();
}

void Widget::on_pushButton_3_clicked()
{
    player.play();
}

void Widget::on_VideoFrameChanged(QVideoFrame frame)
{
    QImage image=frame.toImage();
    image.mirror(true,false);
    QVideoFrame mirror_frame(image);
    ui->widget->videoSink()->setVideoFrame(mirror_frame);
}


void Widget::do_ScreenCapture()
{
    m_mediaProcess.start_ScreenCapture(ui->widget->videoSink());
}

void Widget::do_GrabScreen()
{
    m_mediaProcess.do_GrabScreen();
}

void Widget::do_OpenCamera()
{
    if(m_cameraWidget==nullptr)
        m_cameraWidget=new QVideoWidget();
    m_cameraWidget->show();
    m_mediaProcess.open_Camera(m_cameraWidget->videoSink());
}

void Widget::do_CloseCamera()
{
    m_mediaProcess.close_Camera();
    m_cameraWidget->setVisible(false);
}

void Widget::do_StopScreenCapture()
{
    m_mediaProcess.stop_ScreenCapture();
    const QVideoFrame frame;
    ui->widget->videoSink()->setVideoFrame(frame);
}

void Widget::do_WindCapture(QString windowTitle)
{
    m_mediaProcess.setVideoOutputWidget(ui->widget->videoSink());
    m_mediaProcess.start_WindowCapture(windowTitle);
}

void Widget::do_ImageCapture()
{
    m_mediaProcess.ImageCapture();
}

void Widget::do_StopWindowCapture()
{
    m_mediaProcess.stop_WindowCapture();
    const QVideoFrame frame;
    ui->widget->videoSink()->setVideoFrame(frame);
}

void Widget::do_StartMediaRecorder()
{
    // m_mediaProcess.setVideoOutputWidget(&m_videoSink);
    // m_mediaProcess.open_Camera();
    // m_mediaProcess.start_CameraRecorder();
}

void Widget::do_PauseMediaRecorder()
{

    m_mediaProcess.pause_CameraRecorder();
}

void Widget::do_StopMediaRecorder()
{
    m_mediaProcess.close_Camera();
    m_mediaProcess.stop_CameraRecorder();
}

void Widget::do_StartScreenRecoder()
{
    connect(&player,&QMediaPlayer::errorOccurred,this,[](QMediaPlayer::Error error, const QString &errorString){qDebug()<<errorString;});
    connect(&player,&QMediaPlayer::mediaStatusChanged,this,[](QMediaPlayer::MediaStatus status){qDebug()<<status;});
    barray=new QByteArray();
    barray->resize(100000);
    buffer=new QBuffer(barray);
    buffer->open(QIODevice::OpenModeFlag::ReadWrite);

    connect(buffer,&QBuffer::bytesWritten,this,[](qint64 bytes){qDebug()<<bytes;});
    m_mediaProcess.start_ScreenRecord(buffer);
    player.setSource(QUrl()
    QVideoWidget*widget=new QVideoWidget();
    audio_ouput.setDevice(QMediaDevices::defaultAudioOutput());
    player.setVideoSink(widget->videoSink());
    player.setAudioOutput(&audio_ouput);
    widget->show();
}

void Widget::do_StopScreenRecoder()
{
    m_mediaProcess.stop_ScreenRecord();
}


