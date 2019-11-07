/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#ifndef CSCHCEDULECTRLDLG_H
#define CSCHCEDULECTRLDLG_H

#include <QObject>
#include <DLabel>
#include <DDialog>
#include <DPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
DWIDGET_USE_NAMESPACE

class CSchceduleCtrlDlg : public DDialog
{
    Q_OBJECT
public:
    explicit CSchceduleCtrlDlg(QWidget *parent = nullptr);
    DPushButton *addPushButton(QString btName, int type = 0);
    void setTitleName(QString tileName);
    void setText(QString str);
    void setInformativeText(QString str);
    DPushButton *clickButton();
    void updatesize();
private:
    void initUI();
    void initConnection();

private slots:
    void buttonJudge(int id);
private:
    DLabel                           *m_firstLabel = nullptr;
    DLabel                           *m_seconLabel = nullptr;
    DLabel                           *m_icon;
    DLabel                           *m_Title;
    int                              m_id = -1;
    QButtonGroup                     *m_Buttongroup;
    QVector<DPushButton *>            m_buttonlist;
    QVBoxLayout                      *m_mainBoxLayout;
    QHBoxLayout                      *m_btBoxLayout;
};

#endif // MYSCHCEDULEVIEW_H
