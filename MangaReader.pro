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
    BrowseDirectoryDialog.cpp

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
    BrowseDirectoryDialog.h

QT += \
    widgets \
    network \
    webkitwidgets

CONFIG += c++11
