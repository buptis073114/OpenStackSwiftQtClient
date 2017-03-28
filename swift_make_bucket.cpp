#include "swift_make_bucket.h"

Swift_Make_Bucket::Swift_Make_Bucket(QObject *parent) : QObject(parent)
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


Swift_Make_Bucket::Swift_Make_Bucket(QMap<QString,QString> * token_message ,QString bucketname,QObject *parent) : QObject(parent)
{
    this->token_message=token_message;
    this->bucketname=bucketname;
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

void Swift_Make_Bucket::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_make_bucket()));
}

void Swift_Make_Bucket::slot_swift_make_bucket(){

    slot_swift_make_bucket(this->token_message,this->bucketname);

}

void Swift_Make_Bucket::slot_swift_make_bucket(QMap<QString,QString> * token_message ,
                                               QString bucketname){

    this->token_message=token_message;
    this->bucketname=bucketname;
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

        network_request.setUrl(QUrl(url+"/"+bucketname));

        post_reply = manager->put(network_request,"");
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));
    }
}

void Swift_Make_Bucket::replyFinished(QNetworkReply* reply){
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

    emit signal_swift_make_bucket_success(ret_header_map);

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
}

void Swift_Make_Bucket::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Make_Bucket::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void Swift_Make_Bucket::NetWorkError(QNetworkReply::NetworkError){
    emit signal_swift_make_bucket_error("NetworkError");
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

}

Swift_Make_Bucket::~Swift_Make_Bucket(){
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
}


