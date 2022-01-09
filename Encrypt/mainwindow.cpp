#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QString encrypt_str = rsa_pub_encrypt_base64("123abc");
//    qDebug()<<"加密数据："<<encrypt_str;
//    QString decrypt_str = rsa_pri_decrypt_base64(encrypt_str);
//    qDebug()<<"解密数据："<<decrypt_str;

    QString address;
    auto interfaces = QNetworkInterface::allInterfaces();
    for(int i=0;i<interfaces.length();i++){
        if(interfaces[i].isValid()){
#if defined(Q_OS_WIN32)
            if(interfaces[i].name().contains("ethernet")){
#elif defined(Q_OS_LINUX)
            if(interfaces[i].name().contains("enp")){
#endif
                address = interfaces[i].hardwareAddress();
                break;
            }
        }
    }
    if(!address.isEmpty()){
        ui->lineMAC->setText(address);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::rsa_pub_encrypt_base64(const QString &strClearData)
{
    //公钥  长度为512  （使用自己生成的公秘钥）
    const char* public_key="-----BEGIN PUBLIC KEY-----\n"
            "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArQPqyUf3SQklSVaQ4bXk\n"
            "Migns6djeKnQvQdi5DMLkjSzU0uMGEpVs+RMGv/TjSiKM7RuN2FedrP5OFYIfiSq\n"
            "JuxQJzHdy3eV2tYBIoQ22udZEA5kxzXjqARmxZwxcUIZRErfHFIeb+zM6/h9UZ3K\n"
            "9eYnu0e1DTOZqpkJTJVM/qz3/DfhOfgZ07YRBkZykQMu3hdG4+ysPmIde9sM7c+d\n"
            "7n2wS8bVLou/qvLa11o4fvcrGtzcTRvjiMaqQjOF7MyrYuVvbKTgh942SI6IIIoa\n"
            "NZzJjObF0x1d1DaRtcytvYCU3azYfs05UWVz4GG+zEpXUCUW5x5WXFfN5O6F4pqX\n"
            "KQIDAQAB\n"
            "-----END PUBLIC KEY-----";

    //将字符串键加载到bio对象
    BIO* pKeyBio = BIO_new_mem_buf(public_key, strlen(public_key));
    if (pKeyBio == NULL){
        qDebug()<<"Public encryption BIO is empty.";
        return "";
    }
    RSA* pRsa = RSA_new();
    pRsa = PEM_read_bio_RSA_PUBKEY(pKeyBio, &pRsa, NULL, NULL);
    if ( pRsa == NULL ){
        qDebug()<<"Public encryption read bio rsa error.";
        BIO_free_all(pKeyBio);
        return "";
    }
    int nLen = RSA_size(pRsa);
    char* pEncryptBuf = new char[nLen];
    memset(pEncryptBuf, 0, nLen);
    QByteArray clearDataArry = strClearData.toUtf8();
    int nClearDataLen = clearDataArry.length();
    uchar* pClearData = (uchar*)clearDataArry.data();
    int nSize = RSA_public_encrypt(nClearDataLen,
                                    pClearData,
                                    (uchar*)pEncryptBuf,
                                    pRsa,
                                    RSA_PKCS1_PADDING);

    QString strEncryptData = "";
    if ( nSize >= 0 ){
        QByteArray arry(pEncryptBuf, nSize);
        strEncryptData = arry.toBase64();
    }
    // 释放内存
    delete[] pEncryptBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strEncryptData;
}

QString MainWindow::rsa_pri_decrypt_base64(const QString &strDecryptData)
{
    char private_key[]="-----BEGIN PRIVATE KEY-----\n"
            "MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQCtA+rJR/dJCSVJ\n"
            "VpDhteQyKCezp2N4qdC9B2LkMwuSNLNTS4wYSlWz5Ewa/9ONKIoztG43YV52s/k4\n"
            "Vgh+JKom7FAnMd3Ld5Xa1gEihDba51kQDmTHNeOoBGbFnDFxQhlESt8cUh5v7Mzr\n"
            "+H1Rncr15ie7R7UNM5mqmQlMlUz+rPf8N+E5+BnTthEGRnKRAy7eF0bj7Kw+Yh17\n"
            "2wztz53ufbBLxtUui7+q8trXWjh+9ysa3NxNG+OIxqpCM4XszKti5W9spOCH3jZI\n"
            "joggiho1nMmM5sXTHV3UNpG1zK29gJTdrNh+zTlRZXPgYb7MSldQJRbnHlZcV83k\n"
            "7oXimpcpAgMBAAECggEAc839ZwBuekpO1MnGa+1tck0CVfOiVu+DMiMxhADOq+vu\n"
            "Y8BbYx7OQnx6nE/D7+XEq1sHsrXPnrBUDes4AAbWhPsDo1hzVm8StMktNUc92EOl\n"
            "j2Oy/MwLNzjK+VXUmjRd6SIcOD6W9N+RQhiu+OQfJ3wH+AjFHLL/FfsdCE+em49c\n"
            "BWqsHyNTAUbMzKPXRZTl34fjjAFgfga4XxRiaY0Jm1GbS//hPaHfH5FPPkirmwky\n"
            "Mxlrxv8LZqcfWGKU5H3D5HNZ7q37QBjh/c7ArG2R66bc0C2kbSmfTxmkmUr0QXj1\n"
            "pVTsIYvgS52q3BollJ/36M38HNwlEAeRVJOzFYRzaQKBgQDmQhrfEA8RDTS+ITHQ\n"
            "9183tF9cgINgTYM5DXW18DqAx+2Eq+eKMpuCgd2FPcJ0RxWNhw9xmB5L+HgIkpuL\n"
            "umsIO1I3ldMAEHUyBJW0gZVmxa60wqoIG7TiCog6ckzsl9A1pppd5O4Oaelj5H9O\n"
            "Fb5JB0iVhc7ct+kLf53WUs6YKwKBgQDAW4sfutH+O299Go67SxxjRARBWDxkU7aH\n"
            "TV8iIPyRIeAqhJmLxxrZQJCPyWzBkgrJbU+HuI6x5bUcaEFgP+27pxVbkRGqCjUB\n"
            "kLXfrssQsEvH1L1ExHUBN4USHSLWSFX0bVffHeeDkSjNzjfYctFQVNlZP0YOb09j\n"
            "lYCizxKv+wKBgQCxxXvFmDppqc58tFgnw7Tp5wJxAm9r3KxhUfj9XNluL+07LxV2\n"
            "ZxgVLHqAsmv9JhyHxiywQQezE8RoqfgZavXTxyj07WGYfRxeX5fNYvacACegO2LX\n"
            "G70Nf6Apr7GgQJ1prpUjxOneivq8Ot/mw3bEHzF073gEhKESVZbgr9H9PQKBgQCb\n"
            "VZF8lwXa8q4ubZLTZm1PL+7rwdBLCFr+2hk320F6BcwbKadA/BEPXZcftf3wNvok\n"
            "/PPbWstIl4HwpbQdgKJ/Bgc2QWjtUn7Je82TC3kKSgN4JLCO5sLBr87y+LpYJmPb\n"
            "Fa1/DIwXb9Jul9f7H4UuBMbbzj/JilOmzGTrlKK3nwKBgQDEv9ZGZNR1yhzVn9x/\n"
            "WbIUJgJpOWUhoC0KhQ2PlbXWLvR49QLOvwyGE2jFA/dM0mzdbgBDNtGyLRibgmZP\n"
            "Q2n6j61CEyZDOpP6JuwX2aLOW+GCheenOKCF93GFtlBwqohLJVv437W/ANsz0qHt\n"
            "HizI9uDRznjxINS4/BbTr+7D8g==\n"
            "-----END PRIVATE KEY-----";
    //将字符串键加载到bio对象
    BIO* pKeyBio = BIO_new_mem_buf(private_key, strlen(private_key));
    if (pKeyBio == NULL){
        qDebug()<<"Private decryption BIO is empty.";
        return "";
    }

    RSA* pRsa = RSA_new();
    pRsa = PEM_read_bio_RSAPrivateKey(pKeyBio, &pRsa, NULL, NULL);
    if ( pRsa == NULL ){
        qDebug()<<"Private decryption read bio rsa error.";
        BIO_free_all(pKeyBio);
        return "";
    }

    int nLen = RSA_size(pRsa);
    char* pClearBuf = new char[nLen];
    memset(pClearBuf, 0, nLen);
    //解密
    QByteArray decryptDataArry = strDecryptData.toUtf8();
    decryptDataArry = QByteArray::fromBase64(decryptDataArry);
    int nDecryptDataLen = decryptDataArry.length();
    uchar* pDecryptData = (uchar*)decryptDataArry.data();
    int nSize = RSA_private_decrypt(nDecryptDataLen,
                                   pDecryptData,
                                   (uchar*)pClearBuf,
                                   pRsa,
                                   RSA_PKCS1_PADDING);
    QString strClearData = "";
    if ( nSize >= 0 ){
        strClearData = QByteArray(pClearBuf, nSize);
    }

    // 释放内存
    delete[] pClearBuf;
    BIO_free_all(pKeyBio);
    RSA_free(pRsa);
    return strClearData;
}

void MainWindow::on_btnGenrate_clicked()
{
    QString a;
    a.append("Version");
    a.append(ui->comboVersion->currentText());
    a.append(";");
    if(!ui->lineCode->text().isEmpty()){
        a.append("Code");
        a.append(ui->lineCode->text());
        a.append(";");
    }
    if(!ui->lineMAC->text().isEmpty()){
        a.append("MAC");
        a.append(ui->lineMAC->text());
        a.append(";");
    }
    if(!ui->lineDate->text().isEmpty()){
        a.append("Expire");
        a.append(ui->lineDate->text());
        a.append(";");
    }
    QString encrypt_str = rsa_pub_encrypt_base64(a);
    qDebug()<<"加密数据："<<encrypt_str<<encrypt_str.size();
    ui->textLicenseCode->setPlainText(encrypt_str);
}

void MainWindow::on_btnParser_clicked()
{
    if(ui->textLicenseCode->toPlainText().isEmpty()){
        QMessageBox::information(this,"Warning","请先生成注册码");
        return;
    }
    QString decrypt_str = rsa_pri_decrypt_base64(ui->textLicenseCode->toPlainText());
    qDebug()<<"解密数据："<<decrypt_str;
    ui->lineParsed->setText(decrypt_str);
}

void MainWindow::on_btnRandomCode_clicked()
{
    char a[]={'0','1','2','3','4','5',
              '6','7','8','9','A','B',
              'C','D','E','F','G','H',
              'I','J','K','L','M','N',
              'O','P','Q','R','S','T',
              'U','V','W','X','Y','Z'};
    QString code;
    for(int i=0;i<8;i++){
        code.append(a[QRandomGenerator::global()->bounded(0,36)]);
    }
    ui->lineCode->setText(code);
}

