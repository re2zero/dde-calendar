#include "monthgraphiview.h"
#include <DHiDPIHelper>
#include <QAction>
#include <DMenu>
#include <QMimeData>
#include <QShortcut>

#include "monthschceduleview.h"
#include "schceduledlg.h"
#include "schcedulectrldlg.h"
#include "myschceduleview.h"


bool CDayGraphicsItem::m_LunarVisible = false;

CMonthGraphiview::CMonthGraphiview(QWidget *parent)
    : DGraphicsView(parent),
      m_Scene(new QGraphicsScene(this))
{
    setFrameShape(DFrame::NoFrame);
    setContentsMargins(0, 0, 0, 0);
    this->setViewportMargins(0, 0, 0, 0);
    this->setScene(m_Scene);
    for (int i = 0; i < 42; ++i) {
        CDayGraphicsItem *item = new CDayGraphicsItem();
        item->setZValue(-1);
        m_DayItem.append(item);
        m_Scene->addItem(item);
    }
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    m_createAction = new QAction(tr("New event"), this);
    connect(m_createAction, &QAction::triggered,
            this,  static_cast<void (CMonthGraphiview::*)()>(&CMonthGraphiview::slotCreate));

    QShortcut *shortcut = new QShortcut(parent);
    shortcut->setKey(QKeySequence(QLatin1String("Delete")));
    connect(shortcut,
            &QShortcut::activated, this,
            &CMonthGraphiview::slotDeleteItem);

    m_MonthSchceduleView = new CMonthSchceduleView(this,m_Scene);

//    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalsUpdateShcedule, this, &CMonthView::slotSchceduleUpdate);
//    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalsUpdateShcedule, this, &CMonthView::slotdelete);
//    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalsCurrentScheduleDate, this, &CMonthView::signalsCurrentScheduleDate);
//    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalViewtransparentFrame, this, &CMonthView::signalViewtransparentFrame);
//    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalUpdateUI, this, &CMonthView::slotUpdateUI);
//    connect(m_MonthSchceduleView
//            , &CMonthSchceduleView::signalPressScheduleShow
//            , this
//            , &CMonthView::slotScheduleRemindWidget);
    connect(this,&CMonthGraphiview::signalFontChange,
            m_MonthSchceduleView,&CMonthSchceduleView::slotFontChange);

    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    updateSize();
}

CMonthGraphiview::~CMonthGraphiview()
{
    delete m_MonthSchceduleView;
    for (int i = 0; i < m_DayItem.size(); ++i) {
        CDayGraphicsItem *item = m_DayItem.at(i);
        delete item;
        item = nullptr;
    }
    m_DayItem.clear();
}

void CMonthGraphiview::setTheMe(int type)
{
    m_themetype = type;
    for (int i = 0; i < m_DayItem.size(); ++i) {
        m_DayItem.at(i)->setTheMe(type);
    }
    m_MonthSchceduleView->setTheMe(type);
}

void CMonthGraphiview::setDate(const QDate date[42])
{
    if (date[0].day() !=1) {
        m_currentMonth = date[0].addMonths(1).month();
    } else {
        m_currentMonth = date[0].month();
    }
    for (int i = 0; i < m_DayItem.size(); ++i) {
        m_DayItem.at(i)->setData(date[i]);
        m_DayItem.at(i)->setCurrentMonth(date[i].month() == m_currentMonth);
        m_DayItem.at(i)->setStatus(CDayGraphicsItem::HolidayStatus(getFestivalInfoByDate(date[i])));
    }
}

void CMonthGraphiview::setLunarInfo(QMap<QDate, CaLunarDayInfo> *lunarCache)
{
    m_lunarCache = lunarCache;
    updateLunar();

}

void CMonthGraphiview::setLunarVisible(bool visible)
{
    m_LunarVisible = visible;
    CDayGraphicsItem::m_LunarVisible = visible;
}

void CMonthGraphiview::setScheduleInfo(const QVector<ScheduleDateRangeInfo> &info)
{
    m_shceludelistdata = info;
    updateInfo();
}

void CMonthGraphiview::setSelectSchedule(const ScheduleDtailInfo &scheduleInfo)
{
    QVector<QGraphicsRectItem *> mscheduleShowBtn = m_MonthSchceduleView->getScheduleShowItem();
    for (int i = 0; i < mscheduleShowBtn.size(); ++i) {
        CMonthSchceduleWidgetItem *item = dynamic_cast<CMonthSchceduleWidgetItem *>(mscheduleShowBtn.at(i));
        if (item == nullptr) continue;
        if (scheduleInfo == item->getData()) {
            item->setStartValue(0);
            item->setEndValue(4);
            item->startAnimation();
        }
    }
}

void CMonthGraphiview::updateSize()
{
    m_Scene->setSceneRect(this->viewport()->rect());
    qreal w = m_Scene->width()/7;
    qreal h = m_Scene->height()/6;
    QRectF rect ;
    int w_offset = 0;
    int h_offset = 0;
    for (int i = 0 ; i < m_DayItem.size(); ++i) {
        h_offset = i / 7;
        w_offset = i % 7;
        rect.setRect(w*w_offset,
                     h*h_offset,
                     w,
                     h);
        m_DayItem.at(i)->setRect(rect);
    }
}

void CMonthGraphiview::updateLunar()
{
    QDate date;
    CaLunarDayInfo info;
    QString lunarStr("");
    for (int i = 0 ; i < m_DayItem.size(); ++i) {
        date = m_DayItem.at(i)->getDate();
        if (m_lunarCache->contains(date)) {
            info = m_lunarCache->value(date);
            if (info.mLunarDayName == "初一") {
                info.mLunarDayName = info.mLunarMonthName + info.mLunarDayName;
            }
            if (info.mTerm.isEmpty()) {
                lunarStr = info.mLunarDayName;
            } else {
                lunarStr = info.mTerm;
            }
        } else {
            lunarStr = "";
        }
        m_DayItem.at(i)->setLunar(lunarStr);
    }
}

void CMonthGraphiview::updateInfo()
{
    m_infoitem = nullptr;
    int h = m_MonthSchceduleView->getSchceduleHeight();
    m_MonthSchceduleView->setallsize(this->viewport()->width(),
                                     this->viewport()->height(),
                                     0, 0, 0, h);
    m_MonthSchceduleView->setData(m_shceludelistdata, 1);
}

char CMonthGraphiview::getFestivalInfoByDate(const QDate &date)
{
    for (int i = 0; i < m_festivallist.count(); i++) {
        for (int j = 0; j < m_festivallist[i].listHoliday.count(); j++) {

            if (m_festivallist[i].listHoliday[j].date == date) {
                return m_festivallist[i].listHoliday[j].status;
            }
        }
    }
    return 0;
}

QPointF CMonthGraphiview::getItemPos(const QPoint &p, const QRectF &itemRect)
{
    QPointF scenePos = this->mapToScene(p);
    return  QPointF(scenePos.x()-itemRect.x(),
                    scenePos.y()-itemRect.y());

}

void CMonthGraphiview::DragPressEvent(const QPoint &pos,QGraphicsItem *item)
{
    m_PressPos = pos;
    m_PressDate = getPosDate(pos);
    m_MoveDate = m_PressDate.addMonths(-2);
    CDayGraphicsItem *Dayitem = dynamic_cast<CDayGraphicsItem *>(item);
    if (Dayitem != nullptr) {
        m_DragStatus = IsCreate;
        m_isCreate = false;
        return;
    }
    CMonthSchceduleWidgetItem *infoitem = dynamic_cast<CMonthSchceduleWidgetItem *>(item);
    if (infoitem != nullptr) {
        m_DragScheduleInfo = infoitem->getData();
        if (m_DragScheduleInfo.type.ID == 4) {
            return;
        }
        m_PressRect = infoitem->rect();
        m_PressScheduleInfo = infoitem->getData();
        m_InfoBeginTime = m_DragScheduleInfo.beginDateTime;
        m_InfoEndTime = m_DragScheduleInfo.endDateTime;
        switch (getPosInItem(pos,infoitem->rect())) {
        case LEFT:
            m_DragStatus = ChangeBegin;
            setCursor(Qt::SplitHCursor);
            break;
        case RIGHT:
            m_DragStatus = ChangeEnd;
            setCursor(Qt::SplitHCursor);
            break;
        default:
            m_DragStatus = ChangeWhole;
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(m_DragScheduleInfo.titleName);
            mimeData->setData("Info",
                              CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->InfoToJson(m_DragScheduleInfo).toUtf8());

            if (m_Drag ==nullptr) {
                m_Drag = new QDrag(this);
            }
            m_currentitem = infoitem;
            m_Drag->setMimeData(mimeData);
            m_Drag->setPixmap(infoitem->getPixmap());
            QPointF itemPos = QPointF(pos.x()-infoitem->rect().x(),
                                      pos.y()-infoitem->rect().y());
            m_Drag->setHotSpot(itemPos.toPoint());
            break;
        }
    }
}

ScheduleDtailInfo CMonthGraphiview::getScheduleInfo(const QDate &beginDate, const QDate &endDate)
{
    ScheduleDtailInfo info;
    if (beginDate.daysTo(endDate)>0) {
        info.beginDateTime = QDateTime(beginDate,QTime(0,0,0));
        info.endDateTime = QDateTime(endDate,QTime(23,59,59));
    } else {
        info.beginDateTime = QDateTime(endDate,QTime(0,0,0));
        info.endDateTime = QDateTime(beginDate,QTime(23,59,00));
    }
    info.titleName = tr("New Event");
    info.allday = true;
    info.remind = true;
    info.id = 0;
    info.remindData.n = 1;
    info.remindData.time = QTime(9, 0);
    info.RecurID = 0;
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->GetType(
        1, info.type);
    info.rpeat = 0;

    return info;
}

CMonthGraphiview::PosInItem CMonthGraphiview::getPosInItem(const QPoint &p, const QRectF &itemRect)
{
    QPointF scenePos = this->mapToScene(p);
    QPointF itemPos = QPointF(scenePos.x()-itemRect.x(),
                              scenePos.y()-itemRect.y());
    qreal bottomy = itemRect.width()- itemPos.x();

    if (itemPos.x()<5) {
        return LEFT;
    }
    if (bottomy <5) {
        return RIGHT;
    }
    return MIDDLE;
}

QDate CMonthGraphiview::getPosDate(const QPoint &p)
{
    QRectF rect = this->sceneRect();
    qreal x =0;
    qreal y = 0;
    if (p.x()<0) {
        x =0;
    } else if (p.x()>rect.width()) {
        x = rect.width();
    } else {
        x = p.x();
    }
    if (p.y()<0) {
        y =0;
    } else if (p.y()>rect.height()) {
        y = rect.height();
    } else {
        y = p.y();
    }
    int xoffset = qFloor(x/(rect.width()/7)) %7 ;
    int yoffset = qFloor(y/(rect.height()/6))%6 ;
    return  m_DayItem[xoffset+yoffset*7]->getDate();
}

void CMonthGraphiview::upDateInfoShow(const CMonthGraphiview::DragStatus &status, const ScheduleDtailInfo &info)
{
    switch (status) {
    case NONE:
        Q_UNUSED(info);
        break;
    case ChangeBegin:
    case ChangeEnd: {
        m_MonthSchceduleView->changeDate(info);
    }
    break;
    case ChangeWhole: {
//        int index = vListData.indexOf(info);
//        vListData[index] = info;
    }
    break;
    case IsCreate:
        m_MonthSchceduleView->updateDate(info);
        break;
    }
}

void CMonthGraphiview::updateScheduleInfo(const ScheduleDtailInfo &info)
{
    if (info.rpeat >0) {
        CSchceduleDlg::ChangeRecurInfo(this,info,
                                       m_PressScheduleInfo,m_themetype);
    } else {
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(
            info);
    }
}

void CMonthGraphiview::DeleteItem(CMonthSchceduleWidgetItem *infoitem)
{
    emit signalViewtransparentFrame(1);
    ScheduleDtailInfo info = infoitem->getData();
    if (info.rpeat == 0) {
        CSchceduleCtrlDlg msgBox(this);
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete"), 1);
        msgBox.updatesize();
        DPalette pa = yesButton->palette();
        if (m_themetype == 0 || m_themetype == 1) {
            pa.setColor(DPalette::ButtonText, Qt::red);
        } else {
            pa.setColor(DPalette::ButtonText, "#FF5736");
        }
        yesButton->setPalette(pa);
        msgBox.exec();

        if (msgBox.clickButton() == noButton) {
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
        }
    } else {
        if (info.RecurID == 0) {
            CSchceduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (m_themetype == 0 || m_themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All Future Events"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (m_themetype == 0 || m_themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = info.beginDateTime.addDays(-1);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit slotSchceduleUpdate(infoitem->getData().id);
    emit signalViewtransparentFrame(0);
}

void CMonthGraphiview::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
//        if (event->source() ==this) {
        event->accept();
//        } else {
//            event->acceptProposedAction();
//        }
    } else {
        event->ignore();
    }
}

void CMonthGraphiview::dragLeaveEvent(QDragLeaveEvent *event)
{
    m_MoveDate = m_MoveDate.addMonths(2);
    emit slotSchceduleUpdate(0);
}

void CMonthGraphiview::dragMoveEvent(QDragMoveEvent *event)
{
    QString str = event->mimeData()->data("Info");
    QDate gDate =  getPosDate(event->pos());
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(str.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return;
    }

    if (m_MoveDate !=gDate) {
        m_MoveDate = gDate;
        QJsonObject rootobj = jsonDoc.object();
        m_DragScheduleInfo =
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->JsonObjectToInfo(rootobj);
        qint64 offset = m_PressDate.daysTo(m_MoveDate);
        m_DragScheduleInfo.beginDateTime = m_DragScheduleInfo.beginDateTime.addDays(offset);
        m_DragScheduleInfo.endDateTime    = m_DragScheduleInfo.endDateTime.addDays(offset);
        qreal y = 0;
        QRectF rect = this->sceneRect();
        if (event->pos().y()<0) {
            y =0;
        } else if (event->pos().y()>rect.height()) {
            y = rect.height();
        } else {
            y = event->pos().y();
        }
        int yoffset = qFloor(y/(rect.height()/6))%6;
        m_MonthSchceduleView->updateDate(yoffset,m_DragScheduleInfo);
    }
}

void CMonthGraphiview::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
        if (m_MoveDate !=m_PressDate) {
            updateScheduleInfo(m_DragScheduleInfo);
        }
        emit slotSchceduleUpdate(0);
        m_DragStatus = NONE;
    }
}

void CMonthGraphiview::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    QGraphicsItem *listItem =itemAt(event->pos());
    CMonthSchceduleNumButton *item = dynamic_cast<CMonthSchceduleNumButton *>(listItem);
    if (item!= nullptr) {
        //双击切换视图
        if (item->getDate().year()>1900) {
            emit signalsViewSelectDate(item->getDate());
        }
        return;
    }
    CMonthSchceduleWidgetItem *infoitem = dynamic_cast<CMonthSchceduleWidgetItem *>(listItem);
    if (infoitem != nullptr) {
        CMySchceduleView dlg(infoitem->getData(), this);
        connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CMonthGraphiview::slotSchceduleUpdate);
        connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CMonthGraphiview::slotdelete);
        connect(&dlg, &CMySchceduleView::signalViewtransparentFrame,
                this, &CMonthGraphiview::signalViewtransparentFrame);
        dlg.exec();
        return;
    }
    CDayGraphicsItem *Dayitem = dynamic_cast<CDayGraphicsItem *>(listItem);
    if (Dayitem !=nullptr) {
        QPointF point = getItemPos(event->pos(),Dayitem->rect());
        if (point.y()<38) {
            //双击切换视图
            if (Dayitem->getDate().year()>1900) {
                emit signalsViewSelectDate(Dayitem->getDate());
            }
        } else {
            //双击新建日程
            slotCreate(Dayitem->getDate());
        }
    }

}

void CMonthGraphiview::mousePressEvent(QMouseEvent *event)
{
    m_infoitem = nullptr;
    if (event->button() != Qt::LeftButton) {
        return;
    }
    QGraphicsItem *listItem =itemAt(event->localPos().toPoint());
    CMonthSchceduleWidgetItem *infoitem = dynamic_cast<CMonthSchceduleWidgetItem *>(listItem);
    if (infoitem != nullptr) {
        m_infoitem = infoitem;
        m_press = true;
        infoitem->setPressFlag(true);

        m_MonthSchceduleView->slotupdateItem(infoitem);
        emit signalScheduleShow(true, infoitem->getData());
    } else {
        emit signalScheduleShow(false);
    }
    DragPressEvent(event->pos(),listItem);
}

void CMonthGraphiview::mouseMoveEvent(QMouseEvent *event)
{
    DGraphicsView::mouseMoveEvent(event);
    if (m_press) {
        emit signalScheduleShow(false);
        m_press = false;
    }
    if (!this->sceneRect().contains(event->pos()))
        return;
    CMonthSchceduleWidgetItem *item = dynamic_cast<CMonthSchceduleWidgetItem *>(itemAt(event->pos()));
    if (item != nullptr) {
        m_infoitem = item;
        if (item->getData().type.ID != 4) {
            if (m_DragStatus == NONE) {
                switch (getPosInItem(event->pos(),item->rect())) {
                case LEFT:
                case RIGHT:
                    setCursor(Qt::SplitHCursor);
                    break;
                default:
                    setCursor(Qt::ArrowCursor);
                    break;
                }
            }
        }
    } else {
        if (m_DragStatus == NONE) {
            setCursor(Qt::ArrowCursor);
        }
    }
    m_infoitem = item;
    QDate gDate =  getPosDate(event->pos());
    switch (m_DragStatus) {
    case IsCreate:
        if (qAbs(event->pos().x()-m_PressPos.x())>20 ||qAbs(m_PressDate.daysTo(gDate))>0) {
            m_isCreate = true;
        }
        if (m_isCreate) {
            if (m_MoveDate !=gDate) {
                m_MoveDate = gDate;
                m_DragScheduleInfo = getScheduleInfo(m_PressDate,m_MoveDate);
                upDateInfoShow(IsCreate,m_DragScheduleInfo);
            }
        }
        break;
    case ChangeBegin:
        if (m_MoveDate !=gDate) {
            m_MoveDate = gDate;
            if (m_MoveDate.daysTo(m_InfoEndTime.date())<0) {
                m_DragScheduleInfo.beginDateTime = QDateTime(m_InfoEndTime.date().addDays(1),QTime(0,0,0));
                m_DragScheduleInfo.endDateTime = QDateTime(m_MoveDate,QTime(23,59,0));
            } else {
                m_DragScheduleInfo.beginDateTime = QDateTime(m_MoveDate,QTime(0,0,0));
                m_DragScheduleInfo.endDateTime = m_InfoEndTime;
            }
            upDateInfoShow(ChangeBegin,m_DragScheduleInfo);
        }
        break;
    case ChangeEnd:
        if (m_MoveDate !=gDate) {
            m_MoveDate = gDate;
            if (m_InfoBeginTime.date().daysTo(m_MoveDate)<0) {
                m_DragScheduleInfo.beginDateTime = QDateTime(m_MoveDate,QTime(0,0,0));
                m_DragScheduleInfo.endDateTime = QDateTime(m_InfoBeginTime.date().addDays(-1),QTime(23,59,0));
            } else {
                m_DragScheduleInfo.beginDateTime =m_InfoBeginTime;
                m_DragScheduleInfo.endDateTime = QDateTime(m_MoveDate,QTime(23,59,0));
            }
            upDateInfoShow(ChangeEnd,m_DragScheduleInfo);
        }
        break;
    case ChangeWhole: {
        if (!m_PressRect.contains(event->pos())) {
            m_Drag->exec( Qt::MoveAction);
            m_Drag = nullptr;
            m_DragStatus = NONE;
            setCursor(Qt::ArrowCursor);
        }
    }
    break;
    default:
        break;
    }
}

void CMonthGraphiview::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsItem *listItem =itemAt(event->pos());
    CMonthSchceduleWidgetItem *infoitem = dynamic_cast<CMonthSchceduleWidgetItem *>(listItem);
    if (infoitem != nullptr) {
        infoitem->setPressFlag(false);
    }

    DGraphicsView::mouseReleaseEvent(event);
    setCursor(Qt::ArrowCursor);
    m_press = false;
    switch (m_DragStatus) {
    case IsCreate:
        if (qAbs(m_MoveDate.daysTo(m_PressDate)<43)) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(
                m_DragScheduleInfo);
        }
        break;
    case ChangeBegin:
        if (m_MoveDate != m_InfoBeginTime.date()) {
            updateScheduleInfo(m_DragScheduleInfo);
        }
        break;
    case ChangeEnd:
        if (m_MoveDate != m_InfoEndTime.date()) {
            updateScheduleInfo(m_DragScheduleInfo);
        }
        break;
    default:
        break;
    }
    m_DragStatus = NONE;
    emit slotSchceduleUpdate(0);
}

void CMonthGraphiview::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsItem *listItem = itemAt(event->pos());
    CDayGraphicsItem *Dayitem = dynamic_cast<CDayGraphicsItem *>(listItem);
    if (Dayitem != nullptr) {
        DMenu Context(this);
        Context.addAction(m_createAction);
        m_createDate = Dayitem->getDate();
        Context.exec(QCursor::pos());
    }

    CMonthSchceduleWidgetItem *infoitem = dynamic_cast<CMonthSchceduleWidgetItem *>(listItem);
    if (infoitem !=nullptr) {
        if (infoitem->getData().type.ID !=4) {
            DMenu menu(this);
            menu.addAction(m_editAction);
            menu.addAction(m_deleteAction);
            QAction *action_t = menu.exec(QCursor::pos());
            if (action_t == m_editAction) {
                emit signalViewtransparentFrame(1);
                CSchceduleDlg dlg(0, this);
                dlg.setData(infoitem->getData());
                if (dlg.exec() == DDialog::Accepted) {
                    emit slotSchceduleUpdate(0);
                }
                emit signalViewtransparentFrame(0);
            } else if (action_t == m_deleteAction) {
                DeleteItem(infoitem);
            }
        }
    }
}

void CMonthGraphiview::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateSize();
    updateInfo();
}
void CMonthGraphiview::changeEvent(QEvent *event)
{
    if (event->type() ==QEvent::FontChange) {
        emit signalFontChange();
    }
}

void CMonthGraphiview::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
}

void CMonthGraphiview::slotCreate(const QDate &date)
{
    emit signalViewtransparentFrame(1);
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(date);
    if (date == QDate::currentDate()) {
        tDatatime.setTime(QTime::currentTime());
    } else {
        tDatatime.setTime(QTime(8, 0));
    }
    dlg.setDate(tDatatime);
    dlg.setAllDay(true);
    if (dlg.exec() == DDialog::Accepted) {
        emit slotSchceduleUpdate();
//        slotSchceduleUpdate();
        emit signalsSchceduleUpdate(0);
    }
    emit signalViewtransparentFrame(0);
}

void CMonthGraphiview::slotCreate()
{
    slotCreate(m_createDate);
}


void CMonthGraphiview::slotdelete(const int id)
{
    Q_UNUSED(id);
    emit signalsSchceduleUpdate(0);
}

void CMonthGraphiview::slotDeleteItem()
{
    if (m_infoitem == nullptr)
        return;
    if (m_infoitem->getData().type.ID ==4)
        return;
    DeleteItem(m_infoitem);
    m_infoitem =nullptr;
}

CDayGraphicsItem::CDayGraphicsItem(QGraphicsItem *parent)
    :QGraphicsRectItem(parent),
     m_Date(QDate::currentDate()),
     m_DayLunar(""),
     m_DayStatus(H_NONE)
{
    m_dayNumFont.setPixelSize(24);
    m_dayNumFont.setWeight(QFont::Light);

    m_LunerFont.setPixelSize(12);
    m_LunerFont.setWeight(QFont::Normal);
}

CDayGraphicsItem::~CDayGraphicsItem()
{

}

void CDayGraphicsItem::setData(const QDate &date)
{
    m_Date = date;
    setLunar("");
}

void CDayGraphicsItem::setLunar(const QString &lunar)
{
    m_DayLunar = lunar;
}

void CDayGraphicsItem::setStatus(const CDayGraphicsItem::HolidayStatus &status)
{
    m_DayStatus = status;
}

void CDayGraphicsItem::setTheMe(int type)
{
    m_themetype = type;
    if (type == 0 || type == 1) {
//        m_topBorderColor = Qt::red;
//        m_backgroundCircleColor = "#0081FF";
//        m_backgroundShowColor = "#2CA7F8";
//        m_backgroundShowColor.setAlphaF(0.4);

//        m_defaultTextColor = Qt::black;
//        m_currentDayTextColor = "#FFFFFF";
//        m_weekendsTextColor = Qt::black;
//        m_selectedTextColor = Qt::white;
//        m_festivalTextColor = Qt::black;
//        m_notCurrentTextColor = "#b2b2b2";


//        m_defaultLunarColor = "#5E5E5E";
//        m_defaultLunarColor.setAlphaF(0.5);
//        m_currentDayLunarColor = m_currentDayTextColor;
//        m_weekendsLunarColor = m_defaultLunarColor;
//        m_selectedLunarColor = Qt::white;
//        m_festivalLunarColor = m_defaultLunarColor;
//        m_notCurrentLunarColor = "#dfdfdf";
//        m_solofestivalLunarColor = "#FF7272";
//        m_solofestivalLunarColor.setAlphaF(0.3);
//        m_wrectColor = "#000000";
//        m_wrectColor.setAlphaF(0.05);


        m_dayNumColor = "#000000";
        m_dayNumCurrentColor = "#FFFFFF";

        m_LunerColor = "#5E5E5E";
        m_LunerColor.setAlphaF(0.5);

        m_fillColor = Qt::white;
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_BorderColor = "#000000";
        m_BorderColor.setAlphaF(0.05);

    } else if (type == 2) {

//        m_topBorderColor = Qt::red;
//        m_backgroundCircleColor = "#0059D2";
//        m_backgroundShowColor = "#002AAF";
//        m_backgroundShowColor.setAlphaF(0.4);

//        m_defaultTextColor = "#C0C6D4";
//        m_currentDayTextColor = "#C0C6D4";
//        m_weekendsTextColor = m_defaultTextColor;
//        m_selectedTextColor = "#B8D3FF";
//        m_festivalTextColor = "#C0C6D4";
//        m_notCurrentTextColor = "#C0C6D4";
//        m_notCurrentTextColor.setAlphaF(0.6);

//        m_defaultLunarColor = "#ABDAFF";
//        m_defaultLunarColor.setAlphaF(0.5);
//        m_currentDayLunarColor = m_currentDayTextColor;
//        m_weekendsLunarColor = m_defaultLunarColor;
//        m_selectedLunarColor = Qt::white;
//        m_festivalLunarColor = m_defaultLunarColor;
//        m_notCurrentLunarColor = "#ABDAFF";
//        m_notCurrentLunarColor.setAlphaF(0.3);
//        m_solofestivalLunarColor = "#4DFF7272";
//        QColor wcolor = "#282828";
//        //wcolor.setAlphaF(0.5);
//        m_wrectColor = wcolor;

        m_dayNumColor = "#C0C6D4";
        m_dayNumCurrentColor = "#B8D3FF";

        m_LunerColor = "#ABDAFF";
        m_LunerColor.setAlphaF(0.5);

        m_fillColor = "#000000";
        m_fillColor.setAlphaF(0.05);
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_BorderColor = "#000000";
        m_BorderColor.setAlphaF(0.05);
    }
    update();
}

void CDayGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    const int hh = 36;
    painter->setRenderHints(QPainter::Antialiasing);
    //绘制背景
    if (m_LunarVisible) {
        switch (m_DayStatus) {
        case H_WORK:
            painter->setBrush(QBrush(m_banColor));
            break;
        case H_REST:
            painter->setBrush(QBrush(m_xiuColor));
            break;
        default:
            painter->setBrush(QBrush(m_fillColor));
            break;
        }
    } else {
        painter->setBrush(QBrush(m_fillColor));
    }

    if (!m_IsCurrentMonth) {
        painter->setOpacity(0.4);
    }
    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_BorderColor);
    painter->setPen(pen);
    painter->drawRect(this->rect());

    painter->save();
    painter->restore();
    //绘制日期
    painter->setFont(m_dayNumFont);
    QRectF fillRect;
    if (m_LunarVisible) {
        fillRect.setRect(this->rect().x()+3,this->rect().y()+4, hh, hh);
    } else {
        fillRect.setRect(this->rect().x(),this->rect().y()+4, this->rect().width(), hh);
    }
    if (m_Date ==QDate::currentDate()) {
        QFont tfont = m_dayNumFont;
        tfont.setPixelSize(20);
        painter->setFont(tfont);
        painter->setPen(m_dayNumCurrentColor);
        QPixmap pixmap;
        if (m_themetype == 2)
            pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/darkchoose30X30_checked .svg");
        else {
            pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/choose30X30_checked .svg");
        }
//        pixmap.setDevicePixelRatio(devicePixelRatioF());
        if (m_LunarVisible)
            painter->drawPixmap(fillRect.toRect(),pixmap);
        else
            painter->drawPixmap(QPointF((this->rect().width() - pixmap.width()) / 2 + this->rect().x(), this->rect().y()), pixmap);
    } else {
        painter->setPen(m_dayNumColor);
    }
    fillRect.setY(fillRect.y()-10);
    fillRect.setX(fillRect.x()-1);
    painter->drawText(fillRect,
                      Qt::AlignCenter,
                      QString::number(m_Date.day()));

    painter->save();
    painter->restore();

    //绘制农历
    if (m_LunarVisible) {
        QFontMetrics metrics(m_LunerFont);
        int Lunarwidth = metrics.width(m_DayLunar);
        qreal filleRectX = this->rect().width()-12-3-(58 +Lunarwidth)/2;
        QRectF fillRect(this->rect().x()+filleRectX,
                        this->rect().y()+9,
                        12,
                        12);
        if (filleRectX>hh) {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setRenderHint(QPainter::HighQualityAntialiasing);
            painter->setRenderHint(QPainter::SmoothPixmapTransform);
            switch (m_DayStatus) {
            case H_WORK: {
                QPixmap  pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/ban.svg");
                painter->drawPixmap(fillRect.toRect(), pixmap);
            }

            break;
            case H_REST: {
                QPixmap pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/xiu.svg");
                painter->drawPixmap(fillRect.toRect(), pixmap);
            }
            break;
            default:
                break;
            }
        }
        painter->setFont(m_LunerFont);
        painter->setPen(m_LunerColor);
        painter->drawText(QRectF(this->rect().x()+this->rect().width() - 58,
                                 this->rect().y()+6, 58, 18), Qt::AlignCenter, m_DayLunar);

    }

}
