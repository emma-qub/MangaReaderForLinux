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
    MessageItemDelegate.cpp \
    ChapterInfoWidget.cpp \
    AddMangaDialog.cpp \
    ChaptersOnWebView.cpp \
    ChaptersOnPCView.cpp \
    BrowseDirectoryDialog.cpp \
    NotificationDialog.cpp

HEADERS += \
    MainWindow.h \
    MangaListWidget.h \
    MangaReadWidget.h \
    Utils.h \
    MangaDownloadWidget.h \
    DownloadManager.h \
    MessageListView.h \
    MessageListModel.h \
    MessageItemDelegate.h \
    ChapterInfoWidget.h \
    AddMangaDialog.h \
    ChaptersOnWebView.h \
    ChaptersOnPCView.h \
    BrowseDirectoryDialog.h \
    NotificationDialog.h

QT += \
    widgets \
    network \
    webview

CONFIG += c++11
