#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>
#include <windows.h>
#include <string>
#include <QTime>
#include <QDateTime>
#include <QDesktopWidget>
#include<QDebug>
#include"choosewindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    CurrParmType(INVALID_PARM_TYPE),
    CurrRcdIndex(0)
{
    ui->setupUi(this);

    WindowFormat();   //主窗口格式
    ConnectSlots();   //信号与槽
    LoadData();       //历史数据
}

MainWindow::~MainWindow()
{
    //SaveRecord();
    delete ui;
}

void MainWindow::WindowFormat()
{
    setFixedSize(this->width(),this->height());
    ui->tabWidget->setCurrentIndex(0);        //默认打开模型训练界面
    ui->tabWidget->setPalette(QPalette(QPalette::Background, Qt::white));
    this->setWindowIcon(QIcon(":/res/window.png"));
    setWindowTitle(QString("核心零部件对工业机器人整机性能影响分析"));
    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::white);
    ui->CurrentFile->setPalette(pa);
    ui->CurrentParm->setPalette(pa);
    ui->CurrTestFile->setPalette(pa);
    ui->CurrentModel->setPalette(pa);
    ui->CurrentParm_2->setPalette(pa);
    ui->ErrParm->setPalette(pa);
    //相对误差默认2%
    ui->ErrSetEdit->setText(QString::number(ShowThreshold));
//////////////////////////统一按钮大小///////////////////////////////
    int widthBtn=ui->SelectTrainBtn->width()*1.4;    //键宽
    int heightBtn=ui->SelectTrainBtn->height()*1.4;  //键高
    //模型训练界面按钮
    //SelectTrainBtn//SelectModelBtn
    //TrainModelBtn//AnalyModelBtn//ExitBtn以上不定义固定大小
    ui->ConditionBtn->setFixedSize(widthBtn,heightBtn);  //工况条件按钮
    ui->ReducerBtn->setFixedSize(widthBtn,heightBtn);    //减速器按钮
    ui->ServoBtn->setFixedSize(widthBtn,heightBtn);      //伺服电机按钮
    ui->EnvironmentBtn->setFixedSize(widthBtn,heightBtn);//环境因素按钮
    ui->WholeRobotBtn->setFixedSize(widthBtn,heightBtn); //整机性能按钮
    ui->ErrSetButton->setFixedHeight(heightBtn);   //相对误差按钮
    ui->ErrSetEdit->setFixedHeight(heightBtn);     //相对误差编辑框
    //评估界面按钮
    //SelectTestBtn//测试集按钮SelectModelBtn_2//预测模型按钮
    //DoEstimateBtn//预测评估按钮AnalyModelBtn_2//预测结果分析按钮
    //SaveToFileBtn//保存按钮ExitBtn_2//退出按钮   以上不定义大小
    ui->ConditionBtn_2->setFixedSize(widthBtn,heightBtn);  //工况条件按钮
    ui->ReducerBtn_2->setFixedSize(widthBtn,heightBtn);    //减速器按钮
    ui->ServoBtn_2->setFixedSize(widthBtn,heightBtn);      //伺服电机按钮
    ui->EnvironmentBtn_2->setFixedSize(widthBtn,heightBtn);//环境因素按钮
    ui->WholeRobotBtn_2->setFixedSize(widthBtn,heightBtn); //整机性能按钮
/////////////////////////////////////////////////////////////////////////
///////////////////////////////菜单栏设置//////////////////////////////////////////////////////
    //菜单栏表头设置
    ui->menu->menuAction()->setVisible(false);
    QPushButton *mybtn1=new QPushButton("\t影响因素\n\t选择",this);// 创建按钮
    mybtn1->setStyleSheet("QPushButton{color:green; background-color:transparent;font-size:20px}");
    QFont font = mybtn1->font();
    font.setPointSize(14);
    // 菜单栏图标
    ui->ActionBtn1->setIcon(QIcon(":/res/coreparts.png"));
    ui->ActionBtn2->setIcon(QIcon(":/res/condition.png"));
    ui->ActionBtn3->setIcon(QIcon(":/res/environment.png"));
    ui->Actionhelp->setIcon(QIcon(":/res/help.png"));   //help按钮

    ui->toolBar->setStyleSheet("border:none;");
    ui->toolBar->addWidget(mybtn1); // 添加按钮，可以多次调用添加条目
    ui->toolBar->addSeparator();
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar->addAction(ui->ActionBtn1);
    ui->toolBar->addAction(ui->ActionBtn2);
    ui->toolBar->addAction(ui->ActionBtn3);
    ui->toolBar->addAction(ui->Actionhelp);
    ui->toolBar->setFloatable(false);//关闭浮动，只允许停靠
    ui->toolBar->setMovable(false);//上面所有的设置都会关掉
//////////////////////////////////////////////////////////////////////////////////////////
}

void MainWindow::ConnectSlots()
{
    //菜单栏
    connect(ui->ActionBtn1,SIGNAL(triggered()),this,SLOT(ShowUiSolt1()));
    connect(ui->ActionBtn2,SIGNAL(triggered()),this,SLOT(ShowUiSolt2()));
    connect(ui->ActionBtn3,SIGNAL(triggered()),this,SLOT(ShowUiSolt3()));
    //误差按钮（训练界面）
    connect(ui->ErrSetButton, SIGNAL(clicked()),this,SLOT(ErrSetSlot()));
    //历史数据栏（评估界面）
    connect(ui->HistoryRcdLst, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(CheckRecord()));
///////////////////////////////////模型训练界面////////////////////////////////////////////////
    //模型训练栏
    connect(ui->SelectTrainBtn, SIGNAL(clicked()), this, SLOT(CurrentTrainFileSlot()));
    connect(ui->SelectModelBtn, SIGNAL(clicked()), this, SLOT(SelectModelSlot()));
    connect(ui->TrainModelBtn,SIGNAL(clicked()),this,SLOT(TrainModelSolt()));
    connect(ui->AnalyModelBtn, SIGNAL(clicked()), this, SLOT(AnalyModelSolt()));
    connect(ui->ExitBtn, SIGNAL(clicked()), this, SLOT(ExitSlot()));
    //原始数据查看栏
    connect(ui->ReducerBtn, SIGNAL(clicked()), this, SLOT(CheckReducerSlot()));
    connect(ui->ServoBtn, SIGNAL(clicked()), this, SLOT(CheckServoSlot()));
    connect(ui->ConditionBtn, SIGNAL(clicked()), this, SLOT(CheckConditionSlot()));
    connect(ui->EnvironmentBtn, SIGNAL(clicked()), this, SLOT(CheckEnvironmentSlot()));
    connect(ui->WholeRobotBtn, SIGNAL(clicked()), this, SLOT(CheckWholeRobotSlot()));
    //训练数据表格
    connect(ui->TrainDataTbl, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(CheckSampleSlot()));
///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////评估界面///////////////////////////////////////////
    connect(ui->SelectTestBtn, SIGNAL(clicked()), this, SLOT(CurrentTestFileSlot()));
    connect(ui->SelectModelBtn_2, SIGNAL(clicked()), this, SLOT(SelectModelSlot()));
    connect(ui->DoEstimateBtn, SIGNAL(clicked()), this, SLOT(DoEstimateSlot()));
    connect(ui->AnalyModelBtn_2, SIGNAL(clicked()), this, SLOT(AnalyModelSolt2()));
    connect(ui->SaveToFileBtn, SIGNAL(clicked()), this, SLOT(SaveToFileSlot()));
    connect(ui->ExitBtn_2, SIGNAL(clicked()), this, SLOT(ExitSlot()));

    connect(ui->ConditionBtn_2, SIGNAL(clicked()), this, SLOT(CheckConditionSlot_2()));
    connect(ui->ReducerBtn_2, SIGNAL(clicked()), this, SLOT(CheckReducerSlot_2()));
    connect(ui->ServoBtn_2, SIGNAL(clicked()), this, SLOT(CheckServoSlot_2()));
    connect(ui->EnvironmentBtn_2, SIGNAL(clicked()), this, SLOT(CheckEnvironmentSlot_2()));
    connect(ui->WholeRobotBtn_2, SIGNAL(clicked()), this, SLOT(CheckWholeRobotSlot_2()));
}

void MainWindow::LoadData()
{
    LoadTrainData();

    LoadTestData();
}
/////////////////菜单栏按钮槽函数//////////////////////
void MainWindow::ShowUiSolt1(){setBtnShow(1);}
void MainWindow::ShowUiSolt2(){setBtnShow(2);}
void MainWindow::ShowUiSolt3(){setBtnShow(3);}
void MainWindow::setBtnShow(int flag)
{
//////////////格式化表格和选择选项提示/////////////
    //清空所有表格中的值
    ui->EstimateTbl->clear();
    ui->EstimateTbl->setRowCount(0);
    ui->EstimateTbl->setColumnCount(0);
    ui->TestDataTbl->clear();
    ui->TestDataTbl->setRowCount(0);
    ui->TestDataTbl->setColumnCount(0);
    ui->TrainDataTbl->clear();
    ui->TrainDataTbl->setRowCount(0);
    ui->TrainDataTbl->setColumnCount(0);
    ui->SingleSampleTbl->clear();
    ui->SingleSampleTbl->setRowCount(0);
    ui->SingleSampleTbl->setColumnCount(0);
    //格式化提示字符
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrTestFile->setText("未选择!");
    ui->CurrTestFile->setPalette(pa);
    ui->CurrentParm_2->setText("未选择!");
    ui->CurrentParm_2->setPalette(pa);
    ui->CurrentFile->setText("未选择!");
    ui->CurrentFile->setPalette(pa);
    ui->CurrentModel->setText("未选择!");
    ui->CurrentModel->setPalette(pa);
    ui->CurrentParm->setText("未选择!");
    ui->CurrentParm->setPalette(pa);
///////////////////////////////////////////////////
    //核心零部件
    if(flag==1){
        setWindowTitle(QString("核心零部件对工业机器人整机性能影响分析"));
        winId=REDUCER_PARM_TYPE;
        // 界面0
        ui->ReducerBtn->show();
        ui->ServoBtn->show();
        ui->ConditionBtn->hide();
        ui->EnvironmentBtn->hide();
        // 界面1
        ui->ReducerBtn_2->show();
        ui->ServoBtn_2->show();
        ui->ConditionBtn_2->hide();
        ui->EnvironmentBtn_2->hide();
    }
    else if(flag==2){
        setWindowTitle(QString("工况条件对工业机器人整机性能影响分析"));
        winId=CONDITION_PARM_TYPE;
        //界面0
        ui->ReducerBtn->hide();
        ui->ServoBtn->hide();
        ui->ConditionBtn->show();
        ui->EnvironmentBtn->hide();
        // 界面1
        ui->ReducerBtn_2->hide();
        ui->ServoBtn_2->hide();
        ui->ConditionBtn_2->show();
        ui->EnvironmentBtn_2->hide();
    }
    else if(flag==3){
        setWindowTitle(QString("环境因素对工业机器人整机性能影响分析"));
        winId=ENVIRONMENT_PARM_TYPE;
        //界面0
        ui->ReducerBtn->hide();
        ui->ServoBtn->hide();
        ui->ConditionBtn->hide();
        ui->EnvironmentBtn->show();
        // 界面1
        ui->ReducerBtn_2->hide();
        ui->ServoBtn_2->hide();
        ui->ConditionBtn_2->hide();
        ui->EnvironmentBtn_2->show();

    }
}
///////////////////////////////////////////////////
void MainWindow::ErrSetSlot()
{
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrentParm->setText(QString("整机性能训练结果"));
    ui->CurrentParm->setPalette(pa);

    if(ui->ErrSetEdit->text().isEmpty()) return;

    ShowThreshold=ui->ErrSetEdit->text().toDouble();
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::darkGreen);//设置颜色
    ui->ErrParm->setText(QString::number(ShowThreshold)+QString("%"));
    ui->ErrParm->setPalette(pe);

    if(EstimateDataRate.size()){

        int sumErrParm=0;
        QTableWidget *&Table=ui->SingleSampleTbl;
        QVector<QVector<QString>> &Data=TrainEstimateData;

        Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        //表格规模
        Table->setRowCount(Data.size() - 1); //除去表头
        Table->setColumnCount(Data[0].size());

        //表头H
        QStringList headers;

        for(int i = 0; i < Data[0].size(); i++)
        {
            headers << Data[0][i];
        }

        Table->setHorizontalHeaderLabels(headers);

        //QFont serifFont("Consolas", 10);
        Table->horizontalHeader()->setFont(serifFont);

        //表头V
        headers.clear();
        for(int i = 1; i < Data.size(); i++)
        {
            headers << QString("样本") + QString::number(i);
        }

        Table->setVerticalHeaderLabels(headers);
        Table->verticalHeader()->setFont(serifFont);

        //内容
        for(int i = 1; i < Data.size(); i++)
        {
            for(int j = 0; j < Data[i].size(); j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem (Data[i][j]);
                double rate=EstimateDataRate[i][j].toDouble();

                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置水平和垂直居中
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                //QFont serifFont("Consolas", 10);
                item->setFont(serifFont);
                if(rate>ShowThreshold){
                    sumErrParm++;
                    item->setTextColor(QColor(Qt::red));
                }

                Table->setItem(i - 1, j, item);
            }
        }
        //表格大小自适应于内容
        Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        QMessageBox::information(this,"超阈值警告\t\t\t",QString("\n\n\n")+QString::number(sumErrParm)+QString("个参数超出阈值\n\n\n"));
    }

}

void MainWindow::closeEvent(QCloseEvent *)
{
    emit ExitWin();
}

void MainWindow::showmianwindow(){
    this->show();
}
void MainWindow::CheckControllerSlot()
{

    CheckSample(CONTROLLER_PARM_TYPE);

}

void MainWindow::CheckServoSlot()
{

    CheckSample(SERVO_PARM_TYPE);

}

void MainWindow::CheckReducerSlot()
{

    CheckSample(REDUCER_PARM_TYPE);

}

void MainWindow::CheckConditionSlot()
{
    CheckSample(CONDITION_PARM_TYPE);
}

void MainWindow::CheckConditionSlot_2()
{
     CheckTestSample(CONDITION_PARM_TYPE);
}

void MainWindow::CheckEnvironmentSlot()
{

    CheckSample(ENVIRONMENT_PARM_TYPE);

}

void MainWindow::CheckWholeRobotSlot()
{

    CheckSample(WHOLE_ROBOT_PARM_TYPE);

}



void MainWindow::CheckControllerSlot_2()
{

    CheckTestSample(CONTROLLER_PARM_TYPE);

}

void MainWindow::CheckServoSlot_2()
{

    CheckTestSample(SERVO_PARM_TYPE);

}

void MainWindow::CheckReducerSlot_2()
{

    CheckTestSample(REDUCER_PARM_TYPE);

}

void MainWindow::CheckEnvironmentSlot_2()
{

    //CheckTestSample(ENVIRONMENT_PARM_TYPE);

    //temeprally modified

    QString path = QCoreApplication::applicationDirPath() + "/EstimateData1.csv";
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读的形式打开文件
    {
        qDebug() << "open file failed : " << path;
        return;
    }

    //表格规模
    ui->EstimateTbl->setRowCount(2);
    ui->EstimateTbl->setColumnCount(4);

    QTextStream textStream(&file);
    QString line;
    QStringList headers;

    //表头H
    line = textStream.readLine(); // 不包括“\n”的一行文本
    int i = 0;
    while(1)
    {
        QString TempStr = line.section(',', i, i);
        i++;
        if(TempStr == "")
        {
            break;
        }

        headers << TempStr;
    }

    ui->EstimateTbl->setHorizontalHeaderLabels(headers);
    ui->EstimateTbl->horizontalHeader()->setFont(serifFont);

    //表头V
    headers.clear();
    headers << QString("真实值") << QString("预测值");
    ui->EstimateTbl->setVerticalHeaderLabels(headers);
    ui->EstimateTbl->verticalHeader()->setFont(serifFont);

    //内容
    QVector<QVector<QString> > TempEstimateData(2);
    for(int i = 0; i < 2; i++)
    {
        line = textStream.readLine(); // 不包括“\n”的一行文本
        int j = 0;
        while(1)
        {
            QString TempStr = line.section(',', j, j);
            j++;
            if(TempStr == "")
            {
                break;
            }

            TempEstimateData[i].push_back(TempStr);
        }
    }

    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem (TempEstimateData[i][j]);
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            item->setFont(serifFont);
            ui->EstimateTbl->setItem(i, j, item);
        }
    }

    //表格大小
    ui->EstimateTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->EstimateTbl->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TestDataTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->TestDataTbl->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::CheckWholeRobotSlot_2()
{

    CheckTestSample(WHOLE_ROBOT_PARM_TYPE);

}

void MainWindow::CheckSample(enum PARM_TYPE type)
{
    //check first

    //no data
    if(TrainData.empty())
    {
        qDebug() << "TrainData.empty()" << endl;
        return;
    }

    //no cell selected
    if(ui->TrainDataTbl->currentItem() == NULL)
    {
        ui->CurrentParm->setText(QString("所选数据为空，请重新选择!"));
        qDebug() << "no cell selected" << endl;
        return;
    }

    //clear table
    ClearSingleSampleTbl();

    //calc
    int ParmNum = 0;
    int ParmIndex = 0;
    int Interval = 0;



    //QString TempStr;

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);

    switch (type)
    {
    case INVALID_PARM_TYPE:
        break;
    case CONTROLLER_PARM_TYPE:
        ParmNum = CONTROLLER_PARM_NUM;
        ParmIndex = 0;
        ui->CurrentParm->setText(QString("控制器"));
        ui->CurrentParm->setPalette(pa);
        break;
    case SERVO_PARM_TYPE:
        Interval = CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM;
        ParmNum = SERVO_PARM_NUM;
        ParmIndex = CONTROLLER_PARM_NUM;
        ui->CurrentParm->setText(QString("伺服电机"));
        ui->CurrentParm->setPalette(pa);
        break;
    case REDUCER_PARM_TYPE:
        Interval = CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM;
        ParmNum = REDUCER_PARM_NUM;
        //ParmIndex = 0;    //temporally modified : CONTROLLER_PARM_NUM + SERVO_PARM_NUM;
        ParmIndex=CONTROLLER_PARM_NUM + SERVO_PARM_NUM;
        ui->CurrentParm->setText(QString("减速器"));
        ui->CurrentParm->setPalette(pa);
        break;
    case CONDITION_PARM_TYPE:
        Interval = CONDITION_PARM_NUM;
        ParmNum = CONDITION_PARM_NUM;
        //ParmIndex = 0;    //temporally modified : CONTROLLER_PARM_NUM + SERVO_PARM_NUM;
        ParmIndex=0;
        ui->CurrentParm->setText(QString("工况条件"));
        ui->CurrentParm->setPalette(pa);
        break;
    case ENVIRONMENT_PARM_TYPE:
        ParmNum = ENVIRONMENT_PARM_NUM;
        ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM;
        ui->CurrentParm->setText(QString("环境因素"));
        ui->CurrentParm->setPalette(pa);
        break;
    case WHOLE_ROBOT_PARM_TYPE:
        ParmNum = WHOLE_ROBOT_PARM_NUM;
        ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
        if(winId==CONDITION_PARM_TYPE){
            ParmNum = WHOLE_ROBOT_CONDITION_PARM_NUM;
            ParmIndex = CONDITION_PARM_NUM;
        }
        ui->CurrentParm->setText(QString("整机性能真实值"));
        ui->CurrentParm->setPalette(pa);
        break;
    default:
        break;
    }

    if(ParmNum == 0)
    {
        return;
    }

    int row = ui->TrainDataTbl->currentRow();
    //表格规模
    if(type != INVALID_PARM_TYPE && type != WHOLE_ROBOT_PARM_TYPE && type != ENVIRONMENT_PARM_TYPE  && type != CONDITION_PARM_TYPE)
    {
        ui->SingleSampleTbl->setRowCount(AXIS_NUM);
    }
    else
    {
        ui->SingleSampleTbl->setRowCount(1);
    }

    ui->SingleSampleTbl->setColumnCount(ParmNum);


    //表头H
    QStringList headers;

    try {
        for(int i = ParmIndex; i < ParmIndex + ParmNum; i++)
        {
            if(i>=TrainData[0].size()){
                throw __LINE__;
            }
            headers << TrainData[0][i];
        }

    } catch (int errLine) {
        QPalette errpa;
        errpa.setColor(QPalette::WindowText, Qt::red);
        ui->CurrentParm->setPalette(errpa);
        //ui->CurrentParm
        ui->CurrentParm->setText(QString("索引超出矩阵维度:err line %1").arg(errLine));
        qDebug()<<"索引超出矩阵维度";
        return;
    }


    ui->SingleSampleTbl->setHorizontalHeaderLabels(headers);
    //QFont serifFont("Consolas", 10);
    ui->SingleSampleTbl->horizontalHeader()->setFont(serifFont);

    //表头V
    headers.clear();
    if(type != INVALID_PARM_TYPE && type != ENVIRONMENT_PARM_TYPE && type != WHOLE_ROBOT_PARM_TYPE && type != CONDITION_PARM_TYPE)
    {
        QString TempStr(tr("AXIS"));
        for(int i = 0; i < AXIS_NUM; i++)
        {
            headers << TempStr + QString::number(i + 1);
        }
    }
    else if(type == CONDITION_PARM_TYPE){
        headers << QString("样本")+QString::number(row+1);
    }
    else
    {
        headers << QString("整机");
    }

    ui->SingleSampleTbl->setVerticalHeaderLabels(headers);
    ui->SingleSampleTbl->verticalHeader()->setFont(serifFont);

    //内容
    //whole robot
    if(type == WHOLE_ROBOT_PARM_TYPE)
    {
        int TrainDataR=TrainData.size();
        int TrainDataL=TrainData[0].size();
        try {
            for(int j = 0; j < ParmNum; j++)
            {
                if(row + 1>TrainDataR || j + ParmIndex>TrainDataL){
                    throw __LINE__;
                }
                QTableWidgetItem *item = new QTableWidgetItem (TrainData[row + 1][j + ParmIndex]);
                //qDebug() << TrainData[row + 1][j + ParmIndex];
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                //QFont serifFont("Consolas", 10);
                item->setFont(serifFont);
                ui->SingleSampleTbl->setItem(0, j, item);
            }

        } catch (int errLine) {
            QPalette errpa;
            errpa.setColor(QPalette::WindowText, Qt::red);
            ui->CurrentParm->setPalette(errpa);
            //ui->CurrentParm
            ui->CurrentParm->setText(QString("索引超出矩阵维度:err line %1").arg(errLine));
            qDebug()<<"索引超出矩阵维度";
            return;
        }
    }
    //environment
    else if(type == ENVIRONMENT_PARM_TYPE)
    {

    }
    else if(type != INVALID_PARM_TYPE)
    {
        int rows=AXIS_NUM;
        if(type==CONDITION_PARM_TYPE) rows=1;
        try {
            for(int i = 0; i < rows; i++)
            {
                for(int j = 0; j < ParmNum; j++)
                {
                    if(row + 1>TrainData.size() || j + ParmIndex + Interval * i>TrainData[row+1].size()){
                        throw __LINE__;
                    }
                    QTableWidgetItem *item = new QTableWidgetItem (TrainData[row + 1][j + ParmIndex + Interval * i]);
                    //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                    //QFont serifFont("Consolas", 10);
                    item->setFont(serifFont);
                    ui->SingleSampleTbl->setItem(i, j, item);
                }
                //qDebug() << endl;
            }

        } catch (int errLine) {
            QPalette errpa;
            errpa.setColor(QPalette::WindowText, Qt::red);
            ui->CurrentParm->setPalette(errpa);
            //ui->CurrentParm
            ui->CurrentParm->setText(QString("索引超出矩阵维度:err line %1").arg(errLine));
            qDebug()<<"索引超出矩阵维度";
            return;

        }
    }

    //表格大小自适应于内容
    ui->SingleSampleTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    qDebug() << "111";
}

void MainWindow::ClearSingleSampleTbl()
{
    ui->SingleSampleTbl->clear();
    ui->SingleSampleTbl->setRowCount(0);
    ui->SingleSampleTbl->setColumnCount(0);
    ui->CurrentParm->setText(QString("未选择 ！"));

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->CurrentParm->setPalette(pa);
}

void MainWindow::ClearTestDataTbl()
{
    ui->TestDataTbl->clear();
    ui->TestDataTbl->setRowCount(0);
    ui->TestDataTbl->setColumnCount(0);
}

void MainWindow::ClearTrainDataTbl()
{
    ui->TrainDataTbl->clear();
    ui->TrainDataTbl->setRowCount(0);
    ui->TrainDataTbl->setColumnCount(0);
}

void MainWindow::ClearEstimateTbl()
{
    ui->EstimateTbl->clear();
    ui->EstimateTbl->setRowCount(0);
    ui->EstimateTbl->setColumnCount(0);
}

void MainWindow::CurrentTrainFileSlot()
{
    QString TempPath = QFileDialog::getOpenFileName(this, "Select Train Data File", QCoreApplication::applicationDirPath(), "*.csv");
    if(TempPath == nullptr)
    {
        return;
    }

    CurrentTrainFile = TempPath;
    int len=CurrentTrainFile.size()-1;
    while(CurrentTrainFile[len]!='/'){
        len--;
    }
    exePath_pic=CurrentTrainFile.left(len);

    ShowTrainData();

}

void MainWindow::ShowTrainData()
{
    QString TrainFileName = CurrentTrainFile.section('/', -1);
    ui->CurrentFile->setText(TrainFileName);
    //qDebug()<<"-----------------"<<TrainFileName;
    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrentFile->setPalette(pa);


    ReadTabFile(CurrentTrainFile,TrainData);


    if(TrainData.size() < 2)
    {
        QMessageBox::information(this, QString("错误"), QString("训练数据选择错误，请重新选择！"));
        return;
    }

    ShowTabData(TrainData,ui->TrainDataTbl,1);

    ClearSingleSampleTbl();

}



void MainWindow::CurrentTestFileSlot()
{
    static QString TempPath="";
    PathSelectDir(TempPath,"*.csv");

    //QString TempPath = QFileDialog::getOpenFileName(this, "Select Test Data File", QCoreApplication::applicationDirPath(), "*.csv");
    if(TempPath == nullptr)
    {
        qDebug() << "Test Data File Is Empty" << endl;
        return;
    }

    QVector<QVector<QString> > TempTestData;
    ReadTabFile(TempPath,TempTestData);


    if(TempTestData.size() < 2)
    {
        qDebug() << TestData.size()<<" ->TestData.size() < 2" << endl;
        QMessageBox::information(this, QString("错误"), QString("测试数据文件选择错误，请重新选择！"));
        return;
    }



    for(int i = 0; i < TempTestData.size(); i++)
    {
        bool isTestData=false;
        if(winId==CONDITION_PARM_TYPE){
            isTestData=TempTestData[i].size()!=(WHOLE_ROBOT_CONDITION_PARM_NUM+CONDITION_PARM_NUM+1);
        }
        else{
            isTestData=TempTestData[i].size() != PARM_TOTAL_NUM && TempTestData[i].size() != PARM_TOTAL_NUM - WHOLE_ROBOT_PARM_NUM;
        }

        if(isTestData)
        {
            qDebug() << "Parm Num Error : " << TempTestData[i].size() << endl;  // << ' ' << PARM_TOTAL_NUM << endl;
            QMessageBox::information(this, QString("错误"), QString("测试数据文件选择错误，请重新选择！"));
            return;
        }
    }

    //save record first

    SaveRecord();

    //replace after no fault

    CurrentTestFile = TempPath;

    TestData = TempTestData;

    ui->CurrTestFile->setText(CurrentTestFile.section('/', -1));

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrTestFile->setPalette(pa);

    //History Record : TO DO !

    AddRecord();

    if(winId==CONDITION_PARM_TYPE){
        CheckTestSample(CONDITION_PARM_TYPE);
    }
    else{
        CheckTestSample(SERVO_PARM_TYPE);
    }

    ClearEstimateTbl();
}

void MainWindow::CheckTestSample(enum PARM_TYPE type)
{
    //check first
    //no data
    if(TestData.empty())
    {
        return;
    }

    //clear table
    ClearTestDataTbl();

    //calc
    int ParmNum = 0;
    int ParmIndex = 0;
    int TestDataRows=TestData.size()-1;//测试数据列数
    int Interval = CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM;
    //QString TempStr;

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->ErrParm->setText(QString::number(ShowThreshold)+QString("%"));
    //ui->ErrParm->setPalette(QPalette(QPalette::WindowText, Qt::darkGreen));
    ui->ErrParm->setPalette(pa);
    switch (type)
    {
    case INVALID_PARM_TYPE:
        break;
        ui->CurrentParm_2->setText(QString("未选择 ！"));
        ui->CurrentParm_2->setPalette(QPalette(QPalette::WindowText, Qt::red));
    case CONTROLLER_PARM_TYPE:
        ParmNum = CONTROLLER_PARM_NUM;
        ParmIndex = 0;
        ui->CurrentParm_2->setText(QString("控制器"));
        ui->CurrentParm_2->setPalette(pa);
        break;
    case SERVO_PARM_TYPE:
        ParmNum = SERVO_PARM_NUM;
        ParmIndex = CONTROLLER_PARM_NUM;
        ui->CurrentParm_2->setText(QString("伺服电机"));
        ui->CurrentParm_2->setPalette(pa);
        break;
    case REDUCER_PARM_TYPE:
        ParmNum = REDUCER_PARM_NUM;
        ParmIndex = CONTROLLER_PARM_NUM + SERVO_PARM_NUM;   //temporally modified : CONTROLLER_PARM_NUM + SERVO_PARM_NUM;
        ui->CurrentParm_2->setText(QString("减速器"));
        ui->CurrentParm_2->setPalette(pa);
        break;
    case CONDITION_PARM_TYPE:
        ParmNum = CONDITION_PARM_NUM;
        ParmIndex = 1;   //temporally modified : CONTROLLER_PARM_NUM + SERVO_PARM_NUM;
        ui->CurrentParm_2->setText(QString("工况条件"));
        ui->CurrentParm_2->setPalette(pa);
        break;

    case ENVIRONMENT_PARM_TYPE:
        ParmNum = ENVIRONMENT_PARM_NUM;
        ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM;
        ui->CurrentParm_2->setText(QString("环境因素"));
        ui->CurrentParm_2->setPalette(pa);
        break;
    case WHOLE_ROBOT_PARM_TYPE:
        if(winId==CONDITION_PARM_TYPE){
            ParmNum = WHOLE_ROBOT_CONDITION_PARM_NUM;
            ParmIndex = CONDITION_PARM_NUM+1;
        }
        else{
            ParmNum = WHOLE_ROBOT_PARM_NUM;
            ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
        }
        ui->CurrentParm_2->setText(QString("整机性能期望值"));
        ui->CurrentParm_2->setPalette(pa);
        break;
    default:
        break;
    }

    if(ParmNum == 0)
    {
        return;
    }

    //int row = ui->TrainDataTbl->currentRow();

    //表格规模
    if(type != INVALID_PARM_TYPE && type != WHOLE_ROBOT_PARM_TYPE && type != ENVIRONMENT_PARM_TYPE && type != CONDITION_PARM_TYPE)
    {
        ui->TestDataTbl->setRowCount(TestDataRows*AXIS_NUM);
    }
    else
    {
        ui->TestDataTbl->setRowCount(TestDataRows);
    }

    ui->TestDataTbl->setColumnCount(ParmNum+1);//有一列用于存储表头，所以必须 +1


    //表头H
    QStringList headers;
    headers<<"";
    for(int i = ParmIndex; i < ParmIndex + ParmNum; i++)
    {
        headers << TestData[0][i];
    }
    ui->TestDataTbl->setHorizontalHeaderLabels(headers);
    //QFont serifFont("Consolas", 10);
    ui->TestDataTbl->horizontalHeader()->setFont(serifFont);

    //表头V
    headers.clear();

    ui->TestDataTbl->verticalHeader()->hide();//隐藏表头
    // 设置表格首行颜色
    for(int row=0;row<TestDataRows;row++){
        if(type == WHOLE_ROBOT_PARM_TYPE)
        {

            //for(int row=0;row<TestDataRows;row++){
                QTableWidgetItem *item = new QTableWidgetItem (QString("第")+QString::number(row + 1)+QString("组真实值"));
                //qDebug() << TrainData[row + 1][j + ParmIndex];
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                //QFont serifFont("Consolas", 10);
                item->setFont(serifFont);
                if(row%2==0){
                    item->setBackground(QBrush( QColor(255,227,132)));
                }
                else{
                    item->setBackground(QBrush( QColor(189,252,201)));
                }
                ui->TestDataTbl->setItem(row, 0, item);
            //}

        }
        else if(type == CONDITION_PARM_TYPE){
            //qDebug()<<TestDataRows<<endl;
            for(int i=0;i<1;i++){
                QTableWidgetItem *item = new QTableWidgetItem ("测试样本"+QString::number(row + 1));
                //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                item->setFont(serifFont);
                if(row%2==0){
                    item->setBackground(QBrush( QColor(255,227,132)));
                }
                else{
                    item->setBackground(QBrush( QColor(189,252,201)));
                }
                ui->TestDataTbl->setItem(row, 0, item);
            }

        }
        else{
            for(int i=0;i<AXIS_NUM;i++){
                QTableWidgetItem *item = new QTableWidgetItem ("AXIS"+QString::number(i + 1)+"_"+QString::number(row + 1));
                //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                item->setFont(serifFont);
                if(row%2==0){
                    item->setBackground(QBrush( QColor(255,227,132)));
                }
                else{
                    item->setBackground(QBrush( QColor(189,252,201)));
                }
                ui->TestDataTbl->setItem(i+row*AXIS_NUM, 0, item);
            }
        }

    }

    //内容
    //whole robot
    if(type == WHOLE_ROBOT_PARM_TYPE)
    {
#if 1
        if(winId==CONDITION_PARM_TYPE){
            for(int row=0;row<TestDataRows;row++){
                for(int j = ParmIndex; j < ParmNum+ParmIndex; j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem (TestData[1+row][j]);
                    //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                    //QFont serifFont("Consolas", 10);
                    item->setFont(serifFont);
                    ui->TestDataTbl->setItem(row, j+1-ParmIndex, item);
                }
            }
        }
        else{
            for(int row=0;row<TestDataRows;row++){
                for(int j = 1; j < ParmNum+1; j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem (TestData[row+1][j-1 + ParmIndex]);
                    //qDebug() << TrainData[row + 1][j + ParmIndex];
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                    //QFont serifFont("Consolas", 10);
                    item->setFont(serifFont);
                    ui->TestDataTbl->setItem(row, j, item);
                }
            }
        }
#endif

    }
    //condition
    else if(type == CONDITION_PARM_TYPE){
        qDebug()<<"------------------condition----------"<<endl;
        for(int row=0;row<TestDataRows;row++){
            for(int i = 0; i < 1; i++)
            {
                for(int j = 1; j < ParmNum+1; j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem (TestData[1+row][j]);
                    //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                    //QFont serifFont("Consolas", 10);
                    item->setFont(serifFont);
                    ui->TestDataTbl->setItem(row, j, item);
                }
                //qDebug() << endl;
            }
        }
    }
    //environment
    else if(type == ENVIRONMENT_PARM_TYPE)
    {

    }
    //other valid
    else if(type != INVALID_PARM_TYPE)
    {
        //qDebug()<<"测试数据集行列数："<<Interval;
        for(int row=0;row<TestDataRows;row++){
            for(int i = 0; i < AXIS_NUM; i++)
            {
                for(int j = 1; j < ParmNum+1; j++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem (TestData[1+row][j-1 + ParmIndex + Interval * i]);
                    //qDebug() << TrainData[row + 1][j + ParmIndex + Interval * i];
                    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                    //QFont serifFont("Consolas", 10);
                    item->setFont(serifFont);
                    ui->TestDataTbl->setItem(i+AXIS_NUM*row, j, item);
                }
                //qDebug() << endl;
            }
        }

    }
    //表格大小自适应于内容
    ui->TestDataTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::SelectModelSlot()
{
    //ExePath.clear();
    PathSelectDir(ExePath,"*.exe");//工作目录选择
    // ExePath = QFileDialog::getOpenFileName(this, "Select Model EXE", QCoreApplication::applicationDirPath(), "*.exe");
    if(ExePath == NULL)
    {
        qDebug() << "Invalid Path";
        return;
    }


    QString Model = ExePath.section('/', -1);
    QString CartModel = "cart.exe";
    QString NetModel = "BPNN.exe";
    QString CanditionModel="condition.exe";

    bool isModel=false;
    switch (winId) {
        case REDUCER_PARM_TYPE:
            if(Model == CartModel || Model == NetModel){
               isModel=true;
            }

            break;
        case CONDITION_PARM_TYPE:
            if(Model == CanditionModel){
               isModel=true;
            }
            break;
        case ENVIRONMENT_PARM_TYPE:

            break;

    }

    if(!isModel)
    {
        qDebug() << "Invalid Exe : " << Model;
        ui->CurrentModel->setText(QString("未选择 ！"));
        //color
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::red);
        ui->CurrentModel->setPalette(pa);
        QMessageBox::information(this, QString("错误"), QString("模型选择错误，请重新选择！"));
        return;
    }

    ui->CurrentModel->setText(Model);

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrentModel->setPalette(pa);
    //qDebug()<<"ExePath:"<<ExePath<<endl;

    //qDebug()<<"Model Select Finish"<<endl;


}

void MainWindow::DoEstimateSlot()
{

    if(CurrentTestFile == NULL)
    {
        qDebug() << "CurrentTestFile Is Empty !" ;
        QMessageBox::information(this, QString("错误"), QString("尚无测试数据，请先选择测试数据文件！"));
        return;
    }

    if(ExePath == NULL)
    {
        qDebug() << "ExePath Is Empty !" ;
        QMessageBox::information(this, QString("错误"), QString("尚无模型，请先选择算法模型"));
        return;
    }

    int len=ExePath.size()-1;
    while(ExePath[len]!='/'){
        len--;
    }

    QString path = ExePath.left(len) + "/EstimateData.csv";
    if(winId==CONDITION_PARM_TYPE){
        path = ExePath.left(len) + "/resultConditions.csv";
        QFile::remove(ExePath.left(len)+"/resultConditions.csv");
        QFile::remove(ExePath.left(len)+"/lenConditionsPic.jpg");
        QFile::remove(ExePath.left(len)+"/loadConditionsPic.jpg");
        QFile::remove(ExePath.left(len)+"/velConditionsPic.jpg");
    }
    else{
        path = ExePath.left(len) + "/EstimateData.csv";
        QFile::remove(ExePath.left(len)+"/EstimateData.csv");
        QFile::remove(ExePath.left(len)+"/MSE.jpg");
        QFile::remove(ExePath.left(len)+"/R2.jpg");
        QFile::remove(ExePath.left(len)+"/average.jpg");
    }

    QStringList arguments;
    arguments << CurrentTestFile;
    qDebug()<<CurrentTestFile<<endl;
    QProcess::startDetached(ExePath, arguments);

    //tongbu TO DO:
    //StatueModelEdit
    Sleep(5000);

    // 读取数据
    ReadTabFile(path,EstimateData);
    if(EstimateData.empty())
    {
        QMessageBox::information(this, QString("错误"), QString("无预估结果！"));
        return;
    }

    // 获取表头
    QVector<QString> heads;//头插
    int ParmIndex =  0;
    if(winId==CONDITION_PARM_TYPE){
        ParmIndex=1+CONDITION_PARM_NUM;// 第一行无效
    }
    else{
        ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
    }

    for(int i = ParmIndex; i < TestData[0].size(); i++)
    {
        heads.push_back(TestData[0][i]);
    }
    EstimateData.insert(EstimateData.begin(),heads);

    // 显示表格
    ShowTabData(EstimateData,ui->EstimateTbl,0);

    if(winId==REDUCER_PARM_TYPE){
        exePath_pic=ExePath.left(len) + "/cart_pic";
        QProcess::execute(exePath_pic, arguments);
    }
    //qDebug()<<"------------xxxxxxxxxxxx:"<<ExePath<<endl;

    //Sleep(10000);
    //绘图窗口相关
    //plotWin->PathSet(ExePath);


}

void MainWindow::SaveToFileSlot()
{
    if(ui->EstimateTbl->item(0, 0) == NULL)
    {
        qDebug() << "Table Is Empty !";
        return;
    }

    QString SaveFilePath = QFileDialog::getSaveFileName(this, "Save File", QCoreApplication::applicationDirPath(), "*.csv");
    if(SaveFilePath == NULL)
    {
        qDebug() << "Invalid Path !";
        return;
    }

    QFile ResultFile(SaveFilePath);

    if(!ResultFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "File Open Failed !";
        return;
    }

    QTextStream OutputStream(&ResultFile);
    OutputStream << ',';

    int Cols = ui->EstimateTbl->horizontalHeader()->count();
    for(int i = 0; i < Cols; i++)
    {
        OutputStream << ui->EstimateTbl->horizontalHeaderItem(i)->text() << ',';
    }
    OutputStream << endl;

    int Rows = ui->EstimateTbl->verticalHeader()->count();
    for(int i = 0; i < Rows; i++)
    {
        OutputStream << ui->EstimateTbl->verticalHeaderItem(i)->text() << ',';
        for(int j = 0; j < Cols; j++)
        {
            OutputStream << ui->EstimateTbl->item(i, j)->text() << ',';
        }
        OutputStream << endl;
    }

    ResultFile.close();

}

void MainWindow::AddRecord()
{
    QString Cnt = QString::number(ui->HistoryRcdLst->count() + 1);
    QString FileName = CurrentTestFile.section('/', -1);
    QString Time = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    QString ItemName = Cnt + ":" + FileName + "\n" + Time;
    ui->HistoryRcdLst->addItem(ItemName);
    CurrRcdIndex = ui->HistoryRcdLst->count() - 1;

    ui->HistoryRcdLst->setFont(serifFont);

    //New Record Has No ExePath
    // ExePath.clear();
    ui->CurrentModel->setText(QString("未选择 ！"));

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::red);
    ui->CurrentModel->setPalette(pa);
}

void MainWindow::SaveRecord()
{
    if(ui->HistoryRcdLst->count() == 0)
    {
        qDebug() << "No Rcd, No Save" << endl;
        return;
    }

    QString RcdFilePath = QCoreApplication::applicationDirPath() + "/rcd";
    QString RcdName = ui->HistoryRcdLst->item(CurrRcdIndex)->text();
    RcdFilePath += RcdName[0];
    qDebug() << RcdFilePath;

    QFile RcdFile(RcdFilePath);

    if(!RcdFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "RcdFile Open Failed ! " << RcdFilePath;
        return;
    }

    QTextStream OutputStream(&RcdFile);

    OutputStream << RcdName << endl;
    OutputStream << CurrentTestFile << endl;
    OutputStream << ExePath << endl;

    //Estimate Table
    if(ui->EstimateTbl->item(0, 0) != NULL)
    {
        int Cols = ui->EstimateTbl->horizontalHeader()->count();
        int Rows = ui->EstimateTbl->verticalHeader()->count();

        for(int i = 0; i < Rows; i++)
        {
            //OutputStream << ui->EstimateTbl->verticalHeaderItem(i)->text() << ',';
            for(int j = 1; j < Cols; j++)
            {
                OutputStream << ui->EstimateTbl->item(i, j)->text() << ',';
            }
            OutputStream << endl;
        }
    }

    RcdFile.close();
}

void MainWindow::ExitSlot()
{
    SaveRecord();
    SaveTrainData();
    close();
}

void MainWindow::AnalyModelSolt()
{
    qDebug()<<"************xxxxx:"<<ExePath<<endl;
    if(ExePath==NULL && CurrentTrainFile==NULL){
        QMessageBox::information(this, QString("错误"), QString("图片路径为空!"));
        return;
    }

    if(plotWin->isColseWin == false){
        // 训练绘图
        int len=ExePath.size()-1;
        while(ExePath[len]!='/'){
            len--;
        }

        //analyze文件路径接口
        if(winId==REDUCER_PARM_TYPE){
            exePath_pic = ExePath.left(len) + "/cart_pic.exe";
            QStringList arguments;
            arguments<<CurrentTrainFile;
            QProcess::execute(exePath_pic, arguments);
        }
        else if(winId==CONDITION_PARM_TYPE){
            qDebug()<<ExePath<<endl;
        }


        plotWin=new PlotWindow1;
        plotWin->PathSet(ExePath,winId);
       // plotWin->setModal(true);   //模态
        plotWin->setWindowTitle("工业机器人整机性能评估软件模型分析");
        plotWin->setWindowIcon(QIcon(":/res/window.png"));
        plotWin->setAttribute(Qt::WA_DeleteOnClose); //设置自动释放资源

        plotWin->WindowFormat(winId);
        plotWin->show();
        this->hide();//预测窗口隐藏
        plotWin->isColseWin=true;
        connect(plotWin,SIGNAL(ExitWin1()),this,SLOT(showmianwindow()));
    }

    //显示图片
}

void MainWindow::AnalyModelSolt2()
{
    if(winId==CONDITION_PARM_TYPE){
        AnalyModelSolt();
    }
    else{
        if(ExePath.isEmpty() || subWin->isColseSubWin==true) return;
        int len=ExePath.size()-1;
        while(ExePath[len]!='/'){
            len--;
        }
        subWin = new SubDialog();
        subWin->setModal(true);   //模态
        subWin->setWindowTitle("误差曲线");
        subWin->setAttribute(Qt::WA_DeleteOnClose); //设置自动释放资源
        subWin->get2SubWin(ExePath.left(len));
        subWin->showErrPic();
        subWin->show();
        subWin->isColseSubWin==true;
    }


}


void MainWindow::CheckRecord()
{
    //SaveRecord();
    ReadRecord();
}

void MainWindow::TrainModelSolt()
{

    // 模型训练
    if(ExePath==NULL){
        QMessageBox::information(this,"错误","请选择算法模型");
        return;
    }
    if(CurrentTrainFile==NULL){
        QMessageBox::information(this,"错误","请选择训练集");
        return;
    }
    QStringList arguments;

    qDebug()<<"exePath:"<<ExePath<<endl;
    qDebug()<<"arguments:"<<arguments<<endl;
    QProcess::execute(ExePath, arguments);
    QMessageBox::information(this,"提示","模型训练结束");



    int len=ExePath.size()-1;
    QString path;
    while(ExePath[len]!='/'){
        len--;
    }

    int ParmIndex =0;

    switch (winId) {
        case REDUCER_PARM_TYPE:
            path = ExePath.left(len) + "/EstimateData.csv";
            ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
            break;
        case CONDITION_PARM_TYPE:
            path = ExePath.left(len) + "/resultConditions.csv";
            ParmIndex = CONDITION_PARM_NUM;
            break;
        case ENVIRONMENT_PARM_TYPE:

            break;

    }

    TrainEstimateData.clear();
    // 读取数据
    ReadTabFile(path,TrainEstimateData);
    if(TrainEstimateData.empty())
    {
        QMessageBox::information(this, QString("错误"), QString("无预估结果！"));
        return;
    }

    // 获取表头
    QVector<QString> heads;//头插

    for(int i = ParmIndex; i < TrainData[0].size(); i++)
    {
        heads.push_back(TrainData[0][i]);
    }
    TrainEstimateData.insert(TrainEstimateData.begin(),heads);

    // 显示表格
    ShowTabData(TrainEstimateData,ui->SingleSampleTbl,1);


    int TrainDataRows=TrainData.size();//测试数据列数
    EstimateDataRate.clear();
    EstimateDataRate=QVector<QVector<QString>>(TrainEstimateData.size());

    for(int row=1;row<TrainDataRows;row++){
        for(int i = 0; i < TrainEstimateData[row].size(); i++)
        {
            double data1=TrainEstimateData[row][i].toDouble(); //预测值
            double data2=TrainData[row][i + ParmIndex].toDouble();//真实值
            double rate=data2==0 ? 0:100*abs(data1-data2)/data2;
            EstimateDataRate[row].push_back(QString::number(rate));
        }
    }


    //标红单元格

    ErrSetSlot();

}

void MainWindow::ReadRecord(bool flag /* = false*/)
{
    int TempIndex = 0;
    if(!flag)
    {
        TempIndex = ui->HistoryRcdLst->currentRow();
    }

    QString RcdPath = QCoreApplication::applicationDirPath() + "/rcd";
    RcdPath += QString::number(TempIndex + 1);

    QFile RcdFile(RcdPath);
    if(!RcdFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "RcdFile Open Failed : " << RcdPath << endl;
        return;
    }

    QTextStream Input(&RcdFile);

    //Read File

    //1st and 2nd line is Record Name
    Input.readLine();
    Input.readLine();

    //3rd line is Test File Path
    CurrentTestFile = Input.readLine();
    qDebug() << "CurrentTestFile : " << CurrentTestFile << endl;

    //4th line is Model
    ExePath = Input.readLine();
    qDebug() << "ExePath : " << ExePath << endl;

    // *** Test Data Table Content ***

    QVector<QVector<QString> > TempTestData;
    ReadTabFile(CurrentTestFile,TempTestData);
    QFile CSVFile(CurrentTestFile);
    if(!CSVFile.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读的形式打开文件
    {
        return;
    }

    if(TempTestData.size() < 2)
    {
        qDebug() << "TestData.size() < 2" << endl;
        QMessageBox::information(this, QString("错误"), QString("测试数据文件错误，请重新选择！"));
        return;
    }

    for(int i = 0; i < TempTestData.size(); i++)
    {
        qDebug()<<TempTestData[i].size()<<endl;
        if(TempTestData[i].size() != PARM_TOTAL_NUM && TempTestData[i].size() != PARM_TOTAL_NUM - WHOLE_ROBOT_PARM_NUM)
        {
            qDebug() << "Parm Num Error : " << TempTestData[i].size() << endl;  // << ' ' << PARM_TOTAL_NUM << endl;
            QMessageBox::information(this, QString("错误"), QString("测试数据文件错误，请重新选择！"));
            return;
        }
    }

    TestData = TempTestData;

    ui->CurrTestFile->setText(CurrentTestFile.section('/', -1));

    //color
    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::darkGreen);
    ui->CurrTestFile->setPalette(pa);

    if(ExePath == NULL)
    {
        ui->CurrentModel->setText(QString("未选择 ！"));
        //color
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::red);
        ui->CurrentModel->setPalette(pa);
    }
    else
    {
        ui->CurrentModel->setText(ExePath.section('/', -1));
        //color
        QPalette pa;
        pa.setColor(QPalette::WindowText, Qt::darkGreen);
        ui->CurrentModel->setPalette(pa);
    }

    CheckTestSample(SERVO_PARM_TYPE);

    // *** Test Data Table Content ***


    // *** Estimate Table Content ***

    ClearEstimateTbl();

    QVector<QVector<QString> > TempEstimateData;
    ReadTabFile(RcdPath,TempEstimateData);


    if(!TempEstimateData.empty() && !TestData.empty())
    {
        // 获取表头
        QVector<QString> heads;//头插
        int ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
        for(int i = ParmIndex; i < TestData[0].size(); i++)
        {
            heads.push_back(TestData[0][i]);
        }
        TempEstimateData.insert(TempEstimateData.begin(),heads);

        // 显示表格
        ShowTabData(TempEstimateData,ui->EstimateTbl,0);

    }
    // *** Estimate Table Content ***

    CurrRcdIndex = TempIndex;
    qDebug() << CurrRcdIndex << endl;

    //ClearEstimateTbl();
}

void MainWindow::LoadTrainData()
{
    QString RcdPath = QCoreApplication::applicationDirPath() + "/rcd0";
    QFile RcdFile(RcdPath);

    if(!RcdFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open File Failed : " << RcdPath << endl;
        return;
    }

    QTextStream Input(&RcdFile);

    CurrentTrainFile = Input.readLine();

    ShowTrainData();

    //sample

    int row = Input.readLine().toInt();
    int col = Input.readLine().toInt();

    if(row == -1 || col == -1)
    {
        return;
    }

    //qDebug() << "row, col :" << row << ", " << col << endl;

    ui->TrainDataTbl->setCurrentCell(row, col);

    CheckSample(SERVO_PARM_TYPE);
}

void MainWindow::LoadTestData()
{
    QString RcdPath = QCoreApplication::applicationDirPath() + "/rcd";
    int Cnt = 1;

    while(1)
    {
        QString TempPath = RcdPath + QString::number(Cnt);
        QFile RcdFile(TempPath);

        if(!RcdFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Open File Failed : " << TempPath << endl;
            break;
        }

        QTextStream Input(&RcdFile);

        QString FileName = Input.readLine();
        QString Time = Input.readLine();
        QString RcdName = FileName + "\n" + Time;

        ui->HistoryRcdLst->addItem(RcdName);
        ui->HistoryRcdLst->setFont(serifFont);

        Cnt++;

        RcdFile.close();

    }

    if(Cnt != 1)
    {
        //ReadRecord(true);
    }

}

void MainWindow::SaveTrainData()
{
    if(CurrentTrainFile == NULL)
    {
        return;
    }

    QString TrainDataSavePath = QCoreApplication::applicationDirPath() + "/rcd0";

    QFile TrainDataSaveFile(TrainDataSavePath);

    if(!TrainDataSaveFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        qDebug() << "File Open Failed : " << TrainDataSavePath << endl;
        return;
    }

    QTextStream Output(&TrainDataSaveFile);

    Output << CurrentTrainFile << endl;

    int row = -1;
    int col = -1;

    if(ui->CurrentParm->text() != QString("未选择 ！"))
    {
        row = ui->TrainDataTbl->currentRow();
        col = ui->TrainDataTbl->currentColumn();
    }

    Output << row << endl;
    Output << col << endl;

    TrainDataSaveFile.close();

}


void MainWindow::PathSelectDir(QString& myDir,QString type)
{
    if(myDir==""){
        myDir = QFileDialog::getOpenFileName(this, "Select Test Data File", QCoreApplication::applicationDirPath(),type);
    }else{
        myDir = QFileDialog::getOpenFileName(this, "Select Test Data File", myDir, type);
    }
}

void MainWindow::ReadTabFile(const QString &path, QVector<QVector<QString>> &Table)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) //以只读的形式打开文件
    {
        qDebug() << "open file failed : " << path;
        return;
    }

    Table.clear();
    QTextStream textStream(&file);
    QString line;

    int len=path.size()-1;
    QString mystr;
    while(path[len]!='/'){
        len--;
    }
    for(int i=0;i<3;i++){
        mystr.push_back(path[len+i+1]);
    }
    if(mystr=="rcd"){
        line = textStream.readLine(); // 不包括“\n”的一行文本
        line = textStream.readLine(); // 不包括“\n”的一行文本
        line = textStream.readLine(); // 不包括“\n”的一行文本
        line = textStream.readLine(); // 不包括“\n”的一行文本
        qDebug()<<"------------rcd----"<<endl;
    }



    while(!textStream.atEnd())
    {
        line = textStream.readLine(); // 不包括“\n”的一行文本
        int i = 0;
        if(winId==CONDITION_PARM_TYPE && mainWinId==0) i=1;
        QVector<QString> TmpTab;
        while(1)
        {
            QString TempStr = line.section(',', i, i);
            i++;
            if(TempStr == "")
            {
                break;
            }
            TmpTab.push_back(TempStr);
        }
        if(TmpTab.size()>0){
            Table.push_back(TmpTab);
        }
    }
    qDebug()<<"dd"<<endl;

}

void MainWindow::SaveTabFile(const QString &file, QVector<QVector<QString> > &Table)
{

}

void MainWindow::ShowTabData(const QVector<QVector<QString>> &Data,QTableWidget *&Table,int flag)
{
    if(flag % 4 ==0){
        int DataRows=Data.size();//测试数据列数
        //表格规模
        Table->setRowCount(DataRows-1);
        Table->setColumnCount(Data[0].size()+1);
        qDebug() << "output : " << Data[0].size();

        //表头H
        QStringList headers;
        //int ParmIndex = (CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM + ENVIRONMENT_PARM_NUM;
        headers<<"组数";
        for(int i = 0; i < Data[0].size(); i++)
        {
            headers << Data[0][i];
        }
        Table->setHorizontalHeaderLabels(headers);
        Table->horizontalHeader()->setFont(serifFont);

        //表头V
        headers.clear();
        Table->verticalHeader()->hide(); //隐藏表头

        //第一行
        for(int i = 1; i < DataRows; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem (QString("第")+QString::number(i)+QString("组预测值"));
            item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            item->setFont(serifFont);

            if(i%2==0){
                item->setBackground(QBrush( QColor(255,227,132)));
            }
            else{
                item->setBackground(QBrush( QColor(189,252,201)));
            }
            Table->setItem(i-1, 0, item);
        }

        //内容
        for(int row=1;row<DataRows;row++){
            for(int i = 1; i < Data[row].size()+1; i++)
            {
                QTableWidgetItem *item = new QTableWidgetItem (Data[row][i-1]);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                item->setFont(serifFont);
                Table->setItem(row-1, i, item);
            }

        }

        //表格大小自适应于内容
        Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    else if(flag % 4 ==1){
        Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        //表格规模
        Table->setRowCount(Data.size() - 1); //除去表头
        Table->setColumnCount(Data[0].size());

        //表头H
        QStringList headers;

        for(int i = 0; i < Data[0].size(); i++)
        {
            headers << Data[0][i];
        }

        Table->setHorizontalHeaderLabels(headers);

        //QFont serifFont("Consolas", 10);
        Table->horizontalHeader()->setFont(serifFont);

        //表头V
        headers.clear();
        for(int i = 1; i < Data.size(); i++)
        {
            headers << QString("样本") + QString::number(i);
        }

        Table->setVerticalHeaderLabels(headers);
        Table->verticalHeader()->setFont(serifFont);

        //内容
        for(int i = 1; i < Data.size(); i++)
        {
            for(int j = 0; j < Data[i].size(); j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem (Data[i][j]);
                item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//设置水平和垂直居中
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                //QFont serifFont("Consolas", 10);
                item->setFont(serifFont);
                Table->setItem(i - 1, j, item);
            }
        }
        //表格大小自适应于内容
        Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
    return ;
}

void MainWindow::setWidgetShow(int flag)
{
    if(flag) mainWinId=0;
    else mainWinId=1;
    ui->tabWidget->removeTab(flag);//需要移除的窗口
    setBtnShow(1);

}

