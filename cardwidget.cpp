#include "cardwidget.h"
#include "ui_cardwidget.h"

CardWidget::CardWidget(const CardData &data, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CardWidget)
    , m_data(data)
{
    ui->setupUi(this);

    QString title = m_data.label.trimmed();
    if (title.isEmpty())
        title = "Novo Cadastro";

    QLabel *titleLabel = findChild<QLabel *>("cardName");

    if (titleLabel) titleLabel->setText(title);

    this->setStyleSheet(R"(
        #frame {
            background-color: #f5f5f5;
            border-radius: 12px;
            border: 1px solid #d0d0d0;
        }
    )");

}

CardWidget::CardWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);

    QLabel *titleLabel = nullptr;
    titleLabel = findChild<QLabel *>("cardName");

    if (titleLabel) {
        titleLabel->setText(m_data.label);
    } else {
        this->setWindowTitle(m_data.label);
    }

    this->setStyleSheet(R"(
        #frame {
            background-color: #f5f5f5;
            border-radius: 12px;
            border: 1px solid #d0d0d0;
        }
    )");
}


CardWidget::~CardWidget()
{
    delete ui;
}

void CardWidget::mousePressEvent(QMouseEvent *event) {
    emit cardClicked(this);
    QWidget::mousePressEvent(event);
}

void CardWidget::setData(const CardData &d)
{
    m_data = d;

    QString title = m_data.label.trimmed();
    if (title.isEmpty())
        title = "Novo Cadastro";

    QLabel *titleLabel = findChild<QLabel *>("cardName");

    titleLabel->setText(title);

}
