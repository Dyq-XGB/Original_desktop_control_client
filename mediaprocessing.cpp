#include "mediaprocessing.h"

#include <QApplication>

MediaProcessing::MediaProcessing(QObject *parent)
    : QObject{parent}
{
    //将个媒体处理集结
    setAudioInputDevice();
    setAudioOutDevice();
    setCameraDevice();


    connect(&m_windCapture,&QWindowCapture::errorOccurred,this,&MediaProcessing::on_QWindowCapture_errorOccurred);
    connect(&m_imageCapture,&QImageCapture::errorOccurred,this,&MediaProcessing::on_QImageCapture_errorOccurred);
    connect(&m_imageCapture,&QImageCapture::imageAvailable,this,&MediaProcessing::on_QImageCapture_imageAvailable);
    connect(&m_imageCapture,&QImageCapture::fileFormatChanged,this,&MediaProcessing::on_QImageCapture_fileFormatChanged);

    connect(&m_mediaRecoder, &QMediaRecorder::errorOccurred, [](QMediaRecorder::Error error, const QString &errorString) {
        qDebug() << "Recorder error: " << errorString;
    });
    // QVideoSink sink;

    // QVideoFrame frame;
    // frame.setStreamFrameRate(30);
    // frame.setStreamFrameRate()
}

void MediaProcessing::setVideoOutputWidget(QVideoSink *videosink)
{
    m_session.setVideoOutput(videosink);
}

void MediaProcessing::start_WindowCapture(QString windowTitle)
{
    m_session.setWindowCapture(&m_windCapture);//设置窗口捕捉
    QList<QCapturableWindow> list=QWindowCapture::capturableWindows();
    for(QCapturableWindow&wind:list)
    {
        if(wind.description()==windowTitle)
        {
            m_windCapture.setWindow(wind);
            m_windCapture.start();
            break;
        }
    }
}

void MediaProcessing::start_ScreenCapture(QVideoSink*videosink)
{
    m_session.setVideoSink(videosink);
    setScreenCaptureScreen();
    m_session.setScreenCapture(&m_screenCapture);//设置屏幕捕捉
    m_screenCapture.start();
}

void MediaProcessing::stop_ScreenCapture()
{
    m_screenCapture.stop();
    m_session.setScreenCapture(nullptr);//设置屏幕捕捉
    m_session.setVideoSink(nullptr);
}

void MediaProcessing::start_ScreenRecord(QIODevice*buffer)
{
    setMediaRecoderForamt();
    QString savePath=m_saveMediaRecoderDir+QDateTime::currentDateTime().toString().replace(" ","_").replace(":","_")+m_saveMediaRecoderSuffix;
    // m_mediaRecoder.setOutputLocation(QUrl(savePath));
    m_mediaRecoder.setOutputDevice(buffer);

    m_session.setAudioInput(&m_audioInput);//设置音频输入
    m_session.setRecorder(&m_mediaRecoder);//设置媒体记录

    m_screenCapture.start();
    m_mediaRecoder.record();
}

void MediaProcessing::stop_ScreenRecord()
{
    m_screenCapture.stop();
    m_mediaRecoder.stop();
    m_session.setAudioInput(nullptr);
}


void MediaProcessing::ImageCapture()
{
    m_session.setImageCapture(&m_imageCapture);//设置图片捕捉
    setImageCaptureQuality();
    setImageCaptureFileFormat();
    m_imageCapture.capture();
}


void MediaProcessing::stop_CameraRecorder()
{
    m_mediaRecoder.stop();
    m_camera.stop();
    m_session.setAudioInput(nullptr);
    m_session.setCamera(nullptr);
}

void MediaProcessing::start_CameraRecorder()
{
    setMediaRecoderForamt();
    m_session.setAudioInput(&m_audioInput);//设置音频输入
    m_session.setCamera(&m_camera);//设置相机
    m_session.setRecorder(&m_mediaRecoder);//设置媒体记录

    QString savePath=m_saveMediaRecoderDir+QDateTime::currentDateTime().toString().replace(" ","_").replace(":","_")+m_saveMediaRecoderSuffix;
    m_mediaRecoder.setOutputLocation(QUrl(savePath));

    m_mediaRecoder.record();
}

void MediaProcessing::pause_CameraRecorder()
{
    m_mediaRecoder.pause();
}


void MediaProcessing::start_PlayMedia(QUrl mediaurl, QVideoSink *videosink)
{
    m_mediaPlayer.setSource(mediaurl);
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
    m_mediaPlayer.setVideoSink(videosink);
    m_mediaPlayer.play();
}

void MediaProcessing::stop_PlayMedia()
{
    m_mediaPlayer.stop();
}

void MediaProcessing::pause_PlayMedia()
{
    m_mediaPlayer.pause();
}

void MediaProcessing::stop_WindowCapture()
{
    m_windCapture.stop();
}



void MediaProcessing::close_Camera()
{
    m_camera.stop();
    disconnect(&m_videosink,&QVideoSink::videoFrameChanged,this,&MediaProcessing::on_videoFrameChanged);
    m_cameraWidgetVideoSink=nullptr;
    m_sessionCamera.setCamera(nullptr);
    m_sessionCamera.setVideoSink(nullptr);
}

void MediaProcessing::open_Camera(QVideoSink*videosink)
{
    m_cameraWidgetVideoSink=videosink;
    connect(&m_videosink,&QVideoSink::videoFrameChanged,this,&MediaProcessing::on_videoFrameChanged);
    m_sessionCamera.setCamera(&m_camera);
    m_sessionCamera.setVideoSink(&m_videosink);
    m_camera.start();
}

void MediaProcessing::do_GrabScreen()
{
    QEventLoop loop;
    GrabWidget grabwidget;
    grabwidget.show();
    loop.exec();
}

void MediaProcessing::on_QWindowCapture_errorOccurred(QWindowCapture::Error error, const QString &errorString)
{
    qDebug()<<"QWindowCaptureError!\t"<<"errorCode:"<<error<<"\t"<<errorString;
}

void MediaProcessing::on_QImageCapture_errorOccurred(int id, QImageCapture::Error error, const QString &errorString)
{
    qDebug()<<"QImageCaptureError!\t"<<"errorCode:"<<error<<"\t"<<errorString;
}

void MediaProcessing::on_QImageCapture_imageAvailable(int id, const QVideoFrame &frame)
{
    qDebug()<<"id:"<<id;
    QImage image=frame.toImage().mirrored(true,false);
    qDebug()<<"isnull:"<<image.isNull();
    QImageCapture::FileFormat format=m_imageCapture.fileFormat();
    QString suffix="";

    QString savepath=m_saveImageCaptureDir+QString::number(id)+'_'+QDateTime::currentDateTime().toString().replace(" ","_").replace(":","_")+m_imageSaveSuffix;
    qDebug()<<savepath;
    image.save(savepath,"JPEG");
}

void MediaProcessing::on_QImageCapture_fileFormatChanged()
{

    // m_videoFrameInput.
    // QImageCapture::FileFormat format=m_imageCapture.fileFormat();
    // QString formaName=QImageCapture::fileFormatName(format);
    // m_imageSaveSuffix=QString('.').append(formaName);
}

void MediaProcessing::on_videoFrameChanged(const QVideoFrame &frame)
{
    int planeCount=frame.planeCount();
    int countByteSize=0;
    qDebug()<<"FrameplaneCount:"<<planeCount<<"FrameByteSize:"<<calculateByteCount(frame);

    QVideoFrame frame1=frame;
    frame1.map(QVideoFrame::ReadOnly);

    planeCount=frame1.planeCount();
    for(int i=0;i<3;++i)
    {
        countByteSize+=frame1.mappedBytes(i);
    }
    qDebug()<<"FrameplaneCount:"<<planeCount<<"FrameByteSize:"<<countByteSize;

    QImage image=frame1.toImage().convertToFormat(QImage::Format_RGB888);
    image.mirror(true,false);

    qDebug()<<"ImageByteCount:"<<image.sizeInBytes();
    QVideoFrame mirrored_frame(image);
    mirrored_frame.setStartTime(QDateTime::currentMSecsSinceEpoch());

    m_cameraWidgetVideoSink->setVideoFrame(mirrored_frame);
}


//设置媒体格式
void MediaProcessing::setMediaRecoderForamt(QMediaFormat::FileFormat fileformat,
                                            QMediaFormat::AudioCodec audiocodec,
                                            QMediaFormat::VideoCodec videocodec,
                                            qreal video_frameRate , int video_bitRate, QSize video_size,
                                            int audio_bitRate,int audio_sampleRate,int audio_channels
                                            )
{
    m_mediaRecoderFormat.setFileFormat(QMediaFormat::FileFormat::MPEG4);
    m_mediaRecoderFormat.setAudioCodec(QMediaFormat::AudioCodec::AC3);
    m_mediaRecoderFormat.setVideoCodec(QMediaFormat::VideoCodec::H264);
    m_mediaRecoder.setVideoFrameRate(video_frameRate);
    m_mediaRecoder.setVideoBitRate(video_bitRate);
    m_mediaRecoder.setVideoResolution(video_size);
    m_mediaRecoder.setAudioBitRate(audio_bitRate);
    m_mediaRecoder.setAudioSampleRate(audio_sampleRate);
    m_mediaRecoder.setAudioChannelCount(audio_channels);
    m_mediaRecoder.setEncodingMode(QMediaRecorder::EncodingMode::AverageBitRateEncoding);
    m_mediaRecoder.setMediaFormat(m_mediaRecoderFormat);
}

void MediaProcessing::setAudioInputDevice(QAudioDevice device)
{
    m_audioInput.setDevice(device);//设置音频输入设备
}

void MediaProcessing::setAudioOutDevice(QAudioDevice device)
{
    m_audioInput.setDevice(device);
}

void MediaProcessing::setCameraDevice(QCameraDevice device)
{
    m_camera.setCameraDevice(device);//设置摄像头 摄像头是自带格式的
}

void MediaProcessing::setImageCaptureFileFormat(QImageCapture::FileFormat format)
{
    m_imageCapture.setFileFormat(format);
}

void MediaProcessing::setImageCaptureQuality(QImageCapture::Quality quality)
{
    m_imageCapture.setQuality(quality);
}

void MediaProcessing::setScreenCaptureScreen(QScreen *screen)
{
    m_screenCapture.setScreen(screen);
}

int MediaProcessing::calculateByteCount(const QVideoFrame &frame) {
    if (!frame.isValid()) {
        qDebug() << "无效的视频帧";
        return 0;
    }

    QVideoFrameFormat format = frame.surfaceFormat();
    int width = frame.width();
    int height = frame.height();
    int byteCount = 0;

    switch (format.pixelFormat()) {
    case QVideoFrameFormat::Format_ARGB8888:
    case QVideoFrameFormat::Format_ARGB8888_Premultiplied:
    case QVideoFrameFormat::Format_BGRA8888:
    case QVideoFrameFormat::Format_BGRA8888_Premultiplied:
    case QVideoFrameFormat::Format_RGBA8888:
    case QVideoFrameFormat::Format_RGBX8888:
    case QVideoFrameFormat::Format_ABGR8888:
    case QVideoFrameFormat::Format_XBGR8888:
    case QVideoFrameFormat::Format_XRGB8888:
    case QVideoFrameFormat::Format_BGRX8888:
        // 每像素4字节
        byteCount = width * height * 4;
        break;


    case QVideoFrameFormat::Format_YUV420P:
    case QVideoFrameFormat::Format_YUV420P10:
    case QVideoFrameFormat::Format_YV12:
        // YUV420P: 1.5字节/像素
        byteCount = (width * height * 3) / 2;
        break;

    case QVideoFrameFormat::Format_NV12:
    case QVideoFrameFormat::Format_NV21:
        // NV12 和 NV21: 1.5字节/像素
        byteCount = (width * height * 3) / 2;
        break;

    case QVideoFrameFormat::Format_YUYV:
    case QVideoFrameFormat::Format_UYVY:
        // YUYV 和 UYVY: 2字节/像素
        byteCount = width * height * 2;
        break;

    case QVideoFrameFormat::Format_Y8:
        // 灰度: 1字节/像素
        byteCount = width * height;
        break;

    case QVideoFrameFormat::Format_Y16:
        // 高精度灰度: 2字节/像素
        byteCount = width * height * 2;
        break;

    case QVideoFrameFormat::Format_P010:
    case QVideoFrameFormat::Format_P016:
        // P010 和 P016: 2字节/像素，可能有不同的子采样
        byteCount = (width * height * 3) / 2;
        break;

    case QVideoFrameFormat::Format_IMC1:
    case QVideoFrameFormat::Format_IMC2:
    case QVideoFrameFormat::Format_IMC3:
    case QVideoFrameFormat::Format_IMC4:
        // IMC格式: 1.5字节/像素
        byteCount = (width * height * 3) / 2;
        break;

    case QVideoFrameFormat::Format_SamplerExternalOES:
    case QVideoFrameFormat::Format_Jpeg:
    case QVideoFrameFormat::Format_SamplerRect:
        // 压缩格式，无法直接计算字节数
        qDebug() << "压缩的像素格式，无法直接计算字节数";
        byteCount = 0;
        break;

    default:
        qDebug() << "未知的像素格式";
        byteCount = 0;
        break;
    }

    return byteCount;
}
