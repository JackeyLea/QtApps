#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QtXml>
#include <QMessageBox>

#include "itemdelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initModel();

public slots:
    void on_tableView_clickedBlankArea();

private slots:
    void on_btnAdd_clicked();

    void on_btnRemove_clicked();

    void on_btnSave_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_btnOpen_clicked();

private:
    Ui::MainWindow *ui;

    QStandardItemModel *model;

    qint16 curRow;

    QStringList supportType;
};

#endif // MAINWINDOW_H
