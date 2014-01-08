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


#include "ecievent.h"

EciEvent::EciEvent ( EciEvent::Type type, void* data, int size ) 
  : QEvent((QEvent::Type)type)
  , type_(type)
  , data_(NULL)
  , size_(size)
{
  if (( data != NULL )&(size > 0)) {
    data_ = malloc(size_);
    if (data_) memcpy(data_, data, size_);
  }
}

EciEvent::~EciEvent()
{
  free(data_);
}

void *EciEvent::data()
{
  return data_;
}

EciEvent::Type EciEvent::type()
{
  return type_;
}

int EciEvent::size()
{
  return size_;
}

