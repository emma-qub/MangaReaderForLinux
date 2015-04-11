#include "BrowseDirectoryDialog.h"

BrowseDirectoryDialog::BrowseDirectoryDialog(QWidget* parent, Qt::WindowFlags flags):
  QDialog(parent, flags) {

  _scansDirectoryLineEdit = new QLineEdit(QDir::homePath());
  _scansDirectoryLineEdit->setFixedWidth(300);
  connect(_scansDirectoryLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(checkDirectoryValid(const QString&)));
  _browseDirectoryButton = new QPushButton("Browse");
  _browseDirectoryButton->setFixedWidth(100);
  connect(_browseDirectoryButton, SIGNAL(clicked(void)), this, SLOT(browse(void)));

  QFormLayout* scansLayout = new QFormLayout;
  scansLayout->addRow(_scansDirectoryLineEdit, _browseDirectoryButton);

  _okButton = new QPushButton("OK");
  _okButton->setFixedWidth(100);
  connect(_okButton, SIGNAL(clicked(void)), this, SLOT(accept(void)));

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(_okButton);
  buttonLayout->setAlignment(Qt::AlignRight);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(new QLabel("<b>Welcome to MangaReader!</b>"));
  mainLayout->addWidget(new QLabel("Please set your scans directory:"));
  mainLayout->addLayout(scansLayout);
  mainLayout->addLayout(buttonLayout);

  setLayout(mainLayout);

  connect(this, SIGNAL(rejected(void)), _scansDirectoryLineEdit, SLOT(clear(void)));

  setWindowTitle("Settup");
}

BrowseDirectoryDialog::~BrowseDirectoryDialog(void) {
}

void BrowseDirectoryDialog::checkDirectoryValid(const QString&) {
  if (QDir(_scansDirectoryLineEdit->text()).exists()) {
    _scansDirectoryLineEdit->setStyleSheet(_scansDirectoryLineEdit->styleSheet() + "color: black;");
    _okButton->setEnabled(true);
  } else {
    _scansDirectoryLineEdit->setStyleSheet(_scansDirectoryLineEdit->styleSheet() + "color: red;");
    _okButton->setEnabled(false);
  }
}

void BrowseDirectoryDialog::browse(void) {
  QString defaultDirectory = QDir::homePath();
  if (QDir(_scansDirectoryLineEdit->text()).exists())
    defaultDirectory = _scansDirectoryLineEdit->text();

  QString scanDirectory = QFileDialog::getExistingDirectory(this, "Browse scans directory", defaultDirectory);

  if (!scanDirectory.isEmpty())
    _scansDirectoryLineEdit->setText(scanDirectory);
}
