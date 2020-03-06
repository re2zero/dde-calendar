#ifndef SchecduleRemindWidget_H
#define SchecduleRemindWidget_H

#include <DWidget>
#include <DArrowRectangle>
#include "schedulestructs.h"
#include "scheduledatamanage.h"

DWIDGET_USE_NAMESPACE

class SchecduleRemindWidget : public DArrowRectangle
{
    Q_OBJECT
    Q_DISABLE_COPY(SchecduleRemindWidget)
public:
    static SchecduleRemindWidget *getSchecduleRemindWidget(){
        if( m_instance_ptr == nullptr){
            m_instance_ptr = new SchecduleRemindWidget(nullptr);
        }
        return  m_instance_ptr;
    }
    ~SchecduleRemindWidget() Q_DECL_OVERRIDE;
    void setData(const ScheduleDtailInfo  &vScheduleInfo, const CSchedulesColor &gcolor);
private:
    explicit SchecduleRemindWidget(QWidget *parent = nullptr);
    void splitText(QFont &font, int w, int h, QString &str, QStringList &liststr);
protected:
    bool eventFilter(QObject *object, QEvent *event) Q_DECL_OVERRIDE;
signals:

public slots:
private:
    DWidget *m_centerWidget;
    ScheduleDtailInfo     m_ScheduleInfo;
    CSchedulesColor gdcolor;
    static SchecduleRemindWidget *m_instance_ptr;
};



#endif // SchecduleRemindWidget_H
