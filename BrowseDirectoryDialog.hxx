#ifndef BROWSEDIRECTORYDIALOG_H
#define BROWSEDIRECTORYDIALOG_H

#include <QDialog>

class QLineEdit;

class BrowseDirectoryDialog: public QDialog {
  Q_OBJECT

public:
  BrowseDirectoryDialog(QWidget* parent = nullptr, Qt::WindowFlags flags = 0);

  QString getScansDirectory() const;

public slots:
  void checkDirectoryValid(const QString&);
  void browse();

private:
  QLineEdit* m_scansDirectoryLineEdit;
  QPushButton* m_browseDirectoryButton;
  QPushButton* m_okButton;
};

#endif // BROWSEDIRECTORYDIALOG_H
