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

void Leaderboard::refresh()
{
    clearLayout();
    // 三个等级排行榜
    QList<Record> easyRecords   = m_dbManager->getTopRecords(1);
    QList<Record> normalRecords = m_dbManager->getTopRecords(2);
    QList<Record> hardRecords   = m_dbManager->getTopRecords(3);

    addCategory("简单模式", easyRecords, "#C8E6C9");  // 绿色背景
    addCategory("中等模式", normalRecords, "#FFE0B2"); // 橙色背景
    addCategory("困难模式", hardRecords, "#FFCDD2");   // 红色背景

    m_layout->addStretch();
}

void Leaderboard::addCategory(const QString &title, const QList<Record> &records, const QString &color)
{
    // 分类标题
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 18px; margin: 10px;");
    m_layout->addWidget(titleLabel);

    int rank = 1;
    for (const Record &r : records) {
        QFrame *card = new QFrame;
        card->setFrameShape(QFrame::Box);
        card->setLineWidth(2);
        card->setStyleSheet(QString("background-color: %1; border-radius: 8px;").arg(color));
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
}

