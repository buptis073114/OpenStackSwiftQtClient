#ifndef SWIFT_DOWNLOAD_FILE_H
#define SWIFT_DOWNLOAD_FILE_H

#include <QObject>
#include "global.h"


class Swift_Download_File : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Download_File(QObject *parent = 0);
    explicit Swift_Download_File(QMap<QString,QString>* token_message,
                                 QString bucketname,
                                 QString filepath,
                                 QString filename,
                                 QString savepath,QObject *parent = 0);

    void DoSetup(QThread &cThread);
    ~Swift_Download_File();
    void gotoclear();

signals:
    void signal_swift_download_file_error(QString);
    void signal_swift_download_file_success(QMap<QString,QString>*);

public slots:
    void slot_swift_download_file();
    void slot_swift_download_file(QMap<QString,QString>* token_message,
                                  QString bucketname,
                                  QString filepath,
                                  QString filename,
                                  QString savepath);


    void httpReadyRead();
    void downloadProgress(qint64,qint64);

    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);



private:
    QMap<QString,QString>* token_message;

    QString bucketname;
    QString filepath;
    QString savepath;

    QFile *file {file=NULL};
    QString filename;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_map {ret_map=NULL};

    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};

};

#endif // SWIFT_DOWNLOAD_FILE_H
