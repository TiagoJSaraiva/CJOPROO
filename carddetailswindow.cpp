#include "carddetailswindow.h"
#include "ui_carddetailswindow.h"
#include <QDebug>

CardDetailsWindow::CardDetailsWindow(CardWidget *card, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::CardDetailsWindow),
    associatedCard(card)
{
    ui->setupUi(this);

    QString dlgTitle;
    if (associatedCard) {
        QString label = associatedCard->data().label.trimmed();
        if (label.isEmpty()) label = "Novo Cadastro";
        dlgTitle = QString("Editar — %1").arg(label);
    } else {
        dlgTitle = "Editar — Novo Cadastro";
    }
    setWindowTitle(dlgTitle);

    connect(ui->buttonSave, &QPushButton::clicked,
            this, &CardDetailsWindow::saveData);

    connect(ui->buttonCancel, &QPushButton::clicked,
            this, &CardDetailsWindow::cancel);

    // Delete: emitir o CardWidget* associado (não CardData)
    connect(ui->buttonDelete, &QPushButton::clicked, this, [this]() {
        if (associatedCard) {
            emit requestDelete(associatedCard);
        }
        this->close();
    });

    if (associatedCard) {
        const CardData &d = associatedCard->data();

        ui->editLabel->setText(d.label);
        ui->editUsername->setText(d.username);
        ui->editEmail->setText(d.email);
        ui->editPassword->setText(d.password);
        ui->editUrl->setText(d.url);

        // notas como QTextEdit: usar setPlainText
        ui->editNotes->setText(d.notes);
    }
}

CardDetailsWindow::~CardDetailsWindow()
{
    delete ui;
}

void CardDetailsWindow::saveData()
{
    if (!associatedCard)
        return;

    CardData newData;

    newData.label    = ui->editLabel->text();
    newData.username = ui->editUsername->text();
    newData.email    = ui->editEmail->text();
    newData.password = ui->editPassword->text();
    newData.url      = ui->editUrl->text();
    newData.notes    = ui->editNotes->text();

    associatedCard->setData(newData);

    emit cardEdited();
    close();
}

void CardDetailsWindow::cancel()
{
    close();
}
