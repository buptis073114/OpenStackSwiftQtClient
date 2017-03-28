#include "swift_get_token.h"

Swift_Get_Token::Swift_Get_Token(QObject *parent) : QObject(parent)
{
    if (NULL == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));

    }
}

Swift_Get_Token::Swift_Get_Token(QString swift_url,QString swift_username,
                                 QString swift_password,QObject *parent) : QObject(parent)
{

    this->swift_url=swift_url;
    this->swift_username=swift_username;
    this->swift_password=swift_password;
    if (NULL == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
}

void Swift_Get_Token::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_token_get()));
}


void Swift_Get_Token::slot_swift_token_get(){
    slot_swift_token_get(this->swift_url,
                         this->swift_username,this->swift_password);

}

void Swift_Get_Token::slot_swift_token_get(QString swift_url,QString swift_username,
                                           QString swift_password){
    if (NULL == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    QNetworkRequest network_request;
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_2);
    network_request.setSslConfiguration(config);
    network_request.setRawHeader("X-Storage-User",swift_username.toUtf8());
    network_request.setRawHeader("X-Storage-Pass",swift_password.toUtf8());
    network_request.setUrl(QUrl(swift_url));
    post_reply = manager->get(network_request);

    //manager->put(network_request,"");
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(NetWorkError(QNetworkReply::NetworkError)));


}

void Swift_Get_Token::replyFinished(QNetworkReply* reply){
    qDebug() << Q_FUNC_INFO << " server reply";

    if(NULL!=ret_map){
        delete ret_map;
        ret_map = NULL;
    }

    if(NULL==ret_map){
       ret_map = new QMap<QString,QString>();
    }


    QList<QNetworkReply::RawHeaderPair> ret_message_header= reply->rawHeaderPairs();
    for(int i=0;i<ret_message_header.size();i++){
        QNetworkReply::RawHeaderPair pair = ret_message_header[i];
        QByteArray first_bytes = pair.first;
        QByteArray second_bytes = pair.second;
        ret_map->insert(first_bytes,second_bytes);
    }
    QString ret_data;
    if (NULL != post_reply) {
        ret_data = post_reply->readAll();
        qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;
        post_reply->deleteLater();
        post_reply = NULL;
    }
    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }
    emit signal_swift_token_get_success(ret_map);
}

void Swift_Get_Token::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Get_Token::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void Swift_Get_Token::NetWorkError(QNetworkReply::NetworkError){


    emit signal_swift_token_get_error("network error");

    if (NULL != post_reply) {
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }
}

Swift_Get_Token::~Swift_Get_Token(){
    if (NULL != post_reply) {
        qDebug() << Q_FUNC_INFO << "delete post_reply";
        post_reply->deleteLater();
        post_reply = NULL;
    }

//    if (NULL != manager) {
//        qDebug() << Q_FUNC_INFO << "delete manager";
//        delete manager;
//        manager = NULL;
//    }

    if(NULL!=ret_map){
        qDebug() << Q_FUNC_INFO << "delete ret_map";
        delete ret_map;
        ret_map = NULL;
    }
}
