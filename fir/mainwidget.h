#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

public slots:
    void showMessage(int type);

private slots:
    void on_btnStart_clicked();

    void on_btnReset_clicked();

    void on_radioBtnPVE_clicked();

    void on_radioBtnPVP_clicked();

    void on_btnRegret_clicked();

private:
    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
