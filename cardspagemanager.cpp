#include "cardspagemanager.h"

CardsPageManager::CardsPageManager(QObject *parent) : QObject(parent) {}

void CardsPageManager::addCard(CardWidget *card)
{
    allCards.append(card);
    m_currentPage = totalPages() - 1;
    saveToDisk();
    emit cardsUpdated();
}

int CardsPageManager::currentPage() const
{
    return m_currentPage;
}

int CardsPageManager::totalPages() const
{
    if (allCards.isEmpty())
        return 1;

    return ((allCards.size() - 1) / cardsPerPage) + 1;
}

QVector<CardWidget*> CardsPageManager::cardsOnPage(int page) const
{
    QVector<CardWidget*> pageCards;

    int start = page * cardsPerPage;
    int end = qMin(start + cardsPerPage, allCards.size());

    for (int i = start; i < end; i++)
        pageCards.append(allCards[i]);

    return pageCards;
}

void CardsPageManager::nextPage()
{
    if (m_currentPage < totalPages() - 1)
        m_currentPage++;
    emit cardsUpdated();
}

void CardsPageManager::previousPage()
{
    if (m_currentPage > 0)
        m_currentPage--;
    emit cardsUpdated();
}

void CardsPageManager::setCurrentPage(int page)
{
    if (page >= 0 && page < totalPages())
        m_currentPage = page;
    emit cardsUpdated();
}

void CardsPageManager::saveToDisk()
{
    QJsonArray arr;

    for (CardWidget *c : allCards)
    {
        const CardData &d = c->data();

        QJsonObject obj;
        obj["label"] = d.label;
        obj["username"] = d.username;
        obj["email"] = d.email;
        obj["password"] = d.password;
        obj["url"] = d.url;
        obj["notes"] = d.notes;

        arr.append(obj);
    }

    QJsonObject root;
    root["cards"] = arr;

    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                   + "/cards.json";

    QDir().mkpath(QFileInfo(path).absolutePath());

    QFile f(path);
    if (f.open(QIODevice::WriteOnly))
        f.write(QJsonDocument(root).toJson());
}

void CardsPageManager::loadFromDisk()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
    + "/cards.json";

    QFile f(path);
    if (!f.exists() || !f.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
    QJsonArray arr = doc.object()["cards"].toArray();

    allCards.clear();

    for (const QJsonValue &v : arr)
    {
        QJsonObject obj = v.toObject();
        CardData d;

        d.label    = obj["label"].toString();
        d.username = obj["username"].toString();
        d.email    = obj["email"].toString();
        d.password = obj["password"].toString();
        d.url      = obj["url"].toString();
        d.notes    = obj["notes"].toString();

        CardWidget *w = new CardWidget(d);
        allCards.append(w);
    }
}

void CardsPageManager::removeCard(CardWidget* card)
{
    if (!card) return;

    allCards.removeOne(card);

    // destrói o widget
    card->deleteLater();

    // garante paginação válida
    if (m_currentPage >= totalPages())
        m_currentPage = totalPages() - 1;

    saveToDisk();
    emit cardsUpdated();
}
