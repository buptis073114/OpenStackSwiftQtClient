#include "swift_get_property.h"

Swift_Get_Property::Swift_Get_Property(QObject *parent) : QObject(parent)
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

Swift_Get_Property::Swift_Get_Property(QMap<QString,QString>* token_message,
                                       QString bucketname,
                                       QString filepath,
                                       QObject *parent) : QObject(parent)
{
    this->token_message=token_message;
    this->bucketname=bucketname;
    this->filepath=filepath;
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

void Swift_Get_Property::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_get_file_property()));
}

void Swift_Get_Property::slot_swift_get_file_property(){
    slot_swift_get_file_property(this->token_message,this->bucketname,this->filepath);
}

void Swift_Get_Property::slot_swift_get_file_property(QMap<QString,QString>* token_message,
                                                      QString bucketname,
                                                      QString filepath){
    this->token_message=token_message;
    this->bucketname=bucketname;
    this->filepath=filepath;
    if (NULL == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }
    if(NULL!=token_message){
        QString token;
        QString url;
        QMapIterator<QString, QString> iteraterret_message(*token_message);
        while (iteraterret_message.hasNext()) {
            iteraterret_message.next();

            if(QString::compare(iteraterret_message.key(),QString("X-Storage-Token"))==0){
                token=iteraterret_message.value();
            }else if(QString::compare(iteraterret_message.key(),QString("X-Storage-Url"))==0){
                url=iteraterret_message.value();
            }

        }
        qDebug() << Q_FUNC_INFO << "token is " << token;
        qDebug() << Q_FUNC_INFO << "url is " << url;

        QNetworkRequest network_request;
        QSslConfiguration config;
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1_2);
        network_request.setSslConfiguration(config);
        network_request.setRawHeader("X-Auth-Token",token.toUtf8());

        network_request.setUrl(QUrl(url+"/"+bucketname+"/"+filepath));
        post_reply = manager->head(network_request);
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));
    }

}

void Swift_Get_Property::replyFinished(QNetworkReply* reply){

    qDebug() << Q_FUNC_INFO << " server reply";

    if(NULL!=ret_header_map){
        delete ret_header_map;
        ret_header_map = NULL;
    }

    if(NULL==ret_header_map){
        ret_header_map = new QMap<QString,QString>();
    }

    QList<QNetworkReply::RawHeaderPair> ret_message_header= reply->rawHeaderPairs();
    for(int i=0;i<ret_message_header.size();i++){
        QNetworkReply::RawHeaderPair pair = ret_message_header[i];
        QByteArray first_bytes = pair.first;
        QByteArray second_bytes = pair.second;

        qDebug() << Q_FUNC_INFO << " first_bytes is "<<first_bytes;
        qDebug() << Q_FUNC_INFO << " second_bytes is "<<second_bytes;

        ret_header_map->insert(first_bytes,second_bytes);
    }

    //QString ret_data;
    if (NULL != post_reply) {
        //ret_data = post_reply->readAll();
        //qDebug() << Q_FUNC_INFO << "reply readAll is " << ret_data;
        post_reply->deleteLater();
        post_reply = NULL;
    }
    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }
    emit signal_swift_get_file_property_success(ret_header_map);

}

void Swift_Get_Property::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){

    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Get_Property::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void Swift_Get_Property::NetWorkError(QNetworkReply::NetworkError){

    emit signal_swift_get_file_property_error("network error");


    if (NULL != post_reply) {
        qDebug() << Q_FUNC_INFO << "delete post_reply";
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if(NULL!=ret_header_map){
        qDebug() << Q_FUNC_INFO << "delete ret_header_map";
        delete ret_header_map;
        ret_header_map = NULL;
    }
}

Swift_Get_Property::~Swift_Get_Property(){
    if (NULL != post_reply) {
        qDebug() << Q_FUNC_INFO << "delete post_reply";
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if(NULL!=ret_header_map){
        qDebug() << Q_FUNC_INFO << "delete ret_header_map";
        delete ret_header_map;
        ret_header_map = NULL;
    }
}


