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
#include "service_stub.h"

#include "dbusuiopenschedule.h"
#include "dbusnotify.h"

QDBusMessage callWithArgumentList_stub(QDBus::CallMode mode,
                                       const QString &method,
                                       const QList<QVariant> &args)
{
    Q_UNUSED(mode);
    Q_UNUSED(method);
    Q_UNUSED(args);
    QDBusMessage reply;
    reply.createReply("succ");
    return reply;
}

void qDBusAbstractInterface_callWithArgumentList_stub(Stub &stub)
{
    stub.set(ADDR(QDBusAbstractInterface, callWithArgumentList), callWithArgumentList_stub);
    qDBusMessage_type_Stub(stub);
}

QDBusMessage::MessageType type_Stub()
{
    return QDBusMessage::ReplyMessage;
}

void qDBusMessage_type_Stub(Stub &stub)
{
    stub.set(ADDR(QDBusMessage, type), type_Stub);
}
