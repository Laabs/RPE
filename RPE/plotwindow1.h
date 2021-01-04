#ifndef PLOTWINDOW1_H
#define PLOTWINDOW1_H

#include <QMainWindow>
#pragma execution_character_set("utf-8")

namespace Ui {
class PlotWindow1;
}

class PlotWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow1(QWidget *parent = nullptr);
    ~PlotWindow1();

private:
    Ui::PlotWindow1 *ui;



private slots:
    void ConnectSlots();
    void PicSelect(int type);
    void CompareSlot();
    void MseSlot();
    void RAdjustSlot();
    void WeightSlot();
    void ExitSlot();

public://公共接口函数
    void PathSet(const QString &ExePath,const int& winId);
    void WindowFormat(int winId);
    static bool isColseWin;//窗口关闭标记
    //重写关闭事件
   void closeEvent(QCloseEvent *);
signals:
    // 退出窗口事件
    void ExitWin1();

private:
    //analyze
    QString ExePath;
    int winId;

};

#endif // PLOTWINDOW1_H
