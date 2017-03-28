#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    if (NULL == swiftgettokenthread) {
        swiftgettokenthread = new QThread();
    }
    if(NULL==swift_get_token){
        swift_get_token = new Swift_Get_Token(swift_url,swift_username,swift_password,0);

        //绑定信号
        connect(this, SIGNAL(signal_swift_token_get()),
                swift_get_token, SLOT(slot_swift_token_get()));
        connect(this, SIGNAL(signal_swift_token_get(QString,
                                                    QString,
                                                    QString
                                                    )),
                swift_get_token, SLOT(slot_swift_token_get(QString,
                                                           QString,
                                                           QString)));
        connect(swift_get_token, SIGNAL(signal_swift_token_get_success(QMap<QString,QString> *)),
                this, SLOT(slot_swift_token_get_success(QMap<QString,QString> *)));
        connect(swift_get_token, SIGNAL(signal_swift_token_get_error(QString)),
                this, SLOT(slot_swift_token_get_error(QString)));
        swift_get_token->DoSetup(*swiftgettokenthread);
        swift_get_token->moveToThread(swiftgettokenthread);

    }
    if (!swiftgettokenthread->isRunning()) {
        swiftgettokenthread->start();
    }
    else {
        emit signal_swift_token_get(swift_url,swift_username,swift_password);
    }
}


void MainWidget::slot_swift_token_get_success(QMap<QString,QString> *ret_message){
    //    QMapIterator<QString, QString> iteraterret_message(*ret_message);
    //    while (iteraterret_message.hasNext()) {
    //        iteraterret_message.next();
    //        qDebug() << Q_FUNC_INFO << "key is " << iteraterret_message.key();
    //        qDebug() << Q_FUNC_INFO << "value is " << iteraterret_message.value();
    //    }

    ret_token_message = ret_message;

    list_buckets(ret_token_message);

}

void MainWidget::list_buckets(QMap<QString,QString> *ret_message){
    if(NULL==swiftlistbucketthread){
        swiftlistbucketthread = new QThread();
    }
    if(NULL==swift_list_buckets){
        swift_list_buckets = new Swift_List_Buckets(ret_message,0);
        //绑定信号
        connect(this, SIGNAL(signal_swift_list_buckets()),
                swift_list_buckets, SLOT(slot_swift_list_bucket()));
        connect(this, SIGNAL(signal_swift_list_buckets(QMap<QString,QString>*)),
                swift_list_buckets, SLOT(slot_swift_list_bucket(QMap<QString,QString>* )));

        connect(swift_list_buckets, SIGNAL(
                    signal_swift_list_bucket_success(QMap<QString,QString> *,QString)),
                this, SLOT(
                    slot_swift_list_bucket_success(QMap<QString,QString> *,QString)));
        connect(swift_list_buckets, SIGNAL(signal_swift_list_bucket_error(QString)),
                this, SLOT(slot_swift_list_bucket_error(QString)));
        swift_list_buckets->DoSetup(*swiftlistbucketthread);
        swift_list_buckets->moveToThread(swiftlistbucketthread);
    }
    if (!swiftlistbucketthread->isRunning()) {
        swiftlistbucketthread->start();
    }
    else {
        emit signal_swift_token_get(swift_url,swift_username,swift_password);
    }
}

void MainWidget::slot_swift_token_get_error(QString ret_message){

    qDebug() << Q_FUNC_INFO << "ret_message is " << ret_message;
}



void MainWidget::slot_swift_list_bucket_success(QMap<QString,QString> *header_pair,QString ret_message){

    //    QMapIterator<QString, QString> iteraterret_message(*header_pair);
    //    while (iteraterret_message.hasNext()) {
    //        iteraterret_message.next();
    //        qDebug() << Q_FUNC_INFO << "key is " << iteraterret_message.key();
    //        qDebug() << Q_FUNC_INFO << "value is " << iteraterret_message.value();
    //    }
    //    qDebug() << Q_FUNC_INFO << "ret_message is " << ret_message;

    QStringList bucketnamelist = ret_message.split("\n");

    int i=0;

    for(i=0;i<bucketnamelist.length();i++){
        QString bucketname = bucketnamelist.at(i);
        if(NULL!=bucketnamelist.at(i)&&QString::compare(bucketname,"")!=0){
            qDebug() << Q_FUNC_INFO << "bucketname is " << bucketname;

            //
        }

    }

    list_bucket_files(ret_token_message,"photos");
    //upload_file(ret_token_message,"photos","testfolder");
    //get_file_property(ret_token_message,"photos","testfolder/svn使用方法.docx");
    //delete_file(ret_token_message,"photos","testfolder/svn使用方法.docx");
    //testfolder/svn使用方法.docx
    download_file(ret_token_message,"photos","testfolder/svn使用方法.docx","我的测试文档.docx","C:/Users/ss/Desktop");

}

void MainWidget::slot_swift_list_bucket_error(QString ret_message){
    qDebug() << Q_FUNC_INFO << "ret_message is " << ret_message;
}


void MainWidget::list_bucket_files(QMap<QString,QString> * token_message , QString bucketname){

    if(NULL==swiftlistfilesthread){
        swiftlistfilesthread = new QThread();
    }
    if(NULL==swift_list_files){
        swift_list_files = new Swift_List_Files(token_message,bucketname,0);
        //绑定信号
        connect(this, SIGNAL(signal_swift_list_files()),
                swift_list_files, SLOT(slot_swift_list_files()));
        connect(this, SIGNAL(signal_swift_list_files(QMap<QString,QString> *,QString)),
                swift_list_files, SLOT(slot_swift_list_files(QMap<QString,QString> *,QString )));

        connect(swift_list_files, SIGNAL(
                    signal_swift_list_files_success(QMap<QString,QString> *,QString)),
                this, SLOT(
                    slot_swift_list_files_success(QMap<QString,QString> *,QString)));
        connect(swift_list_files, SIGNAL(signal_swift_list_files_error(QString)),
                this, SLOT(slot_swift_list_files_error(QString)));
        swift_list_files->DoSetup(*swiftlistfilesthread);
        swift_list_files->moveToThread(swiftlistfilesthread);
    }
    if (!swiftlistfilesthread->isRunning()) {
        swiftlistfilesthread->start();
    }
    else {
        emit signal_swift_list_files(token_message,bucketname);
    }
}




void MainWidget::slot_swift_list_files_success(QMap<QString,QString> *header_pair,
                                               QString ret_message){
    QMapIterator<QString, QString> iteraterret_message(*header_pair);
    while (iteraterret_message.hasNext()) {
        iteraterret_message.next();
        qDebug() << Q_FUNC_INFO << "key is " << iteraterret_message.key();
        qDebug() << Q_FUNC_INFO << "value is " << iteraterret_message.value();
    }
    QStringList filenamelist = ret_message.split("\n");

    int i=0;

    for(i=0;i<filenamelist.length();i++){
        QString filename = filenamelist.at(i);
        if(NULL!=filenamelist.at(i)&&QString::compare(filename,"")!=0){
            qDebug() << Q_FUNC_INFO << "filename is " << filename;
        }
    }

    //qDebug() << Q_FUNC_INFO << "ret_message is " << ret_message;
}

void MainWidget::slot_swift_list_files_error(QString ret_message){
    qDebug() << Q_FUNC_INFO << "ret_message is " << ret_message;
}


void MainWidget::upload_file(QMap<QString,QString> * header_pair,QString bucketname,QString ralative_path){

    if(NULL==swiftuploadfilesthread){
        swiftuploadfilesthread = new QThread();
    }

    QString uploadfilepath = "C:/Users/ss/Desktop/svn使用方法.docx";

    if(NULL==swift_upload_file){
        swift_upload_file = new Swift_Upload_File(header_pair,uploadfilepath,bucketname,ralative_path);

        connect(this, SIGNAL(signal_swift_upload_file()),
                swift_upload_file, SLOT(slot_swift_upload_file()));
        connect(this, SIGNAL(signal_swift_upload_file(QMap<QString,QString> *,QString,QString,QString)),
                swift_upload_file, SLOT(slot_swift_upload_file(QMap<QString,QString> *,QString,QString,QString )));
        connect(swift_upload_file, SIGNAL(
                    signal_swift_upload_file_success(QMap<QString,QString> *,QString)),
                this, SLOT(
                    slot_swift_upload_file_success(QMap<QString,QString> *,QString)));
        connect(swift_upload_file, SIGNAL(signal_swift_upload_file_error(QString)),
                this, SLOT(slot_swift_upload_file_error(QString)));

        swift_upload_file->DoSetup(*swiftuploadfilesthread);
        swift_upload_file->moveToThread(swiftuploadfilesthread);
    }

    if (!swiftuploadfilesthread->isRunning()) {
        swiftuploadfilesthread->start();
    }
    else {
        emit signal_swift_upload_file(header_pair,
                                      uploadfilepath,
                                      bucketname,
                                      ralative_path);
    }

}




void MainWidget::slot_swift_upload_file_success(QMap<QString,QString> * ret_pairs,QString ret_message){
    qDebug() << Q_FUNC_INFO << "in slot_swift_upload_file_success";
}

void MainWidget::slot_swift_upload_file_error(QString errormsg){
    qDebug() << Q_FUNC_INFO << "in slot_swift_upload_file_error";
}


void MainWidget::get_file_property(QMap<QString,QString> * header_pair,QString bucketname,QString filepath){

    if(NULL==swiftgetfilepropertythread){
        swiftgetfilepropertythread = new QThread();
    }

    if(NULL==swift_get_file_property){
        swift_get_file_property = new Swift_Get_Property(header_pair,bucketname,filepath);
        connect(this, SIGNAL(signal_swift_get_file_property()),
                swift_get_file_property, SLOT(slot_swift_get_file_property()));
        connect(this, SIGNAL(signal_swift_get_file_property(QMap<QString,QString> *,QString,QString)),
                swift_get_file_property, SLOT(slot_swift_get_file_property(QMap<QString,QString> *,QString,QString )));
        connect(swift_get_file_property, SIGNAL(
                    signal_swift_get_file_property_success(QMap<QString,QString> *)),
                this, SLOT(
                    slot_swift_get_file_property_success(QMap<QString,QString> *)));
        connect(swift_get_file_property, SIGNAL(signal_swift_get_file_property_error(QString)),
                this, SLOT(slot_swift_get_file_property_error(QString)));

        swift_get_file_property->DoSetup(*swiftgetfilepropertythread);
        swift_get_file_property->moveToThread(swiftgetfilepropertythread);

    }

    if (!swiftgetfilepropertythread->isRunning()) {
        swiftgetfilepropertythread->start();
    }
    else {
        emit signal_swift_get_file_property(header_pair,bucketname,filepath);
    }


}



void MainWidget::slot_swift_get_file_property_success(QMap<QString,QString> * ret_message){
    qDebug() << Q_FUNC_INFO << "in slot_swift_get_file_property_success";

    QMapIterator<QString, QString> iteraterret_message(*ret_message);
    while (iteraterret_message.hasNext()) {
        iteraterret_message.next();
        qDebug() << Q_FUNC_INFO << "key is " << iteraterret_message.key();
        qDebug() << Q_FUNC_INFO << "value is " << iteraterret_message.value();
    }

}

void MainWidget::slot_swift_get_file_property_error(QString error_message){
    qDebug() << Q_FUNC_INFO << "error_message is "<<error_message;
}


void MainWidget::delete_file(QMap<QString,QString> * header_pair,QString bucketname,QString filepath){

    if(NULL==swiftdeletefilesthread){
        swiftdeletefilesthread = new QThread();
    }

    if(NULL==swift_delete_file){
        swift_delete_file = new Swift_Delete_File(header_pair,bucketname,filepath);

        connect(this, SIGNAL(signal_swift_delete_file()),
                swift_delete_file, SLOT(slot_swift_delete_file()));
        connect(this, SIGNAL(signal_swift_delete_file(QMap<QString,QString> *,QString,QString)),
                swift_delete_file, SLOT(slot_swift_delete_file(QMap<QString,QString> *,QString,QString )));
        connect(swift_delete_file, SIGNAL(
                    signal_swift_delete_file_success(QMap<QString,QString> *)),
                this, SLOT(
                    slot_swift_delete_file_success(QMap<QString,QString> *)));
        connect(swift_delete_file, SIGNAL(signal_swift_delete_file_error(QString)),
                this, SLOT(slot_swift_delete_file_error(QString)));

        swift_delete_file->DoSetup(*swiftdeletefilesthread);
        swift_delete_file->moveToThread(swiftdeletefilesthread);


    }
    if (!swiftdeletefilesthread->isRunning()) {
        swiftdeletefilesthread->start();
    }
    else {
        emit signal_swift_delete_file(header_pair,bucketname,filepath);
    }


}
void MainWidget::slot_swift_delete_file_success(QMap<QString,QString> * ret_message){
    qDebug() << Q_FUNC_INFO << "in slot_swift_delete_file_success";
}

void MainWidget::slot_swift_delete_file_error(QString error_message){
    qDebug() << Q_FUNC_INFO << "error_message is "<<error_message;
}


void MainWidget::download_file(QMap<QString,QString> * header_pair,
                               QString bucketname,
                               QString filepath,
                               QString filaname,
                               QString savepath){


    if(NULL==swiftdownloadfilethread){
        swiftdownloadfilethread=new QThread();
    }

    if(NULL==swift_download_file){
        swift_download_file = new Swift_Download_File(header_pair,
                                                      bucketname,
                                                      filepath,
                                                      filaname,
                                                      savepath);


        connect(this, SIGNAL(signal_swift_download_file()),
                swift_download_file, SLOT(slot_swift_download_file()));
        connect(this, SIGNAL(signal_swift_download_file(QMap<QString,QString> *,
                                                        QString,QString,QString,QString)),
                swift_download_file, SLOT(slot_swift_download_file(QMap<QString,QString> *,QString,
                                                                   QString,QString,QString )));
        connect(swift_download_file, SIGNAL(
                    signal_swift_download_file_success(QMap<QString,QString> *)),
                this, SLOT(
                    slot_swift_download_file_success(QMap<QString,QString> *)));
        connect(swift_download_file, SIGNAL(signal_swift_download_file_error(QString)),
                this, SLOT(slot_swift_download_file_error(QString)));


        swift_download_file->DoSetup(*swiftdownloadfilethread);
        swift_download_file->moveToThread(swiftdownloadfilethread);

    }
    if (!swiftdownloadfilethread->isRunning()) {
        swiftdownloadfilethread->start();
    }
    else {
        emit signal_swift_download_file(header_pair,
                                        bucketname,
                                        filepath,
                                        filaname,
                                        savepath);
    }

}

void MainWidget::slot_swift_download_file_success(QMap<QString,QString> * ret_message){
    qDebug() << Q_FUNC_INFO << " in slot_swift_download_file_success ";
}

void MainWidget::slot_swift_download_file_error(QString error_message){
    qDebug() << Q_FUNC_INFO << "error_message is "<<error_message;
}

void MainWidget::delete_bucket(){

}

void MainWidget::create_bucket(){

}

MainWidget::~MainWidget()
{

    if(NULL!=swift_upload_file){
        qDebug() << Q_FUNC_INFO << "delete swift_upload_file";
        delete swift_upload_file;
        swift_upload_file = NULL;
    }

    if (NULL != swiftuploadfilesthread&&swiftuploadfilesthread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftuploadfilesthread";
        swiftuploadfilesthread->quit();
        swiftuploadfilesthread->wait();
    }


    if(NULL!=swift_list_files){
        qDebug() << Q_FUNC_INFO << "delete swift_list_files";
        delete swift_list_files;
        swift_list_files = NULL;
    }

    if (NULL != swiftlistfilesthread&&swiftlistfilesthread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftlistfilesthread";
        swiftlistfilesthread->quit();
        swiftlistfilesthread->wait();
    }

    if(NULL!=swift_list_buckets){
        qDebug() << Q_FUNC_INFO << "delete swift_list_buckets";
        delete swift_list_buckets;
        swift_list_buckets = NULL;
    }

    if (NULL != swiftlistbucketthread&&swiftlistbucketthread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftlistbucketthread";
        swiftlistbucketthread->quit();
        swiftlistbucketthread->wait();
    }

    if(NULL!=swift_get_token){
        qDebug() << Q_FUNC_INFO << "delete swift_get_token";
        delete swift_get_token;
        swift_get_token = NULL;
    }

    if (NULL != swiftgettokenthread&&swiftgettokenthread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftgettokenthread";
        swiftgettokenthread->quit();
        swiftgettokenthread->wait();
    }



    if(NULL!=swift_delete_file){
        qDebug() << Q_FUNC_INFO << "delete swift_delete_file";
        delete swift_delete_file;
        swift_delete_file = NULL;
    }

    if (NULL != swiftdeletefilesthread&&swiftdeletefilesthread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftdeletefilesthread";
        swiftdeletefilesthread->quit();
        swiftdeletefilesthread->wait();
    }



    if(NULL!=swift_get_file_property){
        qDebug() << Q_FUNC_INFO << "delete swift_get_file_property";
        delete swift_get_file_property;
        swift_get_file_property = NULL;
    }

    if (NULL != swiftgetfilepropertythread&&swiftgetfilepropertythread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftgetfilepropertythread";
        swiftgetfilepropertythread->quit();
        swiftgetfilepropertythread->wait();
    }


    if(NULL!=swift_download_file){
        qDebug() << Q_FUNC_INFO << "delete swift_download_file";
        delete swift_download_file;
        swift_download_file = NULL;
    }

    if (NULL != swiftdownloadfilethread&&swiftdownloadfilethread->isRunning()) {
        qDebug() << Q_FUNC_INFO << "quit swiftdownloadfilethread";
        swiftdownloadfilethread->quit();
        swiftdownloadfilethread->wait();
    }


    delete ui;
}
