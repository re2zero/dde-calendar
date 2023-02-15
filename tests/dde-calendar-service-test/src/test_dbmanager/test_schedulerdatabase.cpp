/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leilong  <leilong@uniontech.com>
*
* Maintainer: leilong  <leilong@uniontech.com>
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
#include "test_schedulerdatabase.h"

test_schedulerdatabase::test_schedulerdatabase()
{

}

TEST_F(test_schedulerdatabase, GetJob_01)
{
    schedulerdatabase_next = false;
    mBase->GetJob(0);
}

TEST_F(test_schedulerdatabase, GetAllOriginJobs_01)
{
    schedulerdatabase_next = false;
    mBase->GetAllOriginJobs("", "");
}

TEST_F(test_schedulerdatabase, saveRemindJob_01)
{
    schedulerdatabase_next = false;
    mBase->saveRemindJob(Job());
}

TEST_F(test_schedulerdatabase, updateRemindJob_01)
{
    schedulerdatabase_next = false;
    mBase->updateRemindJob(Job());
}

TEST_F(test_schedulerdatabase, getValidRemindJob_01)
{
    schedulerdatabase_next = false;
    mBase->getValidRemindJob();
}

TEST_F(test_schedulerdatabase, getRemindJob_01)
{
    schedulerdatabase_next = false;
    mBase->getRemindJob(1, 2);
}

TEST_F(test_schedulerdatabase, getRemindJob_02)
{
    schedulerdatabase_next = false;
    mBase->getRemindJob(1);
}

TEST_F(test_schedulerdatabase, UpdateJobIgnore_01)
{
    schedulerdatabase_next = false;
    mBase->UpdateJobIgnore("", 0);
}

TEST_F(test_schedulerdatabase, DeleteJobsByJobType_01)
{
    schedulerdatabase_next = false;
    mBase->DeleteJobsByJobType(0);
}

TEST_F(test_schedulerdatabase, getJobIDByJobType_01)
{
    schedulerdatabase_next = false;
    mBase->getJobIDByJobType(0);
}

TEST_F(test_schedulerdatabase, updateColorType_01)
{
    schedulerdatabase_next = false;
    mBase->updateColorType(0, "#000000");
}

TEST_F(test_schedulerdatabase, setDbPath_01)
{
    schedulerdatabase_next = false;
    mBase->setDbPath("123");
    EXPECT_EQ(mBase->m_dbPath, "123");
}
