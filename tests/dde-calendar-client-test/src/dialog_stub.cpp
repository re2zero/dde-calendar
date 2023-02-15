/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "dialog_stub.h"

#include "dialog/dcalendarddialog.h"
#include "dialog/myscheduleview.h"

#include <QObject>

int calendarDDialogExecReturn = 0;

int calendar_DDialog_Exec_stub(void *obj)
{
    Q_UNUSED(obj)
    return calendarDDialogExecReturn;
}
void calendarDDialogExecStub(Stub &stub)
{
    typedef int (*fptr)(DDialog *);
    fptr A_foo = (fptr)(&DDialog::exec);
    stub.set(A_foo, calendar_DDialog_Exec_stub);

    typedef int (*fptr2)(QDialog *);
    fptr2 A_foo2 = (fptr2)(&QDialog::exec);
    stub.set(A_foo2, calendar_DDialog_Exec_stub);
}
