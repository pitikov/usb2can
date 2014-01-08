#ifndef usb2can_H
#define usb2can_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QIcon>
#include <QString>
#include <QApplication>
#include <QTreeView>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>

#include "eci.h"
#include "ui_usb2can.h"
#include "tdevice.h"

namespace Ui {
  class USB2CAN;
}

class usb2can : public QMainWindow
{
Q_OBJECT

typedef struct stc_device_ctrl {
  QStandardItem Class;
  QStandardItem Status;
} stc_device_ctrl;
public:
    usb2can();
    ~usb2can();
    
private:
  bool event(QEvent *event);
  void state( eci::State state );
  void status_reset();
  void status_bus_reset();
  void status_abbonents_reset();
  void dev_info( ECI_HW_INFO *info );
  
  Ui::USB2CAN 		*ui;
  TDevice		*tdevice;
  QStandardItemModel	*modelLog;
  QStandardItemModel	*modelStatus;
  QStandardItemModel	*modelReceiver;
  
  QStandardItem		*itemStatusDevice;
  QStandardItem		*itemDevSerial, *itemDevSerialValue;
  QStandardItem		*itemDevHwVersion, *itemDevHwVersionValue;
  QStandardItem		*itemDevBmVersion, *itemDevBmVersionValue;
  QStandardItem		*itemDevFirmware, *itemDevFirmwareValue;
  QStandardItem		*itemDevFwVersion, *itemDevFwVersionValue;
  QStandardItem		*itemDevApiVersion, *itemDevApiVersionValue;
  QStandardItem		*itemDevCtrlList;
  stc_device_ctrl	ctrlList[ECI_MAXCTRLCOUNT];
  
  
  QStandardItem		*itemStatusBus;
  QStandardItem		*itemStatusAbbonentList;
  
private slots:
  void dev_release();
};

#endif // usb2can_H
