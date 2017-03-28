#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T20:09:25
#
#-------------------------------------------------

QT       += core sql network xml widgets gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenStackSwiftQtClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwidget.cpp \
    global.cpp \
    swift_get_token.cpp \
    swift_list_buckets.cpp \
    swift_list_files.cpp \
    swift_upload_file.cpp \
    swift_delete_file.cpp \
    swift_make_folder.cpp \
    swift_make_bucket.cpp \
    swift_delete_buckets.cpp \
    swift_get_property.cpp \
    swift_download_file.cpp

HEADERS  += mainwidget.h \
    global.h \
    swift_get_token.h \
    swift_list_buckets.h \
    swift_list_files.h \
    swift_upload_file.h \
    swift_delete_file.h \
    swift_make_folder.h \
    swift_make_bucket.h \
    swift_delete_buckets.h \
    swift_get_property.h \
    swift_download_file.h

FORMS    += mainwidget.ui
