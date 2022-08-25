#include <QCoreApplication>
#include <QThread>
#include <QString>
#include <QDebug>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QFile>
#include <QFileInfoList>
#include <QDir>

class DBHelper : public QThread{
public:
    DBHelper(QString dbName){
        _dbName=dbName;
        db = QSqlDatabase::addDatabase("QSQLITE",dbName);
        db.setDatabaseName("words.sqlite3");
        if(db.open()){
            qDebug()<<"Connect to db: "<<db.databaseName()<<" done.";
        }else{
            qDebug()<<"Cannot connect to db: "<<db.lastError();
        }
        query = new QSqlQuery(db);
    }
    ~DBHelper(){
        db.close();
    }

    void getAllName(QString filePath){
        QDir dir(filePath);
        if(!dir.exists()){
            qDebug()<<filePath<<"is not exists";
            return;
        }
        dir.setSorting(QDir::DirsLast);
        //get every file under dir
        foreach(QFileInfo finfo,dir.entryInfoList()){
            if(finfo.fileName()=="." || finfo.fileName()==".."){
                continue;
            }
            if(finfo.isFile()){
                QString sql = QString("select word from %1 where word = \"%2\"").arg(_dbName).arg(finfo.baseName());
                if(query->exec(sql)){
                    if(!query->next()){
                        //this word is not in db
                        QFile file(finfo.filePath());
                        if(!file.exists()){
                            qDebug()<<QString("%1 is no exist.").arg(finfo.filePath());
                            continue;
                        }
                        if(!file.open(QIODevice::ReadOnly)){
                            qDebug()<<"Cannot open file: "<<finfo.filePath();
                            continue;
                        }
                        sql.clear();
                        sql = QString("insert into %1 (word,size,data) values(:word,:size,:data);").arg(_dbName);
                        QByteArray ba = file.readAll();
                        query->clear();
                        query->prepare(sql);
                        query->bindValue(":word",finfo.baseName());
                        query->bindValue(":size",ba.size());
                        query->bindValue(":data",ba);
                        if(!query->exec()){
                            qDebug()<<"Cannot insert data into db: "<<query->lastQuery();
                        }
                        if(db.commit()){
                            qDebug()<<"insert into db successfully.";
                        }
                    }
                }else{
                    qDebug()<<query->lastError();
                }
            }else if(finfo.isDir()){
                getAllName(finfo.filePath());
            }
        }
        return;
    }

protected:
    void run(){
        if(!db.isOpen()) return;
        getAllName(QString("./14200")+QDir::separator()+_dbName);
        qDebug()<<QString("thread %1 finished.").arg(_dbName);
    }

private:
    QSqlDatabase db;
    QSqlQuery *query;
    QString _dbName;
};

int main()
{
    //26
    for(int i=0;i<26;i++){
        DBHelper dbh(QChar(i+'a'));
        dbh.start();
        while(dbh.isRunning()){
            ;
        }
    }
}
