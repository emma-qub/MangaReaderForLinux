#include "ChapterInfoWidget.h"

ChapterInfoWidget::ChapterInfoWidget(QWidget* parent) :
  QWidget(parent) {

  m_numberFilesLabel = new QLabel;
  QFormLayout* numberFilesLayout = new QFormLayout;
  numberFilesLayout->addRow("Number of pages:", m_numberFilesLabel);

  m_totalSizeLabel = new QLabel;
  QFormLayout* totalSizeLayout = new QFormLayout;
  numberFilesLayout->addRow("Total size:", m_totalSizeLabel);

  m_lastModificationLabel = new QLabel;
  QFormLayout* lastModificationLayout = new QFormLayout;
  numberFilesLayout->addRow("Last modified:", m_lastModificationLabel);

  m_lastReadLabel = new QLabel;
  QFormLayout* lastReadLayout = new QFormLayout;
  numberFilesLayout->addRow("Last read:", m_lastReadLabel);

  m_imgPreviewLabel = new QLabel;
  m_imgPreviewLabel->setFixedHeight(500);

  QVBoxLayout* imgPreviewLayout = new QVBoxLayout;
  imgPreviewLayout->addWidget(m_imgPreviewLabel);
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
