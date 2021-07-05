#include <QFormLayout>
#include <QLabel>

#include "../Game.h"
#include "../Rom.h"

#include "RomInfo.h"

RomInfo::RomInfo(QWidget *parent, Rom& rom, Game& game)
  : QDialog(parent)
{
  auto layout = new QFormLayout(this);
  setLayout(layout);

  // identified as
  auto identifiedAsLabel = new QLabel(this);
  identifiedAsLabel->setText(tr("Identified as:"));
  auto identifiedAsField = new QLabel(this);
  identifiedAsField->setText(game.getIdentifier());
  layout->addRow(identifiedAsLabel, identifiedAsField);

  // file size
  auto fileSizeLabel = new QLabel(this);
  fileSizeLabel->setText(tr("File size:"));
  auto fileSizeField = new QLabel(this);
  auto fileSize = rom.getSize();
  fileSizeField->setText(QString::number(fileSize) + tr(" bytes"));
  layout->addRow(fileSizeLabel, fileSizeField);

  // rom size (header)
  auto romSizeLabel = new QLabel(this);
  romSizeLabel->setText(tr("Rom size (header):"));
  auto romSizeField = new QLabel(this);
  auto romSize = rom.readSize();
  romSizeField->setText(QString::number(romSize) + tr(" bytes"));
  layout->addRow(romSizeLabel, romSizeField);

  // checksum in rom
  auto romChecksumLabel = new QLabel(this);
  romChecksumLabel->setText(tr("Rom checksum:"));
  auto romChecksumField = new QLabel(this);
  auto romChecksum = rom.readChecksum();
  romChecksumField->setText(QString::number(romChecksum));
  layout->addRow(romChecksumLabel, romChecksumField);

  // calculated checksum
  auto calculatedChecksumLabel = new QLabel(this);
  calculatedChecksumLabel->setText(tr("Calculated checksum:"));
  auto calculatedChecksumField = new QLabel(this);
  auto calculatedChecksum = rom.calculateChecksum();
  calculatedChecksumField->setText(QString::number(calculatedChecksum));
  layout->addRow(calculatedChecksumLabel, calculatedChecksumField);
}
