#include "usb2can.h"

usb2can::usb2can()
  : QMainWindow()
  , ui(new Ui::USB2CAN)
{
  ui->setupUi(this);
}

usb2can::~usb2can()
{}

#include "usb2can.moc"
