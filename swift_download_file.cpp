#include "swift_download_file.h"

Swift_Download_File::Swift_Download_File(QObject *parent) : QObject(parent)
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

Swift_Download_File::Swift_Download_File(QMap<QString,QString>* token_message,
                                         QString bucketname,
                                         QString filepath,
                                         QString filename,
                                         QString savepath,
                                         QObject *parent) : QObject(parent)
{

    this->token_message=token_message;
    this->bucketname=bucketname;
    this->filepath=filepath;
    this->savepath=savepath;
    this->filename=filename;

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

void Swift_Download_File::DoSetup(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(slot_swift_download_file()));
}


void Swift_Download_File::slot_swift_download_file(){
    slot_swift_download_file(this->token_message,
                             this->bucketname,
                             this->filepath,
                             this->filename,
                             this->savepath);
}

void Swift_Download_File::slot_swift_download_file(QMap<QString,QString>* token_message,
                                                   QString bucketname,
                                                   QString filepath,
                                                   QString filename,
                                                   QString savepath){


    this->token_message=token_message;
    this->bucketname=bucketname;
    this->filepath=filepath;
    this->savepath=savepath;
    this->filename=filename;


    if(!QDir(savepath).exists())
    {
        QDir photoDir;
        photoDir.mkdir(savepath);
    }

    file = new QFile(savepath +"/"+ filename + "_download");
    if(!file->open(QIODevice::WriteOnly|QIODevice::Append))
    {
        //如果打开文件失败，则删除file，并使file指针为0，然后返回
        //qDebug() << "file open error";
        delete file;
        file = NULL;
        return;
    }

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

        post_reply = manager->get(network_request);

        //有可用数据时
        connect(post_reply,SIGNAL(readyRead()),this,SLOT(httpReadyRead()));

        //更新进度条
        connect(post_reply,SIGNAL(downloadProgress(qint64,qint64)),
                this,SLOT(downloadProgress(qint64,qint64)));

        connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(NetWorkError(QNetworkReply::NetworkError)));
    }

}


void Swift_Download_File::httpReadyRead(){
    if (file && post_reply->bytesAvailable()>50000)
    {
        //如果文件存在，则写入文件
        QByteArray bytes = post_reply->read(post_reply->bytesAvailable());

        //download_len = bytes.length() + download_len;

        file->write(bytes);
        file->flush();
    }
}

void Swift_Download_File::downloadProgress(qint64 bytesReceived,qint64 bytesTotal){

    int pro = (int)(bytesReceived*100/bytesTotal);

    qDebug()<<Q_FUNC_INFO<<"pro is "<<pro;
}


void Swift_Download_File::replyFinished(QNetworkReply* reply){


    gotoclear();
    //修改文件名字
    QString path1 = savepath + "/" + filename + "_download";
    QString path2 = savepath + "/" + filename;
    bool ok = QFile::rename(path1,path2);


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

    emit signal_swift_download_file_success(ret_header_map);

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

void Swift_Download_File::slot_sslErrors(QNetworkReply *reply, const QList<QSslError> &errors){

    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();
}

void Swift_Download_File::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){



}

void Swift_Download_File::NetWorkError(QNetworkReply::NetworkError){


    emit signal_swift_download_file_error("NetworkError");
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

void Swift_Download_File::gotoclear(){
    if (file && post_reply->bytesAvailable() > 0) {
        QByteArray bytes = post_reply->read(post_reply->bytesAvailable());
        file->write(bytes);
        file->flush();
    }
    if(NULL!=file){
        file->flush();
        file->close();
        file = 0;
        delete file;
    }
}

Swift_Download_File::~Swift_Download_File(){
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
