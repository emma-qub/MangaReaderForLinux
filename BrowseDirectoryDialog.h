#ifndef BROWSEDIRECTORYDIALOG_H
#define BROWSEDIRECTORYDIALOG_H

#include <QtWidgets>

class BrowseDirectoryDialog: public QDialog {
  Q_OBJECT

public:
  BrowseDirectoryDialog(QWidget* parent = NULL, Qt::WindowFlags flags = 0);
  virtual ~BrowseDirectoryDialog(void) final;

  inline QString getScansDirectory(void) const { return _scansDirectoryLineEdit->text(); }

public slots:
  void checkDirectoryValid(const QString&);
  void browse(void);

private:
  QLineEdit* _scansDirectoryLineEdit;
  QPushButton* _browseDirectoryButton;
  QPushButton* _okButton;
};

#endif // BROWSEDIRECTORYDIALOG_H
