/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Maintainer: Peng Hui<penghui@deepin.com>
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

#include "shortcut.h"

Shortcut::Shortcut(QObject *parent) : QObject(parent)
{
    ShortcutGroup group1;
    ShortcutGroup group2;
    ShortcutGroup group3;
    ShortcutGroup group4;

    group1.groupName = tr("Start/Calendar");
    group2.groupName = tr("Search");
    group3.groupName = tr("Schedule Edit");
    group4.groupName = tr("Schedule Content Editing");

    group1.groupItems <<
                      ShortcutItem(tr("Window Size Toggle"), "Ctrl+Alt+F") <<
                      ShortcutItem(tr("Close"),  "Alt+F4") <<
                      ShortcutItem(tr("Help"),  "F1");

    group2.groupItems << ShortcutItem(tr("Search"), "Ctrl+F");


    group3.groupItems <<
                      ShortcutItem(tr("Create"),       "Ctrl+N") <<
                      ShortcutItem(tr("Delete"),       "Delete") <<
                      ShortcutItem(tr("Cancel Dialog"),   "Esc");;

    group4.groupItems <<
                      ShortcutItem(tr("Copy"), "Ctrl+C") <<
                      ShortcutItem(tr("Cut"),  "Ctrl+X") <<
                      ShortcutItem(tr("Paste"), "Ctrl+V") <<
                      ShortcutItem(tr("All election"), "Ctrl+A") <<
                      ShortcutItem(tr("Delete"), "Delete");
    m_shortcutGroups << group1 << group2 << group3 << group4;

    //convert to json object
    QJsonArray jsonGroups;
    for (auto scg : m_shortcutGroups) {
        QJsonObject jsonGroup;
        jsonGroup.insert("groupName", scg.groupName);
        QJsonArray jsonGroupItems;
        for (auto sci : scg.groupItems) {
            QJsonObject jsonItem;
            jsonItem.insert("name", sci.name);
            jsonItem.insert("value", sci.value);
            jsonGroupItems.append(jsonItem);
        }
        jsonGroup.insert("groupItems", jsonGroupItems);
        jsonGroups.append(jsonGroup);
    }
    m_shortcutObj.insert("shortcut", jsonGroups);
}
QString Shortcut::toStr()
{
    QJsonDocument doc(m_shortcutObj);
    return doc.toJson().data();
}
