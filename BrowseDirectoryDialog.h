#ifndef BROWSEDIRECTORYDIALOG_H
#define BROWSEDIRECTORYDIALOG_H

#include <QtWidgets>

class BrowseDirectoryDialog: public QDialog {
  Q_OBJECT

public:
  BrowseDirectoryDialog(QWidget* parent = nullptr, Qt::WindowFlags flags = 0);
  virtual ~BrowseDirectoryDialog() final;

  inline QString getScansDirectory() const { return m_scansDirectoryLineEdit->text(); }

public slots:
  void checkDirectoryValid(const QString&);
  void browse();

private:
  QLineEdit* m_scansDirectoryLineEdit;
  QPushButton* m_browseDirectoryButton;
  QPushButton* m_okButton;
};

#endif // BROWSEDIRECTORYDIALOG_H
