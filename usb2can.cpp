#include "usb2can.h"
#include "eci.h"
#include "ecievent.h"

usb2can::usb2can()
  : QMainWindow()
  , ui(new Ui::USB2CAN)
  , tdevice(new TDevice(this))
  , modelLog(new QStandardItemModel(0, 1, this))
  , modelStatus(new QStandardItemModel(0, 2, this))
  , modelReceiver(new QStandardItemModel (0, 10, this))
  , itemStatusDevice(new QStandardItem())
  , itemDevSerial(new QStandardItem())
  , itemDevSerialValue(new QStandardItem())
  , itemDevHwVersion(new QStandardItem())
  , itemDevHwVersionValue(new QStandardItem())
  , itemDevBmVersion(new QStandardItem())
  , itemDevBmVersionValue(new QStandardItem())
  , itemDevFirmware(new QStandardItem())
  , itemDevFirmwareValue(new QStandardItem())
  , itemDevFwVersion(new QStandardItem())
  , itemDevFwVersionValue(new QStandardItem())
  , itemDevApiVersion(new QStandardItem())
  , itemDevApiVersionValue(new QStandardItem())
  , itemDevCtrlList(new QStandardItem())
  , itemStatusBus(new QStandardItem())
  , itemStatusAbbonentList(new QStandardItem())
{
  ui->setupUi(this);
  
  ui->listViewActionsLog->setModel(modelLog);
  ui->treeViewStatus->setModel(modelStatus);
  ui->treeViewReceiver->setModel(modelReceiver);
  
  ui->tableViewMsgDetail->hide();
  
  itemStatusDevice->setEditable(false);
  itemStatusAbbonentList->setEditable(false);
  itemStatusBus->setEditable(false);
  modelStatus->appendRow(itemStatusDevice);
  QList<QStandardItem*> itemlist;
  itemlist.append(itemDevSerial);
  itemlist.append(itemDevSerialValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevHwVersion);
  itemlist.append(itemDevHwVersionValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevBmVersion);
  itemlist.append(itemDevBmVersionValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevFirmware);
  itemlist.append(itemDevFirmwareValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevFwVersion);
  itemlist.append(itemDevFwVersionValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevApiVersion);
  itemlist.append(itemDevApiVersionValue);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  itemlist.append(itemDevCtrlList);
  itemStatusDevice->appendRow(itemlist);
  itemlist.clear();
  
  itemDevSerial->setEditable(false);
  itemDevSerialValue->setEditable(false);
  itemDevHwVersion->setEditable(false);
  itemDevHwVersionValue->setEditable(false);
  itemDevBmVersion->setEditable(false);
  itemDevBmVersionValue->setEditable(false);
  itemDevFirmware->setEditable(false);
  itemDevFirmwareValue->setEditable(false);
  itemDevFwVersion->setEditable(false);
  itemDevFwVersionValue->setEditable(false);
  itemDevApiVersion->setEditable(false);
  itemDevApiVersionValue->setEditable(false);
  itemDevCtrlList->setEditable(false);
  
  modelStatus->appendRow(itemStatusBus);
  modelStatus->appendRow(itemStatusAbbonentList);
  
  modelStatus->setHeaderData(0, Qt::Horizontal, tr("Properties"));
  modelStatus->setHeaderData(1, Qt::Horizontal, tr("Value"));
  status_reset();
  
  connect(ui->actionDevReset, SIGNAL(activated()), this, SLOT(dev_release()));
}

usb2can::~usb2can()
{
  tdevice->quit();
  while (tdevice->isRunning()) {}
  delete eci::device();
}

bool usb2can::event ( QEvent* event )
{
  bool ret = true;
  if ((event->type() >= (QEvent::Type)EciEvent::ECI_Event_Begin)&&(event->type() <= (QEvent::Type)EciEvent::ECI_Event_End)) {
    EciEvent* ecievent = (EciEvent*)event;
    switch (ecievent->type()) {
      case EciEvent::ECI_Event_Error :
	modelLog->appendRow(new QStandardItem(QIcon::fromTheme("dialog-error"), *(QString*)ecievent->data()));
	break;
      case EciEvent::ECI_Event_DrvInfo :
	if (ecievent->size() == sizeof(ECI_HW_INFO)) {
	  dev_info((ECI_HW_INFO*)ecievent->data());
	}
	break;
      case EciEvent::ECI_Event_CtrlCapabilities :
	break;
      case EciEvent::ECI_Event_CtrlStatus :
	break;
      case EciEvent::ECI_Event_State :
	state(*(eci::State*)ecievent->data());
	break;
      case EciEvent::ECI_Event_Message :
	break;
      default:
	modelLog->appendRow(new QStandardItem(QIcon::fromTheme("dialog-error"), tr("Unsupported event type") + " - " + QString::number(ecievent->type())));
	break;
    }
  } else ret = QMainWindow::event ( event );
  return ret;
}

void usb2can::state ( eci::State state )
{
  switch (state) {
    case eci::ECI_STATE_Uninitilazed :
      ui->menuReceiver->setDisabled(true);
      ui->toolBarRcv->setDisabled(true);
      ui->actionDevClose->setDisabled(true);
      ui->actionDevOpen->setDisabled(true);
      if (modelLog->rowCount()!=0) modelLog->appendRow(new QStandardItem(QIcon::fromTheme("system-reboot"), tr("Device released")));
      status_reset();
      break;
    case eci::ECI_STATE_Initilazed :
      ui->menuReceiver->setDisabled(true);
      ui->toolBarRcv->setDisabled(true);
      //ui->actionDevOpen->setEnabled(true);
      ui->actionDevClose->setDisabled(true);
      modelLog->appendRow(new QStandardItem(QIcon::fromTheme("dialog-information"), tr("Device initilazed")));
      break;
    case eci::ECI_STATE_Configured :
      break;
    case eci::ECI_STATE_Running :
      break;
    default:
      break;
  }
}

void usb2can::dev_release()
{
  QApplication::postEvent(eci::device(), new EciEvent(EciEvent::ECI_Event_Release));
}

void usb2can::status_reset()
{
  itemStatusDevice->setText(tr("Device undetected"));
  itemStatusDevice->setData(QIcon::fromTheme("dialog-error"), Qt::DecorationRole);
  
  itemDevSerial->setText(tr("serial"));
  itemDevSerialValue->setText("");
  itemDevHwVersion->setText(tr("hardware"));
  itemDevHwVersionValue->setText("");
  itemDevBmVersion->setText(tr("bootmanager"));
  itemDevBmVersionValue->setText("");
  itemDevFirmware->setText(tr("firmware"));
  itemDevFirmwareValue->setText("");
  itemDevFwVersion->setText(tr("fw.version"));
  itemDevFwVersionValue->setText("");
  itemDevApiVersion->setText(tr("api version"));
  itemDevApiVersionValue->setText("");
  itemDevCtrlList->setText("ctrl list");
  for (int id = 0; id < ECI_MAXCTRLCOUNT; id++) {
    ctrlList[id].Class.setText(tr("Undetected"));
    ctrlList[id].Class.setData(QIcon::fromTheme("dialog-warning"), Qt::DecorationRole);
    ctrlList[id].Status.setText("");
  }
  
  ui->treeViewStatus->collapse(itemStatusDevice->index());
  status_bus_reset();
}

void usb2can::status_bus_reset()
{
  itemStatusBus->setText(tr("Bus access deny"));
  itemStatusBus->setData(QIcon::fromTheme("dialog-error"), Qt::DecorationRole);
  ui->treeViewStatus->collapse(itemStatusBus->index());
  
  itemStatusAbbonentList->setData(QIcon::fromTheme("dialog-error"), Qt::DecorationRole);
  itemStatusAbbonentList->setText(tr("Abbonent list acces deny"));
  ui->treeViewStatus->collapse(itemStatusAbbonentList->index());
}

void usb2can::status_abbonents_reset()
{
  itemStatusAbbonentList->setData(QIcon::fromTheme("dialog-warning"), Qt::DecorationRole);
  itemStatusAbbonentList->setText(tr("Abbonent list is empty"));
  ui->treeViewStatus->collapse(itemStatusAbbonentList->index());
}

void usb2can::dev_info ( ECI_HW_INFO* info )
{
  itemStatusDevice->setData(QIcon::fromTheme("hwinfo"), Qt::DecorationRole);
  itemStatusDevice->setText(info->u.V0.szHwBoardType);
  
  itemDevSerialValue->setText(QString(info->u.V0.szHwSerial));
  itemDevHwVersionValue->setText(QString::number(info->u.V0.abHwVersion[0])+"."+QString::number(info->u.V0.abHwVersion[1])+"."+QString::number(info->u.V0.abHwVersion[2])+"."+QString::number(info->u.V0.abHwVersion[3]));
  itemDevBmVersionValue->setText(QString::number(info->u.V0.abBmVersion[0])+"."+QString::number(info->u.V0.abBmVersion[1])+"."+QString::number(info->u.V0.abBmVersion[2])+"."+QString::number(info->u.V0.abBmVersion[3]));
  itemDevFirmwareValue->setText(QString(info->u.V0.szFwIdentification));
  itemDevFwVersionValue->setText(QString::number(info->u.V0.abFwVersion[0])+"."+QString::number(info->u.V0.abFwVersion[1])+"."+QString::number(info->u.V0.abFwVersion[2])+"."+QString::number(info->u.V0.abFwVersion[3]));
  itemDevApiVersionValue->setText(QString::number(info->u.V0.adwApiVersion[0])+"."+QString::number(info->u.V0.adwApiVersion[1])+"."+QString::number(info->u.V0.adwApiVersion[2])+"."+QString::number(info->u.V0.adwApiVersion[3]));
  
  for (int id = 0; id < ECI_MAXCTRLCOUNT; id++) {
    switch (info->u.V0.sCtrlInfo[id].wCtrlClass) {
      case ECI_CTRL_CAN :
	ctrlList[id].Class.setText("CAN");
	ctrlList[id].Class.setData(QIcon::fromTheme("cpu"), Qt::DecorationRole);
	break;
      case ECI_CTRL_LIN :
	ctrlList[id].Class.setText("LIN");
	ctrlList[id].Class.setData(QIcon::fromTheme("dialog-warning"), Qt::DecorationRole);
	break;
      case ECI_CTRL_FLX :
	ctrlList[id].Class.setText("FlexRay");
	ctrlList[id].Class.setData(QIcon::fromTheme("dialog-warning"), Qt::DecorationRole);
	break;
      case ECI_CTRL_KLI :
	ctrlList[id].Class.setText("K-Line");
	ctrlList[id].Class.setData(QIcon::fromTheme("dialog-warning"), Qt::DecorationRole);
	break;
      case ECI_CTRL_UNDEFINED :
      default:
	ctrlList[id].Class.setText("Undefined");
	ctrlList[id].Class.setData(QIcon::fromTheme("dialog-error"), Qt::DecorationRole);
	break;
    }
    switch (info->u.V0.sCtrlInfo[id].wCtrlState) {
      case ECI_CTRL_UNCONFIGURED :
	ctrlList[id].Status.setText(tr("Unconfigured"));
	ctrlList[id].Status.setData(QIcon::fromTheme("user-offline"), Qt::DecorationRole);
	break;
      case ECI_CTRL_INITIALIZED :
	ctrlList[id].Status.setText(tr("Initilazed"));
	ctrlList[id].Status.setData(QIcon::fromTheme("user-invisible"), Qt::DecorationRole);
	break;
      case ECI_CTRL_RUNNING :
	ctrlList[id].Status.setText(tr("Runing"));
	ctrlList[id].Status.setData(QIcon::fromTheme("user-online"), Qt::DecorationRole);
	break;
      default:
	ctrlList[id].Status.setText(tr("Error"));
	ctrlList[id].Status.setData(QIcon::fromTheme("dialog-error"), Qt::DecorationRole);
	break;
    }
    ctrlList[id].Class.setEditable(false);
    ctrlList[id].Status.setEditable(false);
  }

  if (itemDevCtrlList->rowCount()!=info->u.V0.dwCtrlCount) 
  for (int id = 0; id < info->u.V0.dwCtrlCount; id++) {
    QList<QStandardItem*> listitem;
    listitem.append(&ctrlList[id].Class);
    listitem.append(&ctrlList[id].Status);
    itemDevCtrlList->appendRow(listitem);
  }
  
  ui->treeViewStatus->expand(itemStatusDevice->index());
  ui->treeViewStatus->expand(itemDevCtrlList->index());
  ui->actionDevOpen->setEnabled(true);
}


#include "usb2can.moc"
