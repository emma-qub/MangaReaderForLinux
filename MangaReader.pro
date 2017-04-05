SOURCES += \
    main.cpp \
    MainWindow.cpp \
    MangaListWidget.cpp \
    MangaReadWidget.cpp \
    Utils.cpp \
    MangaDownloadWidget.cpp \
    DownloadManager.cpp \
    MessageListView.cpp \
    MessageListModel.cpp \
    AddMangaDialog.cpp \
    BrowseDirectoryDialog.cpp \
    NotificationDialog.cpp \
    MangaListDelegate.cxx \
    ChapterListWidget.cxx

HEADERS += \
    MainWindow.h \
    MangaListWidget.h \
    MangaReadWidget.h \
    Utils.h \
    MangaDownloadWidget.h \
    DownloadManager.h \
    MessageListView.h \
    MessageListModel.h \
    AddMangaDialog.h \
    BrowseDirectoryDialog.h \
    NotificationDialog.h \
    MangaListDelegate.hxx \
    ChapterListWidget.hxx

QT += \
    widgets \
    network

CONFIG += \
    c++11
