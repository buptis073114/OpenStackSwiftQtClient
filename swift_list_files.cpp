﻿#include "swift_list_files.h"

Swift_List_Files::Swift_List_Files(QObject *parent) : QObject(parent)
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


Swift_List_Files::Swift_List_Files(QMap<QString,QString> * token_message ,
                                   QString bucketname,QObject *parent) : QObject(parent)
{
    this->token_message = token_message;
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


void Swift_List_Files::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_list_files()));
}

void Swift_List_Files::slot_swift_list_files(){
    slot_swift_list_files(this->token_message,this->bucketname);
}

void Swift_List_Files::slot_swift_list_files(QMap<QString,QString> * token_message ,
                                            QString bucketname){
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
        QString token  ;
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
        post_reply = manager->get(network_request);

        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));
    }
}


void Swift_List_Files::replyFinished(QNetworkReply* reply){
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
        ret_header_map->insert(first_bytes,second_bytes);
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


    QStringList filenamelist = ret_data.split("\n");

    int i=0;

    for(i=0;i<filenamelist.length();i++){
        QString filename = filenamelist.at(i);
        if(NULL!=filenamelist.at(i)&&QString::compare(filename,"")!=0){
            qDebug() << Q_FUNC_INFO << "filename is " << filename;
        }
    }


    emit signal_swift_list_files_success(ret_header_map ,ret_data);
}

void Swift_List_Files::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_List_Files::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){

}

void Swift_List_Files::NetWorkError(QNetworkReply::NetworkError){


    emit signal_swift_list_files_error("network error");

    if (NULL != post_reply) {
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }
}


Swift_List_Files::~Swift_List_Files(){
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
