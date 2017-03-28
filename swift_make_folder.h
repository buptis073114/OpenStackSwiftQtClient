#ifndef SWIFT_MAKE_FOLDER_H
#define SWIFT_MAKE_FOLDER_H

#include <QObject>
#include "global.h"
#include <QFileInfo>
#include <QFile>
#include <QDataStream>

class Swift_Make_Folder : public QObject
{
    Q_OBJECT
public:
    explicit Swift_Make_Folder(QObject *parent = 0);

    explicit Swift_Make_Folder(QMap<QString,QString> * token_message ,QObject *parent = 0);
    ~Swift_Make_Folder();

    void DoSetup(QThread &cThread);

signals:

public slots:

private:
    QMap<QString,QString>* token_message;
    QNetworkAccessManager *manager{ manager = NULL };
    QNetworkReply *post_reply{ post_reply = NULL };

    QMap<QString,QString> * ret_map {ret_map=NULL};
};

#endif // SWIFT_MAKE_FOLDER_H
