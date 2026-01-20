#include "AcoController.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include "SelectionController.h"

AcoController::AcoController(SelectionController* selector, QWidget* parent)
    : QObject(parent), m_selector(selector), m_parentWidget(parent)
{
    // Create widgets
    m_runBtn = new QPushButton("Run ACO", parent);
    m_resetBtn = new QPushButton("Reset", parent);
    m_infoLabel = new QLabel(parent);
    // stop use until selection two node
    m_runBtn->setEnabled(false);
    m_infoLabel->setText("Selected: (none)");

    connect(m_runBtn, &QPushButton::clicked, this, &AcoController::onRunClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &AcoController::onResetClicked);
}

void AcoController::addToLayout(QHBoxLayout* layout)
{
    layout->addWidget(m_runBtn);
    layout->addWidget(m_resetBtn);
    layout->addWidget(m_infoLabel);
}

QString AcoController::selectionText() const
{
    if (!m_selector) return "Selected: (none)";
    if (!m_selector->hasSource())
        return "Selected: click source";
    if (!m_selector->hasDestination())
        return "Selected: " + m_selector->source() + " -> (click destination)";

    return "Selected: " + m_selector->source() + " -> " + m_selector->destination();
}

void AcoController::sync()
{
    if (!m_selector) return;
    m_runBtn->setEnabled(m_selector->ready());// Run only when ready
    m_infoLabel->setText(selectionText());// Update small label
}

void AcoController::onResetClicked()
{
    if (m_selector)
    {
        m_selector->clear();
    }
        sync();
    
}

void AcoController::onRunClicked()
{
    if (!m_selector) return;
    if (!m_selector->ready())
    {
        QMessageBox::information(m_parentWidget, "ACO", "Please select source and destination first.");
        sync();
        return;
    }

    emit runRequested(m_selector->source(), m_selector->destination());
    

    sync();
}
