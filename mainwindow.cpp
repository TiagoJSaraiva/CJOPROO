#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cardwidget.h"
#include "carddetailswindow.h"
#include "cardspagemanager.h"

static void clearLayoutButKeepWidgets(QLayout *layout) {
    if (!layout) return;
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        QWidget *w = item->widget();
        if (w) {
            w->hide();
        }
        delete item;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(R"(
        #cardsContainerFrame {
            background-color: #f5f5f5;
            border-radius: 12px;
            border: 1px solid #d0d0d0;
        }
    )");

    ui->cardsContainer->setHorizontalSpacing(0);
    ui->cardsContainer->setVerticalSpacing(0);
    ui->cardsContainer->setContentsMargins(5, 5, 5, 0);

    cardsManager = new CardsPageManager(this);

    connect(cardsManager, &CardsPageManager::cardsUpdated,
            this, &MainWindow::refreshPage);

    cardsManager->loadFromDisk();
    refreshPage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addCardButton_clicked()
{
    CardData initialData;
    initialData.label = "";

    CardWidget *card = new CardWidget(initialData);
    card->setData(initialData);

    cardsManager->addCard(card);
    cardsManager->setCurrentPage(cardsManager->totalPages() - 1);

    refreshAll();
}

void MainWindow::refreshPage()
{
    clearLayoutButKeepWidgets(ui->cardsContainer);

    QVector<CardWidget*> cards =
        cardsManager->cardsOnPage(cardsManager->currentPage());

    const int maxCols = 6;
    int row = 0, col = 0;

    for (CardWidget *c : cards)
    {
        ui->cardsContainer->addWidget(c, row, col);
        c->show();

        if (!c->signalsConnected) {

            connect(c, &CardWidget::cardClicked, this,
                    [this](CardWidget *clickedCard)
                    {
                        CardDetailsWindow *dlg = new CardDetailsWindow(clickedCard, nullptr);
                        dlg->setWindowFlag(Qt::Window);  // garante que abre como janela separada
                        dlg->setAttribute(Qt::WA_DeleteOnClose);

                        // Card editado
                        connect(dlg, &CardDetailsWindow::cardEdited, this, [=]() {
                            cardsManager->saveToDisk();
                            refreshPage();
                        });

                        // Card deletado
                        connect(dlg, &CardDetailsWindow::requestDelete,
                                this, &MainWindow::deleteCard);

                        dlg->show();
                    });

            c->signalsConnected = true;
        }

        col++;
        if (col >= maxCols) {
            col = 0;
            row++;
        }
    }

    ui->labelPageInfo->setText(
        QString("%1 / %2")
            .arg(cardsManager->currentPage() + 1)
            .arg(cardsManager->totalPages()));
}

void MainWindow::on_buttonNextPage_clicked()
{
    cardsManager->nextPage();
}

void MainWindow::on_buttonPrevPage_clicked()
{
    cardsManager->previousPage();
}

void MainWindow::deleteCard(CardWidget *card)
{
    if (!card) return;

    // delega a remoção ao manager (garanta que CardsPageManager::removeCard exista)
    cardsManager->removeCard(card);

    // atualiza a interface
    refreshAll();
}

void MainWindow::refreshAll()
{
    if (cardsManager) {
        cardsManager->saveToDisk();
    }

    if (cardsManager) {
        int tp = cardsManager->totalPages();
        if (tp <= 0) tp = 1;
        if (cardsManager->currentPage() >= tp) {
            cardsManager->setCurrentPage(qMax(0, tp - 1));
        }
    }

    refreshPage();
}
