#ifndef SUBDIALOG_H
#define SUBDIALOG_H
#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QResizeEvent>
#include <QGraphicsSceneMouseEvent>

namespace Ui {
class SubDialog;
}

class SubDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubDialog(QWidget *parent = nullptr);
    ~SubDialog();
public:
    void showErrPic();

private:
    Ui::SubDialog *ui;
public:
    QString dirFile;
public:
    void get2SubWin(QString);
    static bool isColseSubWin;//窗口关闭标记

protected:
    virtual void paintEvent( QPaintEvent * e);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent* size);//窗口大小改变事件
    virtual void mousePressEvent(QMouseEvent  *event);//鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent  *event); // 鼠标移动
    virtual void mouseReleaseEvent(QMouseEvent  *);//鼠标释放事件


private:
    float zoomx=1;  //缩放系数
    float zoomy=1;  //缩放系数
    bool m_isMove; // 鼠标按下标记
    bool m_isFirst;//是否是第一次移动
    int x,y;
    QPixmap pix;
    QPoint startPos;
    QPointF     m_startPos;
    QPointF     prePos;
};

#endif // SUBDIALOG_H
