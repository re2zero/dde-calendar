#ifndef SchecduleRemindWidget_H
#define SchecduleRemindWidget_H

#include <DWidget>
#include <DArrowRectangle>
#include "schedulestructs.h"
#include "scheduledatamanage.h"

DWIDGET_USE_NAMESPACE

class CenterWidget;

class SchecduleRemindWidget : public DArrowRectangle
{
    Q_OBJECT
    Q_DISABLE_COPY(SchecduleRemindWidget)
public:
    static SchecduleRemindWidget *getSchecduleRemindWidget()
    {
        if ( m_instance_ptr == nullptr) {
            m_instance_ptr = new SchecduleRemindWidget(nullptr);
        }
        return  m_instance_ptr;
    }
    ~SchecduleRemindWidget() Q_DECL_OVERRIDE;
    void setData(const ScheduleDtailInfo  &vScheduleInfo, const CSchedulesColor &gcolor);
private:
    explicit SchecduleRemindWidget(QWidget *parent = nullptr);
signals:

public slots:
private:
    CenterWidget *m_centerWidget;
    ScheduleDtailInfo     m_ScheduleInfo;
    CSchedulesColor gdcolor;
    static SchecduleRemindWidget *m_instance_ptr;
};

class CenterWidget : public DWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(CenterWidget)
public:
    CenterWidget(DWidget *parent = nullptr);
    ~CenterWidget() Q_DECL_OVERRIDE;
    void setData(const ScheduleDtailInfo  &vScheduleInfo, const CSchedulesColor &gcolor);
protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
    ScheduleDtailInfo     m_ScheduleInfo;
    CSchedulesColor gdcolor;
};



#endif // SchecduleRemindWidget_H
