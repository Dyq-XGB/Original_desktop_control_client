#ifndef GRABWIDGET_H
#define GRABWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDateTime>
#include <QPushButton>
class GrabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrabWidget(QWidget *parent = nullptr);

signals:

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
private slots:
    void on_okButton_clicked(bool checked);
    void on_cancelButton_clicked(bool checked);
private:
    void captureScreenshot(const QRect &captureRect);
    void savePixmap(QPixmap&screenshot);
private:
    QPoint m_startPoint;  // 鼠标按下位置
    QPoint m_endPoint;    // 鼠标释放位置
    QString m_saveshotDir="D:/";
    QPushButton m_okButton;//确定按钮
    QPushButton m_cancelButton;//取消按钮
    QRect captureRect;//截取范围
};

#endif // GRABWIDGET_H
