#ifndef UTILS_H
#define UTILS_H

#include <QStringList>
#include <QDir>
#include <QObject>

class Utils {

public:
  Utils(void);

  static QStringList dirList(const QDir& dir, bool reverseOrder = false);
  static QStringList filesList(QDir& dir, const QStringList &nameFilters = QStringList());
  static void updateChapterRead(const QString &mangaName, const QString &chapterName, bool read);
  static bool isChapterRead(const QString& mangaName, const QString& chapterName);
  static QList<bool> areChaptersRead(const QString& mangaName);
  static bool isMangaRead(QList<bool> chaptersRead);
  static void initdb(const QString& mangaName);
  static void addChapter(const QString& mangaName, const QString& chapterName);
  static bool removeDirectory(const QString& dirName);
  static const QDir& getScansDirectory(void);
  static QString getIconsPath(void);
  static QString getSourceAbsolutePath(void);

private:
  static void initdb(void);

public:
  static QDir _scansDirectory;
};

#endif // UTILS_H
