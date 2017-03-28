#ifndef SWIFT_MAKE_FOLDER_H
#define SWIFT_MAKE_FOLDER_H

#include <QObject>
#include "global.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>

class Swift_Make_Folder : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Make_Folder(QObject *parent = 0);

    explicit Swift_Make_Folder(QMap<QString,QString> * token_message ,
                               QString bucketname,
                               QString foldername,
                               QObject *parent = 0);
    ~Swift_Make_Folder();

    void DoSetup(QThread &cThread);

signals:

    void signal_swift_make_folder_error(QString);

    void signal_swift_make_folder_success(QMap<QString,QString> *);

public slots:

    void slot_swift_make_folder();

    void slot_swift_make_folder(QMap<QString,QString> * token_message ,
                                QString bucketname,
                                QString foldername);


    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:
    QMap<QString,QString>* token_message;
    QString bucketname;
    QString foldername;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_map {ret_map=NULL};
    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};
};

#endif // SWIFT_MAKE_FOLDER_H
