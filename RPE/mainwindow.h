#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma execution_character_set("utf-8")

#include <QMainWindow>
#include"subdialog.h"
#include "plotwindow1.h"

#include <QString>
#include <QVector>
#include <QFont>
#include <QStringList>
#include <QModelIndex>
#include<QSystemTrayIcon>
#include <QTableWidget>
#include <QCloseEvent>

//宏定义每项指标的数量（列数）
#define CONTROLLER_PARM_NUM 0
#define SERVO_PARM_NUM 12 //12,6
#define REDUCER_PARM_NUM 9 //9,6
#define CONDITION_PARM_NUM 3
#define WHOLE_ROBOT_CONDITION_PARM_NUM 6
#define ENVIRONMENT_PARM_NUM 0
#define WHOLE_ROBOT_PARM_NUM 17 //17,4
#define AXIS_NUM 6
#define PARM_TOTAL_NUM ((CONTROLLER_PARM_NUM + SERVO_PARM_NUM + REDUCER_PARM_NUM) * AXIS_NUM \
    + (ENVIRONMENT_PARM_NUM) + (WHOLE_ROBOT_PARM_NUM))

//枚举各种类型的代表数字
enum PARM_TYPE
{
    INVALID_PARM_TYPE = 0,
    CONTROLLER_PARM_TYPE = 1,
    SERVO_PARM_TYPE = 2,
    REDUCER_PARM_TYPE = 3,
    CONDITION_PARM_TYPE=4,
    ENVIRONMENT_PARM_TYPE = 5,
    WHOLE_ROBOT_PARM_TYPE = 6
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void WindowFormat();            //界面格式窗口
    void ConnectSlots();            //链接各类槽函数接口函数
    void LoadData();                //记录历史数据
public slots:
    void ShowUiSolt1();//界面1（核心零部件）显示
    void ShowUiSolt2();//界面2（工况条件）显示
    void ShowUiSolt3();//界面3（环境因素）显示

    void ErrSetSlot();  //误差设置槽函数
private:
    void setBtnShow(int flag);//1：核心零部件；2：环境；3：工况
signals:
    // 退出窗口事件
    void ExitWin();

public:
    void CheckSample(enum PARM_TYPE type);
    void ClearSingleSampleTbl();
    void CheckTestSample(enum PARM_TYPE type);
    void ClearTestDataTbl();
    void ClearTrainDataTbl();
    void AddRecord();
    void SaveRecord();
    void ReadRecord(bool flag = false);
    void ClearEstimateTbl();

    void LoadTrainData();
    void LoadTestData();
    void SaveTrainData();

    void closeEvent(QCloseEvent *); //重写关闭事件
    void showmianwindow();          //显示此窗口

private slots:

    //训练模型窗口
    void CurrentTrainFileSlot();
    void CheckControllerSlot();
    void CheckServoSlot();
    void CheckReducerSlot();
    void CheckEnvironmentSlot();
    void CheckWholeRobotSlot();
    void CheckConditionSlot();
    void CheckConditionSlot_2();

    //评估窗口
    void CurrentTestFileSlot();
    void CheckControllerSlot_2();
    void CheckServoSlot_2();
    void CheckReducerSlot_2();
    void CheckEnvironmentSlot_2();
    void CheckWholeRobotSlot_2();
    void SelectModelSlot();
    void DoEstimateSlot();// 模型评估
    void SaveToFileSlot();
    void CheckRecord();
    void TrainModelSolt();//模型训练
    void ExitSlot();
    void AnalyModelSolt();//评估训练集
    void AnalyModelSolt2();//评估测试集



private:
    void ShowTrainData();
    void PathSelectDir(QString& myDir,QString type);//工作目录选择

    void ReadTabFile(const QString& path,QVector<QVector<QString>> &Table);//从路径file读取文件到 Table
    void SaveTabFile(const QString& path,QVector<QVector<QString>> &Table);//将Table数据保存到文件 Table
    void ShowTabData(const QVector<QVector<QString>> &Data,QTableWidget *&Table,int flag);//在屏幕显示Table 的数据,flag决定首行冻结

private: // 窗口类定义区
    Ui::MainWindow* ui;
    SubDialog*      subWin;
    //ChooseDialog*   chooseWin;
    PlotWindow1*    plotWin;

private:

    QString CurrentTrainFile;
    QVector<QVector<QString> > TrainData;

    QString CurrentTestFile;
    QVector<QVector<QString> > TestData;
    QVector<QVector<QString>> TrainEstimateData;//训练集预测结果
    QVector<QVector<QString>> EstimateData;//测试集预测结果
    QVector<QVector<QString>> EstimateDataRate;//标红单元格
    double ShowThreshold=2;

    QString ExePath;

    enum PARM_TYPE CurrParmType;
    QStringList TrainDataHeaders;
    int CurrRcdIndex;
    QFont serifFont;

    //评估界面模型路径与训练文件路径
    QString exePath_pic;
    QString CurrentTrainFile_pic;

    // winId  1：核心零部件；2：环境；3：工况
    int winId=REDUCER_PARM_TYPE;
    int mainWinId=0;//0：第一个窗口，1：第二个窗口


public:
    void setWidgetShow(int flag);//0：界面1，1：界面2

};

#endif // MAINWINDOW_H
