#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnNormalOpen_clicked();

private:
    Ui::Widget *ui;

    QStringList m_imgPath;
    uint32_t m_imgCnt=0;
    uint32_t m_curIndex=0;
};
#endif // WIDGET_H
