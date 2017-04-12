#include "BrowseDirectoryDialog.hxx"

#include <QDir>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QFileDialog>

BrowseDirectoryDialog::BrowseDirectoryDialog(QWidget* parent, Qt::WindowFlags flags):
  QDialog(parent, flags) {

  m_scansDirectoryLineEdit = new QLineEdit(QDir::homePath());
  m_scansDirectoryLineEdit->setFixedWidth(300);
  connect(m_scansDirectoryLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkDirectoryValid(const QString&)));
  m_browseDirectoryButton = new QPushButton("Browse");
  m_browseDirectoryButton->setFixedWidth(100);
  connect(m_browseDirectoryButton, SIGNAL(clicked()), this, SLOT(browse()));

  QFormLayout* scansLayout = new QFormLayout;
  scansLayout->addRow(m_scansDirectoryLineEdit, m_browseDirectoryButton);

  m_okButton = new QPushButton("OK");
  m_okButton->setFixedWidth(100);
  connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(m_okButton);
  buttonLayout->setAlignment(Qt::AlignRight);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(new QLabel("<b>Welcome to MangaReader!</b>"));
  mainLayout->addWidget(new QLabel("Please set your scans directory:"));
  mainLayout->addLayout(scansLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  connect(this, SIGNAL(rejected()), m_scansDirectoryLineEdit, SLOT(clear()));

  setWindowTitle("Settup");
}

QString BrowseDirectoryDialog::getScansDirectory() const {
  return m_scansDirectoryLineEdit->text();
}

void BrowseDirectoryDialog::checkDirectoryValid(const QString&) {
  if (QDir(m_scansDirectoryLineEdit->text()).exists()) {
    m_scansDirectoryLineEdit->setStyleSheet(m_scansDirectoryLineEdit->styleSheet() + "color: black;");
    m_okButton->setEnabled(true);
  } else {
    m_scansDirectoryLineEdit->setStyleSheet(m_scansDirectoryLineEdit->styleSheet() + "color: red;");
    m_okButton->setEnabled(false);
  }
}

void BrowseDirectoryDialog::browse() {
  QString defaultDirectory = QDir::homePath();
  if (QDir(m_scansDirectoryLineEdit->text()).exists())
    defaultDirectory = m_scansDirectoryLineEdit->text();

  QString scanDirectory = QFileDialog::getExistingDirectory(this, "Browse scans directory", defaultDirectory);

  if (!scanDirectory.isEmpty())
    m_scansDirectoryLineEdit->setText(scanDirectory);
}
