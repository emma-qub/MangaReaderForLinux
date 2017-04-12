SOURCES += \
    main.cxx \
    MainWindow.cxx \
    MangaListWidget.cxx \
    MangaReadWidget.cxx \
    Utils.cxx \
    MangaDownloadWidget.cxx \
    MessageListView.cxx \
    MessageListModel.cxx \
    AddMangaDialog.cxx \
    BrowseDirectoryDialog.cxx \
    MangaListDelegate.cxx \
    ChapterListWidget.cxx \
    SlidingStackedWidget.cxx \
    Downloader.cxx

HEADERS += \
    MainWindow.hxx \
    MangaListWidget.hxx \
    MangaReadWidget.hxx \
    Utils.hxx \
    MangaDownloadWidget.hxx \
    MessageListView.hxx \
    MessageListModel.hxx \
    AddMangaDialog.hxx \
    BrowseDirectoryDialog.hxx \
    MangaListDelegate.hxx \
    ChapterListWidget.hxx \
    SlidingStackedWidget.hxx \
    Downloader.hxx

QT +=  widgets

CONFIG += c++14
