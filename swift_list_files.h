#ifndef SWIFT_LIST_FILES_H
#define SWIFT_LIST_FILES_H

#include <QObject>
#include "global.h"

class Swift_List_Files : public QObject
{
    Q_OBJECT
public:
    explicit Swift_List_Files(QObject *parent = 0);
    explicit Swift_List_Files(QMap<QString,QString> * token_message ,
                              QString bucketname,QObject *parent = 0);
    ~Swift_List_Files();

    void DoSetup(QThread &cThread);

signals:
    void signal_swift_list_files_error(QString);
    void signal_swift_list_files_success(QMap<QString,QString> *,QString);

public slots:
    void slot_swift_list_files();
    void slot_swift_list_files(QMap<QString,QString> * token_message ,
                               QString bucketname);


    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:
    QMap<QString,QString> * token_message;

    QString bucketname;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };


    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};
};

#endif // SWIFT_LIST_FILES_H
