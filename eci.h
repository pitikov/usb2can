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


#ifndef ECI_H
#define ECI_H

#include <QtCore>
#include <OsEci.h>
#include <ECI_hwtype.h>

class eci : public QObject
{
  Q_OBJECT

public:
  static eci *device( QObject *gui = NULL );
  ~eci();

  typedef enum State {
    ECI_STATE_Uninitilazed,
    ECI_STATE_Initilazed,
    ECI_STATE_Configured,
    ECI_STATE_Running
  } State;
private:
  virtual bool event ( QEvent* event );
  eci( QObject* gui );
  
  static eci	*self_;
  QObject	*event_trap;
  QMutex	*mutex_ctrlState;
  State		ctrl_state;
  QTimer	*timerCtrlProcess;
  ECI_HW_PARA	*stc_hw_param;
  ECI_HW_INFO	*stc_hw_info;
  
private slots:
  void ctrlProcess();
  
  void dev_initilaze();
  void dev_relese();
  void error( DWORD errnum );
  void dev_getinfo();

};

#endif // ECI_H
