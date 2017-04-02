#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QDir>
#include <QObject>

class Utils {

public:
  Utils();

  static QStringList dirList(const QDir& dir, bool reverseOrder = false);
  static QStringList filesList(QDir& dir, const QStringList &nameFilters = QStringList());
  static void updateChapterRead(const QString &mangaName, const QString &chapterName, bool read);
  static bool isChapterRead(const QString& mangaName, const QString& chapterName);
  static QList<bool> areChaptersRead(const QString& mangaName);
  static bool isMangaRead(QList<bool> chaptersRead);
  static void initdb(const QString& mangaName);
  static void addChapter(const QString& mangaName, const QString& chapterName);
  static bool removeDirectory(const QString& dirName);
  static int countChaptersToRead(QList<bool> const& p_chaptersReadState);

  static const QDir& getScansDirectory();
  static QString getIconsPath();
  static QString getSourceAbsolutePath();
  static QString getScriptsAbsolutePath();

private:
  static void initdb();

public:
  static QDir m_scansDirectory;
};

#endif // UTILS_H
