#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <QPushButton>

class CPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CPushButton(QWidget *parent = nullptr);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool pressed = false;
};

#endif // CPUSHBUTTON_H
