#include "swift_upload_file.h"

Swift_Upload_File::Swift_Upload_File(QObject *parent) : QObject(parent)
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


Swift_Upload_File::Swift_Upload_File(QMap<QString,QString> * header_pair,
                                     QString filepath,
                                     QString bucketname,
                                     QString ralative_path,
                                     QObject *parent) : QObject(parent)
{

    this->header_pair=header_pair;
    this->filepath=filepath;
    this->bucketname=bucketname;
    this->ralative_path=ralative_path;

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

void Swift_Upload_File::DoSetup(QThread &cThread){

    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_upload_file()));

}

void Swift_Upload_File::slot_swift_upload_file(){

    slot_swift_upload_file(this->header_pair,this->filepath,this->bucketname,this->ralative_path);

}

void Swift_Upload_File::slot_swift_upload_file(QMap<QString,QString> * header_pair ,
                                               QString filepath,
                                               QString bucketname,
                                               QString ralative_path){
    if (NULL == manager) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
                this, SLOT(slot_sslErrors(QNetworkReply*, QList<QSslError>)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(provideAuthenication(QNetworkReply*, QAuthenticator*)));
    }


    if(NULL!=header_pair){
        QString token  ;
        QString url;
        QMapIterator<QString, QString> iteraterret_message(*header_pair);
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

        QFileInfo filepathinfo1(filepath);
        QString upload_filename = filepathinfo1.fileName();

        QNetworkRequest network_request;
        QSslConfiguration config;
        config.setPeerVerifyMode(QSslSocket::VerifyNone);
        config.setProtocol(QSsl::TlsV1_2);
        network_request.setSslConfiguration(config);
        network_request.setRawHeader("X-Auth-Token",token.toUtf8());
        network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        QFile file(filepath);
        file.open(QIODevice::ReadOnly);
        QDataStream filein(&file);

        file_len = file.size();

        //filein.skipRawData(len);


        qint64 readlen = file_len;
//        if(len + block_length >file_len){
//            readlen = file_len - len;
//        }else{
//            readlen = block_length;
//        }


        if(NULL==m_buf){
            m_buf = (char *)malloc(readlen);
            //m_buf = new char[readlen];
        }else{
            memset(m_buf,0,readlen);
        }
        if(m_buf){
            filein.readRawData(m_buf, readlen);
        }


        QByteArray arr(m_buf,(int)readlen);

        if(m_buf){
            free(m_buf);
            m_buf = NULL;
        }
        if(file.isOpen()){
            file.close();
        }

        network_request.setUrl(QUrl(url+"/"+bucketname+"/"+ralative_path+"/"+upload_filename));
        network_request.setHeader(QNetworkRequest::ContentLengthHeader, arr.length());

        post_reply = manager->put(network_request,arr);
        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));
        connect(post_reply, SIGNAL(uploadProgress(qint64, qint64)), this,
                SLOT(progressChanged(qint64, qint64)));
    }

}


void Swift_Upload_File::progressChanged(qint64 progress, qint64 total){

    qDebug() << Q_FUNC_INFO << "progress is"<<progress;
    qDebug() << Q_FUNC_INFO << "total is"<<total;

}

void Swift_Upload_File::replyFinished(QNetworkReply* reply){
    qDebug() << Q_FUNC_INFO << " server reply";

    if(NULL!=ret_header_map){
        delete ret_header_map;
        ret_header_map = NULL;
    }

    if(NULL==ret_header_map){
        ret_header_map = new QMap<QString,QString>();
    }

    QList<QNetworkReply::RawHeaderPair> ret_message_header= post_reply->rawHeaderPairs();
    for(int i=0;i<ret_message_header.size();i++){
        QNetworkReply::RawHeaderPair pair = ret_message_header[i];
        QByteArray first_bytes = pair.first;
        QByteArray second_bytes = pair.second;
        ret_header_map->insert(first_bytes,second_bytes);

        qDebug() << Q_FUNC_INFO << "reply first_bytes is " << first_bytes;
        qDebug() << Q_FUNC_INFO << "reply second_bytes is " << second_bytes;
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

    emit signal_swift_upload_file_success(ret_header_map ,ret_data);
}

void Swift_Upload_File::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Upload_File::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){


}

void Swift_Upload_File::NetWorkError(QNetworkReply::NetworkError){

    emit signal_swift_upload_file_error("network error");

    if (NULL != post_reply) {
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }
}

Swift_Upload_File::~Swift_Upload_File(){

    if (NULL != post_reply) {
        post_reply->deleteLater();
        post_reply = NULL;
    }

    if (NULL != manager) {
        delete manager;
        manager = NULL;
    }

}
