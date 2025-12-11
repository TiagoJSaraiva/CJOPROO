#ifndef CARDDETAILSWINDOW_H
#define CARDDETAILSWINDOW_H

#include "cardwidget.h"
#include <QWidget>

namespace Ui {
class CardDetailsWindow;
}

class CardDetailsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CardDetailsWindow(CardWidget *card, QWidget *parent = nullptr);
    ~CardDetailsWindow();

signals:
    void cardEdited();
    // sinal que pede a remoção do CardWidget associado
    void requestDelete(CardWidget *card);

private slots:
    void saveData();
    void cancel();

private:
    Ui::CardDetailsWindow *ui;
    CardWidget *associatedCard;
};

#endif // CARDDETAILSWINDOW_H
