/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2014  Евгений Питиков <pitikov@yandex.ru>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "eci.h"
#include "ecievent.h"
#include <ECI109.h>
#include <ECI.h>
#include <ECI_align.h>
#include <ECI_cantype.h>
#include <ECI_error.h>
#include <ECI_lintype.h>
#include <ECI_pshpack1.h>
#include <ECI_poppack.h>
#include <ECI_strings.h>
#include <QApplication>

eci* eci::self_ = NULL;

eci* eci::device ( QObject* gui )
{
  if (!self_) {
    if (gui) {
      self_ = new eci(gui);
    } else {
      while (!self_) {}
    }
  }
  return self_;
}

bool eci::event ( QEvent* event )
{
  bool ret = true;
  if ((event->type() >= (QEvent::Type)EciEvent::ECI_Event_Begin) && (event->type() <= (QEvent::Type)EciEvent::ECI_Event_End)) {
    EciEvent *ecievent = (EciEvent*)event;
    switch (ecievent->type()) {
      case EciEvent::ECI_Event_Release:
	dev_relese();
	break;
      case EciEvent::ECI_Event_DrvOpen:
	break;
      case EciEvent::ECI_Event_DrvClose:
	break;
      case EciEvent::ECI_Event_CtrlOpen:
	break;
      case EciEvent::ECI_Event_CtrlClose:
	break;
      case EciEvent::ECI_Event_CtrlSetFilter:
	break;
      case EciEvent::ECI_Event_CtrlCommand:
	break;
      case EciEvent::ECI_Event_CtrlSend:
	break;
      default:
	QString err = tr("Unsupported event type");
	QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_Error, &err, sizeof err));
	break;
    }
  } else ret = QObject::event( event );
  return ret;
}

eci::eci( QObject *gui )
  : event_trap(gui)
  , mutex_ctrlState(new QMutex(QMutex::NonRecursive))
  , ctrl_state(ECI_STATE_Uninitilazed)
  , timerCtrlProcess(new QTimer(this))
  , stc_hw_param(new ECI_HW_PARA)
  , stc_hw_info(new ECI_HW_INFO)
{
  timerCtrlProcess->setInterval(1000);
  QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_State, &ctrl_state, sizeof ctrl_state));
  
  connect(timerCtrlProcess, SIGNAL(timeout()), this, SLOT(ctrlProcess()));
  timerCtrlProcess->start();
}

eci::~eci()
{
}



void eci::ctrlProcess()
{ 
  switch (ctrl_state) {
    case ECI_STATE_Uninitilazed :
      dev_initilaze();
      break;
    case ECI_STATE_Initilazed :
      dev_getinfo();
      break;
    case ECI_STATE_Configured :
      break;
    case ECI_STATE_Running :
      break;
    default:
      dev_relese();
      break;
  }
}

void eci::dev_initilaze()
{
  ECI_RESULT result = ECI_ERR_ACCESS_DENIED;
  memset(stc_hw_param, 0, sizeof(ECI_HW_PARA));
  stc_hw_param->wHardwareClass = ECI_HW_USB;
  stc_hw_param->dwFlags = ECI_SETTINGS_FLAG_NONE;
  result = ECIDRV_Initialize(1, stc_hw_param);
  if (result == ECI_OK) { 
    timerCtrlProcess->stop();
    mutex_ctrlState->lock();
    ctrl_state = ECI_STATE_Initilazed;
    QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_State, &ctrl_state, sizeof ctrl_state));
    mutex_ctrlState->unlock();
    timerCtrlProcess->setInterval(3000);
    timerCtrlProcess->start();
  } else {
    error( result );
  }
}

void eci::dev_relese()
{
  ECI_RESULT result = ECI_OK;
  result = ECIDRV_Release();
  if (result == ECI_OK) {
    mutex_ctrlState->lock();
    ctrl_state = ECI_STATE_Uninitilazed;
    QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_State, &ctrl_state, sizeof ctrl_state));
    mutex_ctrlState->unlock();
  } else {
    error( result );
  }
}

void eci::error ( DWORD errnum )
{
  if (ctrl_state != ECI_STATE_Uninitilazed) {
    QString err_str = QString(ECIDRV_GetErrorString(errnum));
    QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_Error, &err_str, sizeof err_str));
  }
}

void eci::dev_getinfo()
{
  ECI_RESULT result = ECI_OK;
  memset(stc_hw_info, 0, sizeof(ECI_HW_INFO));
  result = ECIDRV_GetInfo(0, stc_hw_info);
  if (result == ECI_OK) {
    QApplication::postEvent(event_trap, new EciEvent(EciEvent::ECI_Event_DrvInfo, stc_hw_info, sizeof(ECI_HW_INFO)));
  } else {
    error(result);
    dev_relese();
  }
}

#include "eci.moc"

