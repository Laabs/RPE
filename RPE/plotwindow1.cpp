#include "plotwindow1.h"
#include "ui_plotwindow1.h"
#include "mainwindow.h"
#include<QDebug>
#include<QMessageBox>
#include<QPixmap>
#include<QString>

PlotWindow1::PlotWindow1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow1)
{
    ui->setupUi(this);
    ConnectSlots();

}

PlotWindow1::~PlotWindow1()
{
    isColseWin=false;
    delete ui;
}

void PlotWindow1::closeEvent(QCloseEvent *)
{
    emit ExitWin1();
}

void PlotWindow1::ConnectSlots()
{
    connect(ui->CompareBtn, SIGNAL(clicked()), this, SLOT(CompareSlot()));
    connect(ui->MseBtn, SIGNAL(clicked()), this, SLOT(MseSlot()));
    connect(ui->RAdjustBtn, SIGNAL(clicked()), this, SLOT(RAdjustSlot()));
    connect(ui->WeightBtn, SIGNAL(clicked()), this, SLOT(WeightSlot()));
    connect(ui->ExitBtn_3, SIGNAL(clicked()), this, SLOT(ExitSlot()));
}

void PlotWindow1::CompareSlot()
{
    PicSelect(0);
}

void PlotWindow1::MseSlot()
{
    PicSelect(1);
}

void PlotWindow1::RAdjustSlot()
{
    PicSelect(2);
}

void PlotWindow1::WeightSlot()
{
    PicSelect(3);
}

void PlotWindow1::ExitSlot()
{
    this->close();
}

void PlotWindow1::PathSet(const QString &ExePath,const int& winId)
{
    this->ExePath=ExePath;
    this->winId=winId;
}

void PlotWindow1::WindowFormat(int winId)
{
    switch (winId) {
        case 3: //零部件
            ui->CompareBtn->setText("相对误差");
            ui->MseBtn->setText("均方误差");
            ui->RAdjustBtn->setText("校正决定系数");
            ui->Tool_5->show();
            break;
        case 4: // 工况
            ui->Tool_3->setTitle(QString("整机性能影响因素"));
            ui->CompareBtn->setText(QString("立方体边长"));
            ui->MseBtn->setText(QString("负载"));
            ui->RAdjustBtn->setText(QString("速度"));
            ui->Tool_5->hide();
            break;

        case 5:// 环境
            ui->Tool_5->show();
            break;
        default:
            break;

    }
}

bool PlotWindow1::isColseWin=false;

void PlotWindow1::PicSelect(int type)
{

    //界面三图片路径赋值
    int len=ExePath.size()-1;
    while(ExePath[len]!='/'){
        len--;
    }
    QString pathPic1,pathPic2,pathPic3,pathPic4;
    if(winId==3){
        pathPic1 = ExePath.left(len) + "/average.jpg";
        pathPic2 = ExePath.left(len) + "/MSE.jpg";
        pathPic3 = ExePath.left(len) + "/R2.jpg";
        pathPic4 = " ";
    }
    else if(winId==4){
        pathPic1 = ExePath.left(len) + "/lenConditionsPic.jpg";
        pathPic2 = ExePath.left(len) + "/loadConditionsPic.jpg";
        pathPic3 = ExePath.left(len) + "/velConditionsPic.jpg";
        pathPic4 = " ";
    }


    QString filename;
    switch (type)
    {
    case 0:
        filename = pathPic1;//比较曲线图片
        break;
    case 1:
        filename = pathPic2;//均方误差图片
        break;
    case 2:
        filename = pathPic3;//校正决定系数图片
        break;
    case 3:
        filename = pathPic4;//影响因子权重系数图片
        break;
    default:
        break;
    }

    if(filename.isEmpty())
    {
        return;
    }
    else
    {
        QImage* image= new QImage;     //创建QImage对象
        if(!image->load(filename))    //判断是否载入成功
        {
            QMessageBox::information(this,tr("错误"),tr("图像读取失败!"));
            delete image;
            return;
        }
        QPixmap pix;
        pix.scaled(ui->pic->size());//设置图片大小
        ui->pic->setScaledContents(true);//设置label全填效果
        pix.load(filename);
        ui->pic->setPixmap(pix);
    }
}


