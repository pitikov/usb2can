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


#ifndef ECIEVENT_H
#define ECIEVENT_H

#include <QEvent>


class EciEvent : public QEvent
{

public:
  typedef enum Type {
    ECI_Event_Begin = QEvent::User,
    
    ECI_Event_State,
    
    ECI_Event_Release,
    ECI_Event_DrvOpen,
    ECI_Event_DrvClose,
    ECI_Event_CtrlOpen,
    ECI_Event_CtrlClose,
    ECI_Event_CtrlSetFilter,
    ECI_Event_CtrlCommand,
    ECI_Event_CtrlSend,
    
    ECI_Event_Error,
    ECI_Event_DrvInfo,
    ECI_Event_CtrlCapabilities,
    ECI_Event_CtrlStatus,
    
    ECI_Event_Message,
    
    ECI_Event_End
  } Type;
  
  EciEvent ( Type type, void *data = NULL, int size = 0);
  ~EciEvent();
  Type type();
  void *data();
  int size();
private:
  Type type_;
  void *data_;
  int size_;
};

#endif // ECIEVENT_H
