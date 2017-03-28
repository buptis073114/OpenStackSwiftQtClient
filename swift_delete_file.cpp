#include "swift_delete_file.h"

Swift_Delete_File::Swift_Delete_File(QObject *parent) : QObject(parent)
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
Swift_Delete_File::Swift_Delete_File(QMap<QString,QString>* token_message,
                                     QString bucketname,
                                     QString filepath,QObject *parent) : QObject(parent)
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

void Swift_Delete_File::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_delete_file()));
}

void Swift_Delete_File::slot_swift_delete_file(){
    slot_swift_delete_file(this->token_message,this->bucketname,this->filepath);
}

void Swift_Delete_File::slot_swift_delete_file(QMap<QString,QString>* token_message,
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

        post_reply = manager->deleteResource(network_request);
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));

    }


}

void Swift_Delete_File::replyFinished(QNetworkReply* reply){


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

    emit signal_swift_delete_file_success(ret_header_map);

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

void Swift_Delete_File::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){

    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Delete_File::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){



}

void Swift_Delete_File::NetWorkError(QNetworkReply::NetworkError){


    emit signal_swift_delete_file_error("NetworkError");
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

Swift_Delete_File::~Swift_Delete_File(){


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
