#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cardspagemanager.h"
#include "cardwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CardsPageManager *cardsManager;

    void refreshPage();
    void refreshAll();

    int currentRow = 0;
    int currentCol = 0;
    const int maxCols = 6;

private slots:
    void on_addCardButton_clicked();
    void on_buttonNextPage_clicked();
    void on_buttonPrevPage_clicked();
    void deleteCard(CardWidget *card);
};

#endif // MAINWINDOW_H
