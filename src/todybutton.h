#ifndef TODYBUTTON_H
#define TODYBUTTON_H

#include <DPushButton>
DWIDGET_USE_NAMESPACE
class CTodyButton : public DPushButton
{
    Q_OBJECT
public:
    CTodyButton(QWidget *parent = nullptr);
    void setBColor(QColor normalC, QColor hoverC, QColor pressc, QColor normalC1, QColor hoverC1, QColor pressc1);
    void setTColor(QColor normalC, QColor hoverC, QColor pressc);
    void setshadowColor(QColor sc);
protected:
    // void paintEvent ( QPaintEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    QColor                   m_bnormalColor = "#FFFFFF";
    QColor                   m_bhoverColor = "#FFFFFF";
    QColor                   m_bPressColor = "#FFFFFF";
    QColor                   m_dbnormalColor = "#FFFFFF";
    QColor                   m_dbhoverColor = "#FFFFFF";
    QColor                   m_dbPressColor = "#FFFFFF";
    QColor                   m_tnormalColor = "#000000";
    QColor                   m_thoverColor = "#000000";
    QColor                   m_tPressColor = "#000000";
    QColor                   m_shadowColor  = "#FFFFFF";
    QFont                    m_font;
    bool                     m_pressstate = false;
    bool                     m_hovertate = false;
};

#endif // TODYBUTTON_H
