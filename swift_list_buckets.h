#ifndef SWIFT_LIST_BUCKETS_H
#define SWIFT_LIST_BUCKETS_H

#include <QObject>
#include "global.h"

class Swift_List_Buckets : public QObject
{
    Q_OBJECT
public:
    explicit Swift_List_Buckets(QObject *parent = 0);

    explicit Swift_List_Buckets(QMap<QString,QString>* token_message,QObject *parent = 0);

    ~Swift_List_Buckets();


    void DoSetup(QThread &cThread);

signals:
    void signal_swift_list_bucket_success(QMap<QString,QString> *,QString);
    void signal_swift_list_bucket_error(QString);

public slots:


    void slot_swift_list_bucket();
    void slot_swift_list_bucket(QMap<QString,QString>* token_message);


    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:
    QMap<QString,QString>* token_message;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};
};

#endif // SWIFT_LIST_BUCKETS_H
