#include "ChapterInfoWidget.h"

ChapterInfoWidget::ChapterInfoWidget(QWidget* parent) :
  QWidget(parent) {

  _numberFilesLabel = new QLabel;
  QFormLayout* numberFilesLayout = new QFormLayout;
  numberFilesLayout->addRow("Number of pages:", _numberFilesLabel);

  _totalSizeLabel = new QLabel;
  QFormLayout* totalSizeLayout = new QFormLayout;
  numberFilesLayout->addRow("Total size:", _totalSizeLabel);

  _lastModificationLabel = new QLabel;
  QFormLayout* lastModificationLayout = new QFormLayout;
  numberFilesLayout->addRow("Last modified:", _lastModificationLabel);

  _lastReadLabel = new QLabel;
  QFormLayout* lastReadLayout = new QFormLayout;
  numberFilesLayout->addRow("Last read:", _lastReadLabel);

  _imgPreviewLabel = new QLabel;
  _imgPreviewLabel->setFixedHeight(500);

  QVBoxLayout* imgPreviewLayout = new QVBoxLayout;
  imgPreviewLayout->addWidget(_imgPreviewLabel);
  imgPreviewLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(numberFilesLayout);
  mainLayout->addLayout(totalSizeLayout);
  mainLayout->addLayout(lastModificationLayout);
  mainLayout->addLayout(lastReadLayout);
  mainLayout->addSpacerItem(new QSpacerItem(0, 20));
  mainLayout->addLayout(imgPreviewLayout);

  mainLayout->setAlignment(Qt::AlignTop);

  setLayout(mainLayout);
  setFixedHeight(700);
}
