#ifndef SWIFT_MAKE_BUCKET_H
#define SWIFT_MAKE_BUCKET_H

#include <QObject>
#include "global.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>

class Swift_Make_Bucket : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Make_Bucket(QObject *parent = 0);

    explicit Swift_Make_Bucket(QMap<QString,QString> * token_message ,QObject *parent = 0);
    ~Swift_Make_Bucket();

    void DoSetup(QThread &cThread);

signals:

public slots:

private:
    QMap<QString,QString>* token_message;
    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_map {ret_map=NULL};
};

#endif // SWIFT_MAKE_BUCKET_H
