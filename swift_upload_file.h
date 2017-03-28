#ifndef SWIFT_UPLOAD_FILE_H
#define SWIFT_UPLOAD_FILE_H

#include <QObject>
#include "global.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>

class Swift_Upload_File : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Upload_File(QObject *parent = 0);
    explicit Swift_Upload_File(QMap<QString,QString> * header_pair , QString filepath ,
                               QString bucketname, QString ralative_path
                               , QObject *parent = 0);
    ~Swift_Upload_File();
    void DoSetup(QThread &cThread);

signals:
    void signal_swift_upload_file_success(QMap<QString,QString> *,QString);
    void signal_swift_upload_file_error(QString);

public slots:

    void slot_swift_upload_file();
    void slot_swift_upload_file(QMap<QString,QString> * header_pair,
                                QString filepath,
                                QString bucketname,
                                QString ralative_path);
    void progressChanged(qint64, qint64);

    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:
    QMap<QString,QString> * header_pair{header_pair = NULL} ;
    QString filepath;
    QString bucketname;
    QString ralative_path;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    qint64 file_len = 0; //上传文件大小
    char* m_buf{m_buf = NULL};

    qint64 block_length = 512*1024;

    QMap<QString,QString> * ret_header_map{ret_header_map = NULL };
};

#endif // SWIFT_UPLOAD_FILE_H
