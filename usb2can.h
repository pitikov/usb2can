#ifndef usb2can_H
#define usb2can_H

#include <QtGui>
#include "ui_usb2can.h"

namespace Ui {
  class USB2CAN;
}

class usb2can : public QMainWindow
{
Q_OBJECT
public:
    usb2can();
    virtual ~usb2can();
    
private:
  Ui::USB2CAN *ui;
};

#endif // usb2can_H
