#ifndef SWIFT_GET_PROPERTY_H
#define SWIFT_GET_PROPERTY_H

#include <QObject>
#include "global.h"

class Swift_Get_Property : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Get_Property(QObject *parent = 0);
    explicit Swift_Get_Property(QMap<QString,QString>* token_message,
                                QString bucketname,
                                QString filepath,QObject *parent = 0);
    ~Swift_Get_Property();
    void DoSetup(QThread &cThread);

signals:
    void signal_swift_get_file_property_success(QMap<QString,QString>*);
    void signal_swift_get_file_property_error(QString);

public slots:

    void slot_swift_get_file_property();
    void slot_swift_get_file_property(QMap<QString,QString>* token_message,
                                      QString bucketname,
                                      QString filepath);

    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:

    QMap<QString,QString>* token_message;
    //QString url;
    QString bucketname;
    QString filepath;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_header_map {ret_header_map=NULL};
};

#endif // SWIFT_GET_PROPERTY_H
