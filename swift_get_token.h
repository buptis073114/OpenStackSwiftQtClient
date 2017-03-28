#ifndef SWIFT_GET_TOKEN_H
#define SWIFT_GET_TOKEN_H

#include <QObject>
#include "global.h"

class Swift_Get_Token : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Get_Token(QObject *parent = 0);
    explicit Swift_Get_Token(QString swift_url,QString swift_username,
                             QString swift_password,QObject *parent = 0);
    ~Swift_Get_Token();
    void DoSetup(QThread &cThread);

signals:
    void signal_swift_token_get_success(QMap<QString,QString> *);
    void signal_swift_token_get_error(QString);

public slots:
    void slot_swift_token_get();
    void slot_swift_token_get(QString swift_url,QString swift_username,
                              QString swift_password);



    void replyFinished(QNetworkReply* reply);
    void slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void NetWorkError(QNetworkReply::NetworkError);

private:
    QString swift_url;
    QString swift_username;
    QString swift_password;

    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };


    QMap<QString,QString> * ret_map {ret_map=NULL};


};

#endif // SWIFT_GET_TOKEN_H
