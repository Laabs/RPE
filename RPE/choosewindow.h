#ifndef CHOOSEWINDOW_H
#define CHOOSEWINDOW_H
//#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include"mainwindow.h"

namespace Ui {
class ChooseWindow;
}

class ChooseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseWindow(QWidget *parent = nullptr);
    ~ChooseWindow();

private:
    Ui::ChooseWindow *ui;

//以下为自定义函数或成员
private:
    //槽函数与窗口属性集合函数
    void ConnectSlots();
    void WindowFormat();

private slots:
    void ModelSolt();     //模型训练窗口函数槽
    void PredictSolt();   //预测窗口函数槽
    void ShowChooseWin(); //本窗口类显示函数（用于回显此窗口）

public:
    MainWindow* mainWin;   //模型训练窗口与预测窗口对象
    bool state=false;      //记录是否先训练模型
};

#endif // CHOOSEWINDOW_H
