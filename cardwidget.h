#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include "carddata.h"


namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(const CardData &data, QWidget *parent = nullptr);
    explicit CardWidget(QWidget *parent = nullptr);
    ~CardWidget();

    const CardData& data() const { return m_data; }

    void setData(const CardData &d);
    bool signalsConnected = false;

private:
    Ui::CardWidget *ui;
    CardData m_data;

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();
    void cardClicked(CardWidget *self);
};

#endif // CARDWIDGET_H
