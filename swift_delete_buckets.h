#ifndef SWIFT_DELETE_BUCKETS_H
#define SWIFT_DELETE_BUCKETS_H

#include <QObject>
#include "global.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>

class Swift_Delete_Buckets : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Delete_Buckets(QObject *parent = 0);

    explicit Swift_Delete_Buckets(QMap<QString,QString>* token_message,QString bucketname,QObject *parent = 0);
    ~Swift_Delete_Buckets();
    void DoSetup(QThread &cThread);

signals:
    void signal_swift_delete_bucket_success(QMap<QString,QString>*);
    void signal_swift_delete_bucket_error(QString);

public slots:
    void slot_swift_delete_bucket();
    void slot_swift_delete_bucket(QMap<QString,QString>* token_message,QString bucketname);


    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:

    QMap<QString,QString>* token_message;
    QString bucketname;
    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};

};

#endif // SWIFT_DELETE_BUCKETS_H
