#include "grabwidget.h"



GrabWidget::GrabWidget(QWidget *parent)
    : QWidget{parent}
{
    m_okButton.setParent(this);
    m_okButton.resize(40,20);
    m_okButton.setScreen(QGuiApplication::primaryScreen());
    m_okButton.setText("确定");
    connect(&m_okButton,&QPushButton::clicked,this,&GrabWidget::on_okButton_clicked);


    m_cancelButton.setParent(this);
    m_cancelButton.resize(40,20);
    m_cancelButton.setScreen(QGuiApplication::primaryScreen());
    m_cancelButton.setText("取消");
    connect(&m_cancelButton,&QPushButton::clicked,this,&GrabWidget::on_cancelButton_clicked);

    this->setWindowOpacity(0.5);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);  // 无边框窗口
    setGeometry(QGuiApplication::primaryScreen()->geometry());  // 设置全屏
}

void GrabWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        m_startPoint=event->pos();
            m_endPoint=m_startPoint;
        }
    }

 void GrabWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_startPoint != QPoint(-1, -1)) {
        // 完成选择区域，获取截图

        captureRect=QRect(m_startPoint, m_endPoint);//得到截取区域
        if (captureRect.height()<5||captureRect.width()<5)//如果区域过小就认定为是想要截取全屏
            captureRect=QGuiApplication::primaryScreen()->geometry();
        else
        {
            captureRect = captureRect.normalized();  // 确保矩形的起点为左上角
            m_okButton.move(QPoint(captureRect.x()+captureRect.width()-80,captureRect.y()+captureRect.height()));//确定按钮
            m_cancelButton.move(QPoint(captureRect.x()+captureRect.width()-40,captureRect.y()+captureRect.height()));//取消按钮
            m_okButton.show();
            m_cancelButton.show();
        }
    }
}

void GrabWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_startPoint != QPoint(-1, -1)) {
        // 鼠标移动时，更新结束点并重绘
        m_endPoint = event->pos();
        update();
    }
}

void GrabWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 100));  // 半透明灰色背景
    painter.drawRect(rect());

    // 绘制选中的截图区域
    if (m_startPoint != QPoint(-1, -1)&&m_endPoint!=QPoint(-1, -1)) {
        QRect captureRect(m_startPoint, m_endPoint);
        painter.setBrush(QColor(255, 255, 255, 200));  // 半透明白色遮罩
        painter.drawRect(captureRect);
    }
}

void GrabWidget::on_okButton_clicked(bool checked)
{
    this->setWindowOpacity(0);//知道位置之后先将窗口设置为透明 避免影响之后的截取
    captureScreenshot(captureRect);
    close();  // 关闭选择区域窗口]
}

void GrabWidget::on_cancelButton_clicked(bool checked)
{
    close();
}


void GrabWidget::captureScreenshot(const QRect &captureRect)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (!screen) {
        qWarning() << "No screen found!";
        return;
    }
    // 截取选定区域
    QPixmap screenshot = screen->grabWindow(0, captureRect.x(), captureRect.y(), captureRect.width(), captureRect.height());

    // 保存截图到文件
    savePixmap(screenshot);
}

void GrabWidget::savePixmap(QPixmap &screenshot)
{
    QString savepath=m_saveshotDir+QDateTime::currentDateTime().toString().replace(" ","_").replace(":","_").append(".png");
    if (screenshot.save(savepath, "PNG")) {
        qDebug() << "Screenshot saved to screenshot.png";
    } else {
        qWarning() << "Failed to save screenshot!";
    }
}
