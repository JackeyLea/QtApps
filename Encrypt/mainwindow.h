#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QNetworkInterface>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QRandomGenerator>
#include <QMessageBox>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //加密函数
    QString rsa_pub_encrypt_base64(const QString &strClearData);
    //解密函数
    QString rsa_pri_decrypt_base64(const QString &strDecryptData);

private slots:
    void on_btnGenrate_clicked();

    void on_btnParser_clicked();

    void on_btnRandomCode_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
