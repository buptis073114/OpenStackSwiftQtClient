#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QList>
#include <QMap>
#include <QThread>
#include <QTimer>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "global.h"
#include "swift_get_token.h"
#include "swift_list_buckets.h"
#include "swift_list_files.h"
#include "swift_upload_file.h"
#include "swift_delete_file.h"
#include "swift_get_property.h"
#include "swift_download_file.h"
#include "swift_make_folder.h"
#include "swift_make_bucket.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();


    void list_buckets(QMap<QString,QString> *);
    void list_bucket_files(QMap<QString,QString> *, QString );
    void upload_file(QMap<QString,QString> * header_pair,QString bucketname,QString ralative_path);
    void get_file_property(QMap<QString,QString> * header_pair,QString bucketname,QString filepath);
    void delete_file(QMap<QString,QString> * header_pair,QString bucketname,QString filepath);

    void download_file(QMap<QString,QString> * header_pair,
                       QString bucketname,
                       QString filepath,
                       QString filaname,
                       QString savepath);

    void delete_bucket();
    void create_bucket();

signals:
    void signal_swift_token_get();
    void signal_swift_token_get(QString,QString,QString);
    void signal_swift_list_buckets();
    void signal_swift_list_buckets(QMap<QString,QString>*);

    void signal_swift_list_files();
    void signal_swift_list_files(QMap<QString,QString> *,QString);


    void signal_swift_upload_file();
    void signal_swift_upload_file(QMap<QString,QString> *,QString,QString,QString);


    void signal_swift_get_file_property();
    void signal_swift_get_file_property(QMap<QString,QString> *,QString,QString);



    void signal_swift_delete_file();
    void signal_swift_delete_file(QMap<QString,QString> *,QString,QString);


    void signal_swift_download_file();
    void signal_swift_download_file(QMap<QString,QString> *,QString,QString,QString,QString);

public slots:
    void slot_swift_token_get_success(QMap<QString,QString> *);
    void slot_swift_token_get_error(QString);

    void slot_swift_list_bucket_success(QMap<QString,QString> *,QString);
    void slot_swift_list_bucket_error(QString);



    void slot_swift_list_files_success(QMap<QString,QString> *,QString);
    void slot_swift_list_files_error(QString);



    void slot_swift_upload_file_success(QMap<QString,QString> *,QString);
    void slot_swift_upload_file_error(QString);

    void slot_swift_get_file_property_success(QMap<QString,QString> *);
    void slot_swift_get_file_property_error(QString);

    void slot_swift_delete_file_success(QMap<QString,QString> *);
    void slot_swift_delete_file_error(QString);


    void slot_swift_download_file_success(QMap<QString,QString> *);
    void slot_swift_download_file_error(QString);

private:
    Ui::MainWidget *ui;

    Swift_Get_Token *swift_get_token{swift_get_token = NULL};
    QThread *swiftgettokenthread{ swiftgettokenthread = NULL };

    QMap<QString,QString> * ret_token_message {ret_token_message = NULL};

    Swift_List_Buckets *swift_list_buckets {swift_list_buckets= NULL};
    QThread *swiftlistbucketthread{ swiftlistbucketthread = NULL };


    Swift_List_Files *swift_list_files {swift_list_files= NULL};
    QThread *swiftlistfilesthread{ swiftlistfilesthread = NULL };


    Swift_Upload_File *swift_upload_file {swift_upload_file= NULL};
    QThread *swiftuploadfilesthread{ swiftuploadfilesthread = NULL };



    Swift_Delete_File *swift_delete_file {swift_delete_file= NULL};
    QThread *swiftdeletefilesthread{ swiftdeletefilesthread = NULL };


    Swift_Get_Property *swift_get_file_property {swift_get_file_property= NULL};
    QThread *swiftgetfilepropertythread{swiftgetfilepropertythread=NULL};


//    Swift_Delete_File *swift_delete_file {swift_delete_file= NULL};
//    QThread *swiftdeletefilethread{swiftdeletefilethread=NULL};

    Swift_Download_File *swift_download_file {swift_download_file= NULL};
    QThread *swiftdownloadfilethread{swiftdownloadfilethread=NULL};

};

#endif // MAINWIDGET_H
