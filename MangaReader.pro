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
    AddMangaDialog.cpp \
    ChaptersOnWebView.cpp \
    ChaptersOnPCView.cpp \
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
    MessageItemDelegate.h \
    AddMangaDialog.h \
    ChaptersOnWebView.h \
    ChaptersOnPCView.h \
    BrowseDirectoryDialog.h \
    NotificationDialog.h \
    MangaListDelegate.hxx \
    ChapterListWidget.hxx

QT += \
    widgets \
    network \
    webview

CONFIG += \
    c++11
