#include "choosewindow.h"
#include "ui_choosewindow.h"
#include<QString>
#include <QTextCodec>
#include<QMessageBox>




ChooseWindow::ChooseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChooseWindow)
{
    ui->setupUi(this);
    WindowFormat();
    ConnectSlots();
}
ChooseWindow::~ChooseWindow()
{
    delete ui;
}

void ChooseWindow::WindowFormat()
{
    //设置窗口大小
    this->setFixedSize(this->width(),this->height());

    ui->ModelBtn->setIcon(QIcon(":/res/model.png"));
    ui->PredictBtn->setIcon(QIcon(":/res/prediction.png"));
    //设置文本对齐格式
    ui->ModelBtn->setStyleSheet("text-align: middel");
    ui->PredictBtn->setStyleSheet("text-align: middel");

    //设置背景图
    this->setAutoFillBackground(true);
    QPixmap pixmap(":/res/backgrand.jpg");
    pixmap=pixmap.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    //校徽设置在UI界面实现

//    设置背景颜色
//    QPalette palette2(this->palette());
//    palette2.setColor(QPalette::Background, Qt::lightGray);
//    this->setPalette(palette2);

    setWindowTitle(QString("工业机器人整机性能评估软件"));
    this->setWindowIcon(QIcon(":/res/window.png"));
}

void ChooseWindow::ConnectSlots()
{
    //点击模型训练按钮，进入模型训练界面
    connect(ui->ModelBtn,&QPushButton::clicked,[=](){
        ModelSolt();
        state=true;
    });
    //点击模型评估按钮，进入预测界面
    connect(ui->PredictBtn,&QPushButton::clicked,[=](){
        if(state){
            PredictSolt();
            state=false;
        }else{
            int warning_ret=QMessageBox::warning(this,tr("温馨提示"),
                tr("为了您预测结果的准确性，请先训练模型！\n若不训练模型使用默认模型，请点击Yes？"),QMessageBox::Yes|QMessageBox::No);
            if(warning_ret==QMessageBox::Yes){PredictSolt();}
        }
    });
}

void ChooseWindow::ModelSolt()
{
    mainWin=new MainWindow;
    mainWin->setAttribute(Qt::WA_DeleteOnClose); //设置自动释放资源
    mainWin->setWidgetShow(1);//移除评估界面，显示模型训练界面

    mainWin->show();
    this->hide();
    connect(mainWin,SIGNAL(ExitWin()),this,SLOT(ShowChooseWin()));

}

void ChooseWindow::PredictSolt()
{
    mainWin=new MainWindow;
    mainWin->setAttribute(Qt::WA_DeleteOnClose); //设置自动释放资源
    mainWin->setWidgetShow(0);//移除模型训练界面，显示评估界面

    mainWin->show();
    this->hide();
    connect(mainWin,SIGNAL(ExitWin()),this,SLOT(ShowChooseWin()));
}

void ChooseWindow::ShowChooseWin()
{
    this->show();
}

