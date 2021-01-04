#include "subdialog.h"
#include "ui_subdialog.h"
#include<QDebug>
#include<QPainter>
#include<QFileDialog>
#include<QMessageBox>
#include<QIcon>
#include <QEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QGraphicsItem>

SubDialog::SubDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubDialog)
{

    //
    qDebug()<<"jixjxijxijiaoyukaoshiyuan"<<endl;
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/res/window.png"));
    m_isMove = false;
    m_isFirst=false;
    //showErrPic();

}

bool SubDialog::isColseSubWin=false;

SubDialog::~SubDialog()
{
    qDebug()<<"delete the subWindow!!!"<<endl;
    isColseSubWin=false;
    delete ui;
}

void SubDialog::showErrPic()
{
    QString filename = dirFile;
    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        filename=dirFile+QString("/average.jpg");
        QImage* image= new QImage;     //创建QImage对象
        if(!image->load(filename))    //判断是否载入成功
        {
            QMessageBox::information(this,tr("打开图像情况"),tr("打开失败！！00"));
            delete image;
            return;
        }
        setMouseTracking(true);
        startPos = QPoint(0, 0);
        pix.scaled(ui->picLabel->size());
        ui->picLabel->setScaledContents(true);
        pix.load(filename);
        this->resize(pix.width(),pix.height());
        ui->picLabel->setAlignment(Qt::AlignCenter);
        //ui->picLabel->setPixmap(pix);
    }
}

void SubDialog::get2SubWin(QString filename)
{
    dirFile=filename;
    qDebug()<<"-------connect-----------"<<endl;
}

void SubDialog::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.drawPixmap(startPos.x(), startPos.y(), pix.scaled(pix.width()*zoomx,pix.height()*zoomy,Qt::IgnoreAspectRatio));

}

void SubDialog::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0) {
        zoomx += 0.1;
    } else {
        zoomx -= 0.1;
    }
    startPos.setX(-(event->pos().x())*(zoomx-1));
    startPos.setY(-(event->pos().y())*(zoomx-1));
    zoomy=zoomx;
    //qDebug()<<startPos;
    update();
}

void SubDialog::resizeEvent(QResizeEvent *size)
{
    startPos.setX(0);
    startPos.setY(0);
    zoomx=((float)this->width())/pix.width();
    zoomy=((float)this->height()/pix.height());
    //qDebug()<<"(x,y)="<<zoomx<<","<<zoomy<<endl;
    update();
}

void SubDialog::mousePressEvent(QMouseEvent  *event)
{
    if(event->button()== Qt::LeftButton)
        {
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
            if(m_isMove==false) m_startPos=mouse->pos();
            m_isMove = true;//标记鼠标左键被按下
            m_isFirst=true;//第一次按下
            qDebug()<<"----press---------"<<endl;
        }
        else if(event->button() == Qt::RightButton)
        {
            startPos.setX(0);
            startPos.setY(0);
            zoomx=((float)this->width())/pix.width();
            zoomy=((float)this->height()/pix.height());
            update();
            //ResetItemPos();//右击鼠标重置大小
        }
}

void SubDialog::mouseMoveEvent(QMouseEvent  *event)
{
    if(m_isMove)
    {

         QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
         if(m_isFirst){
             m_startPos.setX(mouse->x());
             m_startPos.setY(mouse->y());
             prePos=startPos;
             m_isFirst=false;
         }
         startPos.setX(prePos.x()+mouse->x() - m_startPos.x());
         startPos.setY(prePos.y()+mouse->y() - m_startPos.y());

         update();
    }
}

void SubDialog::mouseReleaseEvent(QMouseEvent  *)
{
     m_isMove = false;//标记鼠标左键已经抬起
     m_isFirst= false;
}




