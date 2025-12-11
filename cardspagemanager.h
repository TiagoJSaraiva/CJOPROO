#ifndef CARDSPAGEMANAGER_H
#define CARDSPAGEMANAGER_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include "cardwidget.h"

class CardsPageManager : public QObject
{
    Q_OBJECT
public:
    explicit CardsPageManager(QObject *parent = nullptr);

    void addCard(CardWidget *card);

    int currentPage() const;
    int totalPages() const;

    QVector<CardWidget*> cardsOnPage(int page) const;

    void nextPage();
    void previousPage();
    void setCurrentPage(int page);

    void saveToDisk();
    void loadFromDisk();

    void removeCard(CardWidget* card);

private:
    QVector<CardWidget*> allCards;
    int m_currentPage = 0;
    const int cardsPerPage = 12;

signals:
    void cardsUpdated();
};

#endif // CARDSPAGEMANAGER_H
