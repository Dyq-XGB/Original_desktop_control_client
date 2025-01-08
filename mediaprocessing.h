#ifndef MEDIAPROCESSING_H
#define MEDIAPROCESSING_H

#include <QObject>
#include <QtMultimedia/QtMultimedia>
#include <QWidget>
#include <QVideoWidget>
#include <QVideoFrame>

#include "grabwidget.h"
class MediaProcessing : public QObject
{
    Q_OBJECT
public:
    explicit MediaProcessing(QObject *parent = nullptr);

public:
    void setVideoOutputWidget(QVideoSink*videosink);

    void start_WindowCapture(QString windowTitle);//待测试
    void stop_WindowCapture();//已完成

    void ImageCapture();//已完成



    //待测试
    void start_PlayMedia(QUrl mediaurl,QVideoSink*videosink);
    void stop_PlayMedia();
    void pause_PlayMedia();

    void stop_CameraRecorder();//已完成
    void start_CameraRecorder();//已完成
    void pause_CameraRecorder();//已完成


    void close_Camera();//已完成
    void open_Camera(QVideoSink *videosink);//已完成

    void do_GrabScreen();//已完成


    void start_ScreenCapture(QVideoSink*videosink);//已完成
    void stop_ScreenCapture();//已完成
    void start_ScreenRecord(QIODevice *buffer);//已完成
    void stop_ScreenRecord();//已完成


private slots:
    void on_QWindowCapture_errorOccurred(QWindowCapture::Error error, const QString &errorString);
    void on_QImageCapture_errorOccurred(int id, QImageCapture::Error error, const QString &errorString);
    void on_QImageCapture_imageAvailable(int id, const QVideoFrame &frame);
    void on_QImageCapture_fileFormatChanged();

    void on_videoFrameChanged(const QVideoFrame &frame);

private:
    void setMediaRecoderForamt(QMediaFormat::FileFormat fileformat=QMediaFormat::FileFormat::WebM,
                               QMediaFormat::AudioCodec audiocodec=QMediaFormat::AudioCodec::AC3,
                               QMediaFormat::VideoCodec videocodec=QMediaFormat::VideoCodec::H264
                               , qreal video_frameRate=60, int video_bitRate=8000000, QSize video_size=QGuiApplication::primaryScreen()->size(),
                                int audio_bitRate=128000,int audio_sampleRate=44100,int audio_channels=2);
    void setAudioInputDevice(QAudioDevice device=QMediaDevices::defaultAudioInput());
    void setAudioOutDevice(QAudioDevice device=QMediaDevices::defaultAudioOutput());
    void setCameraDevice(QCameraDevice device=QMediaDevices::defaultVideoInput());
    void setImageCaptureFileFormat(QImageCapture::FileFormat format=QImageCapture::FileFormat::JPEG);
    void setImageCaptureQuality(QImageCapture::Quality quality=QImageCapture::HighQuality);
    void setScreenCaptureScreen(QScreen*screen=QGuiApplication::primaryScreen());


    int calculateByteCount(const QVideoFrame &frame);

private:


    QCamera m_camera;//摄像头
    QAudioInput  m_audioInput;//音频输入
    QAudioOutput m_audioOutput;//音频输出


    QMediaCaptureSession m_session; //媒体集结器
    QWindowCapture m_windCapture;//窗口捕捉器
    QScreenCapture m_screenCapture;//屏幕捕捉器
    QImageCapture m_imageCapture;//图片
    QMediaRecorder m_mediaRecoder;//媒体记录
    QMediaFormat m_mediaRecoderFormat;//媒体数据
    GrabWidget m_grabWidget;//截图器
    QMediaPlayer m_mediaPlayer;//媒体播放器
    QTimer m_captureTimer;//捕捉记时

    QMediaCaptureSession m_sessionCamera;

    QVideoSink m_videosink;
    QVideoSink*m_cameraWidgetVideoSink;
private:
    QString m_saveImageCaptureDir="D:/";
    QString m_imageSaveSuffix=".jpg";
    QString m_saveMediaRecoderDir="D:/";
    QString m_saveMediaRecoderSuffix=".mp4";

signals:
};

#endif // MEDIAPROCESSING_H
