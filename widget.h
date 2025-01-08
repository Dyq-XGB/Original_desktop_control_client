#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVideoWidget>
#include <QFrame>
#include "mediaprocessing.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_VideoFrameChanged( QVideoFrame frame);

private:
    void do_ScreenCapture();
    void do_GrabScreen();
    void do_OpenCamera();
    void do_CloseCamera();
    void do_StopScreenCapture();
    void do_WindCapture(QString windowTitle);
    void do_ImageCapture();
    void do_StopWindowCapture();
    void do_StartMediaRecorder();
    void do_PauseMediaRecorder();
    void do_StopMediaRecorder();

    void do_StartScreenRecoder();
    void do_StopScreenRecoder();
private:
    QBuffer*buffer;
    QByteArray*barray;
    QMediaPlayer player;
    QAudioOutput audio_ouput;
    MediaProcessing m_mediaProcess;//处理关于媒体的类
    QVideoWidget* m_cameraWidget=nullptr;

    QFile file;
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
