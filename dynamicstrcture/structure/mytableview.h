#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>

class MyTableView : public QTableView
{
    Q_OBJECT

public:
    MyTableView(QWidget *parent=0);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clickBlankArea();
};

#endif // MYTABLEVIEW_H
