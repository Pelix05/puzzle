#include "leaderboard.h"
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QPalette>

Leaderboard::Leaderboard(DatabaseManager *manager, QWidget *parent)
    : QWidget(parent), m_dbManager(manager)
{
    //滚动显示
    m_scrollArea = new QScrollArea(this);
    QWidget *container = new QWidget;
    m_layout = new QVBoxLayout(container);
    container->setLayout(m_layout);
    m_scrollArea->setWidget(container);
    m_scrollArea->setWidgetResizable(true);

    //显示区域
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);
}

void Leaderboard::clearLayout()
{
    while (QLayoutItem* item = m_layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

void Leaderboard::refresh(int topN)
{
    clearLayout();
    QList<Record> topRecords = m_dbManager->getTopRecords(topN);

    int rank = 1;
    for (const Record &r : topRecords) {
        QFrame *card = new QFrame;
        card->setFrameShape(QFrame::Box);
        card->setLineWidth(2);
        card->setStyleSheet("background-color: #E0E0E0; border-radius: 8px;");
        card->setFixedHeight(60);

        QHBoxLayout *cardLayout = new QHBoxLayout(card);

        QLabel *rankLabel = new QLabel(QString::number(rank));
        QLabel *nameLabel = new QLabel(r.playerName);
        QLabel *durationLabel = new QLabel(QString("%1 秒").arg(r.duration));
        QLabel *stepsLabel = new QLabel(QString("%1 步").arg(r.steps));
        QLabel *timeLabel = new QLabel(r.createdAt);

        cardLayout->addWidget(rankLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(durationLabel);
        cardLayout->addWidget(stepsLabel);
        cardLayout->addWidget(timeLabel);

        m_layout->addWidget(card);
        rank++;
    }

    m_layout->addStretch();
}
