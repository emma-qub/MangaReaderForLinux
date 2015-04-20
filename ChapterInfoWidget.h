#ifndef CHAPTERINFOWIDGET_H
#define CHAPTERINFOWIDGET_H

#include <QtWidgets>

class ChapterInfoWidget: public QWidget {
  Q_OBJECT

public:
  explicit ChapterInfoWidget(QWidget* parent = nullptr);

  inline void setNumberFiles(const QString& numberFiles) { _numberFilesLabel->setText(numberFiles); }
  inline void setTotalSize(const QString& totalSize) { _totalSizeLabel->setText(totalSize); }
  inline void setLastModification(const QString& lastModification) { _lastModificationLabel->setText(lastModification); }
  inline void setLastRead(const QString& lastRead) { _lastReadLabel->setText(lastRead); }
  inline void setImgPreview(const QPixmap& imgPreview) { _imgPreviewLabel->setPixmap(imgPreview.scaled(_imgPreviewLabel->width(), _imgPreviewLabel->height(), Qt::KeepAspectRatio)); }

signals:

public slots:

private:
  QLabel* _numberFilesLabel;
  QLabel* _totalSizeLabel;
  QLabel* _lastModificationLabel;
  QLabel* _lastReadLabel;
  QLabel* _imgPreviewLabel;
};

#endif // CHAPTERINFOWIDGET_H
