#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "base.hpp"

#define IndexUrl "https://github.com/JackeyLea/"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void drawText(QString text);
    QString toHex(QByteArray byte);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionQt_triggered();

    void on_actionIndex_triggered();

    void on_textText_textChanged();

    void on_comboBoxFont_currentIndexChanged(int index);

    void on_buttonChooseColor_clicked();

private:
    Ui::MainWindow *ui;

    QColor font_color;

    int font_size;
    int lcd_type;
};
#endif // MAINWINDOW_H
