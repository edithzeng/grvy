// -*-c++-*-
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//
// Copyright (C) 2008,2009,2010 The PECOS Development Team
//
// Please see http://pecos.ices.utexas.edu for more information.
//
// This file is part of the PECOS HPC Toolkit (HPCT)
//
// HPCT is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// HPCT is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HPCT.  If not, see <http://www.gnu.org/licenses/>.
//
//--------------------------------------------------------------------------
// hpct_log: Application logging related routines.
//
// $Id$
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

#include<stdio.h>
#include<hpct_classes.h>
#include<hpct_int.h>
#include<hpct.h>
#include<string>

using namespace std;
using namespace HPCT;

void hpct_log_setlevel(int priority)
{
  _HPCT_Log.change_priority(priority);
  return;
}

void hpct_log(int loglevel, const char *mesg)
{
  _HPCT_Log.msg(loglevel,mesg);
}

//-----------------------------------------------------------------
//                     Fortran Interfaces
//-----------------------------------------------------------------

extern "C" void hpct_log_setlevel_(int *priority) {
  hpct_log_setlevel(*priority);
}

extern "C" void hpct_log_(int loglevel, char *mesg, int _namelen)
{
  char *message = hpct_f2c_char(mesg,_namelen);
  _HPCT_Log.msg(loglevel,message);
  delete[] message;
}



