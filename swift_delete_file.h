#ifndef SWIFT_DELETE_FILE_H
#define SWIFT_DELETE_FILE_H

#include <QObject>
#include "global.h"


class Swift_Delete_File : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Delete_File(QObject *parent = 0);
    explicit Swift_Delete_File(QMap<QString,QString>* token_message,
                               QString bucketname,
                               QString filepath,QObject *parent = 0);
    ~Swift_Delete_File();
    void DoSetup(QThread &cThread);


signals:
    void signal_swift_delete_file_success(QMap<QString,QString>*);
    void signal_swift_delete_file_error(QString);


public slots:
    void slot_swift_delete_file();
    void slot_swift_delete_file(QMap<QString,QString>* token_message,
                                         QString bucketname,
                                         QString filepath);

    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);


private:
    QMap<QString,QString>* token_message;

    QString bucketname;
    QString filepath;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_map {ret_map=NULL};

    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};

};

#endif // SWIFT_DELETE_FILE_H
