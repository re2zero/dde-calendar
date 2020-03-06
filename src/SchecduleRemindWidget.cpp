#include "SchecduleRemindWidget.h"
#include <QPainter>
SchecduleRemindWidget *SchecduleRemindWidget::m_instance_ptr = nullptr;
SchecduleRemindWidget::SchecduleRemindWidget(QWidget *parent)
    : DArrowRectangle(DArrowRectangle::ArrowLeft, DArrowRectangle::FloatWindow, parent)
    , m_centerWidget(new DWidget(this))
{
    m_centerWidget->installEventFilter(this);
    m_centerWidget->setFixedWidth(207);
    m_centerWidget->setFixedHeight(57);
    setContent(m_centerWidget);
}

SchecduleRemindWidget::~SchecduleRemindWidget()
{

}

void SchecduleRemindWidget::setData(const ScheduleDtailInfo &vScheduleInfo, const CSchedulesColor &gcolor)
{
    m_ScheduleInfo = vScheduleInfo;
    gdcolor = gcolor;
    update();
}

void SchecduleRemindWidget::splitText(QFont &font, int w, int h, QString &str, QStringList &liststr)
{
    if (str.isEmpty())
        return;
    QFontMetrics fontmetris(font);
    int heightT = fontmetris.height();

#if 1
    QString tstr;
    QStringList tliststr;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fontmetris.width(tstr) + 5;
        if (widthT >= w) {
            tstr.chop(1);
            if (tstr.isEmpty())
                break;
            tliststr.append(tstr);
            tstr.clear();
            i--;
        }
    }
    tliststr.append(tstr);
    if (w < 30) {
        if (tliststr.isEmpty()) {
            liststr.append("...");
        } else {
            liststr.append(tliststr.at(0) + "...");
        }
    } else {
        for (int i = 0; i < tliststr.count(); i++) {
            if ((i + 1) * heightT <= h) {
                liststr.append(tliststr.at(i));
            } else {
                if (i == 0) {
                    break;
                    // liststr.append("...");
                } else {
                    tstr = liststr.at(i - 1);
                    tstr.chop(3);
                    liststr[i - 1] = tstr + "...";
                }
                break;
            }
        }
    }
#endif
}


bool SchecduleRemindWidget::eventFilter(QObject *object, QEvent *event)
{
    DArrowRectangle::eventFilter(object, event);
    DWidget *cell = qobject_cast<DWidget *>(object);
    if (cell && event->type() == QEvent::Paint) {
        int diam = 8;
        int x = 40 - 13;
        int y = 17 - 5;
        QFont timeFont;
        timeFont.setFamily("SourceHanSansSC-Medium");
        timeFont.setPixelSize(12);
        QPainter painter(cell);
        painter.setRenderHints(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(gdcolor.gradientToC));
        painter.drawEllipse(x, y, diam, diam);
        //draw time
        QPen pen;
        pen.setColor(QColor(65, 77, 104, 178));
        painter.setPen(pen);
        painter.setFont(timeFont);
        QString timestr;

        QLocale locale;
//        if (locale.language() == QLocale::Chinese) {
//            timestr = tr("上午");
//        }
        timestr = m_ScheduleInfo.beginDateTime.time().toString("ap hh:mm");
        painter.drawText(QRect( x + 13, 7, 60, 18), Qt::AlignLeft | Qt::AlignTop, timestr);
        int textRectWidth = 165;

        QFontMetrics metrics = painter.fontMetrics();
        int flags = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop ; // 自动换行
        QRect textBoundingRect = metrics.boundingRect(QRect(x, 30, textRectWidth, 0), flags, m_ScheduleInfo.titleName);

        pen.setColor(QColor(65, 77, 104));
        painter.setPen(pen);
        if (textBoundingRect.height() > 108) {
            textBoundingRect.setHeight(108);
            QStringList liststr;
            splitText(timeFont, textBoundingRect.width(), textBoundingRect.height(), m_ScheduleInfo.titleName,
                      liststr);
            for (int i = 0; i < liststr.count(); i++) {
                painter.drawText(
                    QRect(textBoundingRect.x()
                          , textBoundingRect.y() + i * 20,
                          textBoundingRect.width(), 20),
                    Qt::AlignLeft, liststr.at(i));
            }
        } else {
            painter.drawText(textBoundingRect
                             , flags
                             , m_ScheduleInfo.titleName);
        }
        m_centerWidget->setFixedHeight(textBoundingRect.height() + 30 + 17);

//        QString strElidedText = metrics.elidedText(m_ScheduleInfo.titleName, Qt::ElideRight, textRectWidth, Qt::TextShowMnemonic);
//        painter.drawText(textBoundingRect
//                         , flags
//                         , strElidedText);

        painter.end();
    }

}
