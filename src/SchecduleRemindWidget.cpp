#include "SchecduleRemindWidget.h"
#include <QPainter>
#include <QtMath>
#include <DFontSizeManager>
#include <DGuiApplicationHelper>
const int fontsize = 12;//DFontSizeManager::T8;
SchecduleRemindWidget::SchecduleRemindWidget(QWidget *parent)
    : DArrowRectangle(DArrowRectangle::ArrowLeft, DArrowRectangle::FloatWindow, parent)
    , m_centerWidget(new CenterWidget(this))
{
    m_centerWidget->setFixedWidth(207);
    m_centerWidget->setFixedHeight(57);
//    m_centerWidget->setTheMe(DGuiApplicationHelper::instance()->themeType());
    setContent(m_centerWidget);
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged,
                     m_centerWidget,
                     &CenterWidget::setTheMe);
}

SchecduleRemindWidget::~SchecduleRemindWidget()
{

}

void SchecduleRemindWidget::setData(const ScheduleDtailInfo &vScheduleInfo, const CSchedulesColor &gcolor)
{
    m_centerWidget->setData(vScheduleInfo, gcolor);
    m_ScheduleInfo = vScheduleInfo;
    gdcolor = gcolor;
    update();
}

CenterWidget::CenterWidget(DWidget *parent)
    : DWidget (parent)
{
    setTheMe(0);
}

CenterWidget::~CenterWidget()
{

}

void CenterWidget::setData(const ScheduleDtailInfo &vScheduleInfo, const CSchedulesColor &gcolor)
{
    m_ScheduleInfo = vScheduleInfo;
    gdcolor = gcolor;
    update();
}

void CenterWidget::setTheMe(const int type)
{
    Q_UNUSED(type);
    DPalette palette;
    textColor = palette.color(DPalette::Normal, DPalette::Text);
    update();
}

void CenterWidget::paintEvent(QPaintEvent *e)
{
    int diam = 8;
    int x = 40 - 13;
    int y = 17 - 5;
    QFont timeFont;
    timeFont.setFamily("SourceHanSansSC-Medium");
    timeFont.setPixelSize(fontsize);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(gdcolor.gradientToC));
    painter.drawEllipse(x, y, diam, diam);
    //draw time
    QPen pen;
    pen.setColor(textColor);
    painter.setPen(pen);
    painter.setFont(timeFont);
    QString timestr;

    QLocale locale;
    timestr = m_ScheduleInfo.beginDateTime.time().toString("AP H:mm");
    painter.drawText(QRect( x + 13, 7, 60, 18), Qt::AlignLeft | Qt::AlignTop, timestr);
    int textRectWidth = 165;
    QFontMetrics metrics = painter.fontMetrics();
    const int  textwidth = metrics.width(m_ScheduleInfo.titleName);
    const int  textheight = metrics.height();
    const int  h_count = qCeil(textwidth / textRectWidth);

    QString text;
    QStringList testList;
    if (h_count < 1) {
        testList.append(m_ScheduleInfo.titleName);
    } else {
        const int text_Max_Height = 108;
        const int text_HeightMaxCount = qFloor(text_Max_Height / textheight);
        for (int i = 0; i < m_ScheduleInfo.titleName.count(); ++i) {
            text += m_ScheduleInfo.titleName.at(i);
            if (metrics.width(text) > textRectWidth) {
                text.remove(text.count() - 1, 1);
                testList.append(text);
                text = "";
                if (testList.count() == (text_HeightMaxCount - 1)) {
                    text = m_ScheduleInfo.titleName.right( m_ScheduleInfo.titleName.count() - i );
                    testList.append(metrics.elidedText(text, Qt::ElideRight, textRectWidth));
                    break;
                }
                --i;
            } else {
                if (i + 1 == m_ScheduleInfo.titleName.count()) {
                    testList.append(text);
                }
            }
        }
    }
    for (int i = 0; i < testList.count(); i++) {
        painter.drawText(
            QRect(x, 30 + i * textheight, textRectWidth, textheight),
            Qt::AlignLeft, testList.at(i));
    }
    this->setFixedHeight(testList.count() * textheight + 30 + 8);
    painter.end();
}
