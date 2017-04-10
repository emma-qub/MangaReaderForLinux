SOURCES += \
    main.cpp \
    MainWindow.cpp \
    MangaListWidget.cpp \
    MangaReadWidget.cpp \
    Utils.cpp \
    MangaDownloadWidget.cpp \
    MessageListView.cpp \
    MessageListModel.cpp \
    AddMangaDialog.cpp \
    BrowseDirectoryDialog.cpp \
    MangaListDelegate.cxx \
    ChapterListWidget.cxx \
    SlidingStackedWidget.cxx

HEADERS += \
    MainWindow.h \
    MangaListWidget.h \
    MangaReadWidget.h \
    Utils.h \
    MangaDownloadWidget.h \
    MessageListView.h \
    MessageListModel.h \
    AddMangaDialog.h \
    BrowseDirectoryDialog.h \
    MangaListDelegate.hxx \
    ChapterListWidget.hxx \
    SlidingStackedWidget.hxx

QT +=  widgets

CONFIG += c++11
