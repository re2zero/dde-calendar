#ifndef EXPORTEDINTERFACE_H
#define EXPORTEDINTERFACE_H

#include <QObject>
#include <DExportedInterface>
#include "schedulestructs.h"
DCORE_USE_NAMESPACE
struct Exportpara {
    int   viewType; //视图名称:year,month,week,day对应年、月、周、日视图类型
    QDateTime  viewTime; //表示需要查看视图上某个时间
    QString  ADTitleName;
    QDateTime  ADStartTime;
    QDateTime  ADEndTime;
};
class ExportedInterface : public Dtk::Core::DUtil::DExportedInterface
{
public:
    explicit ExportedInterface(QObject *parent = nullptr);
    QVariant invoke(const QString &action, const QString &parameters) const;
private:
    bool analysispara( QString &parameters, ScheduleDtailInfo &info, Exportpara &para) const;
    void parsingScheduleRRule(QString str, ScheduleDtailInfo &info) const;
    void parsingScheduleRemind(QString str, ScheduleDtailInfo &info) const;
    QDateTime fromconvertData(QString str) const;
private:
    QObject                *m_object;
};

#endif // EXPORTEDINTERFACE_H
