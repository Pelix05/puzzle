#include "leaderboard.h"
#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QPalette>
#include <QDebug>

Leaderboard::Leaderboard(DatabaseManager *manager, QWidget *parent)
    : QWidget(parent), m_dbManager(manager)
{

    // 设置初始窗口大小（可调整）
    setMinimumSize(800, 600);   // 最小窗口尺寸
    resize(1024, 768);          // 默认显示尺寸

    // 设置背景图
    QPalette palette;
    QPixmap bgPixmap(":/images/background.png"); // 替换为你自己的图片路径
    palette.setBrush(QPalette::Window, bgPixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setAutoFillBackground(true);
    setPalette(palette);


    //滚动显示
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setFrameShape(QFrame::NoFrame); // 去掉边框
    m_scrollArea->setStyleSheet("background: transparent;"); // 背景透明
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
    qDebug() << "[Leaderboard] 开始刷新排行榜...";
    // 三个等级排行榜
    QList<Record> easyRecords   = m_dbManager->getTopRecords(1);
    QList<Record> normalRecords = m_dbManager->getTopRecords(2);
    QList<Record> hardRecords   = m_dbManager->getTopRecords(3);

    qDebug() << "简单模式记录数:" << easyRecords.size();
    qDebug() << "中等模式记录数:" << normalRecords.size();
    qDebug() << "困难模式记录数:" << hardRecords.size();

    addCategory("简单模式", easyRecords, "#00CED1");  // 绿色背景
    addCategory("中等模式", normalRecords, "#1E90FF"); // 橙色背景
    addCategory("困难模式", hardRecords, "#8A2BE2");   // 红色背景

    m_layout->addStretch();
}

void Leaderboard::addCategory(const QString &title, const QList<Record> &records, const QString &color)
{
    // 分类标题
    qDebug() << "[Leaderboard] 添加分类:" << title << "记录数:" << records.size();

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "font-weight: bold;"
        "font-size: 18px;"
        "margin: 10px;"
        "color: #00FFFF;;" // 或其他适合背景图的颜色
        "text-shadow: 1px 1px 2px #00FFFF;" // 可选，增加阴影让字体更清晰
        );
    m_layout->addWidget(titleLabel);

    int rank = 1;
    for (const Record &r : records) {
        qDebug() << "排名:" << rank
                 << "玩家:" << r.playerName
                 << "时长:" << r.duration
                 << "步数:" << r.steps
                 << "时间:" << r.createdAt;
        QFrame *card = new QFrame;
        card->setFrameShape(QFrame::NoFrame);
        card->setLineWidth(1);
        // 根据 r.level 设置边框颜色
        QString borderColor;
        switch(r.level) {
        case 1: borderColor = "#00BFFF"; break; // 简单-蓝色
        case 2: borderColor = "#8A2BE2"; break; // 中等-紫色
        case 3: borderColor = "#FF4500"; break; // 困难-红色
        default: borderColor = "#FFFFFF"; break; // 默认白色
        }
        card->setStyleSheet(QString(
            "background-color: rgba(50, 50, 50, 180);" // 半透明深灰
            "border: 2px solid %1;"                    // 根据难度颜色
            "border-radius: 6px;"
            "padding:5px;"
            ).arg(borderColor));

        QHBoxLayout *cardLayout = new QHBoxLayout(card);
        cardLayout->setContentsMargins(10, 5, 10, 5); // 内边距
        cardLayout->setSpacing(15);

        QLabel *rankLabel = new QLabel(QString::number(rank));
        QLabel *nameLabel = new QLabel(r.playerName);
        QLabel *durationLabel = new QLabel(QString("%1 秒").arg(r.duration));
        QLabel *stepsLabel = new QLabel(QString("%1 步").arg(r.steps));
        QLabel *timeLabel = new QLabel(r.createdAt);

        // 设置文字颜色自适应
        QString textColor = "#FFFFFF"; // 黑色，可根据背景改
        rankLabel->setStyleSheet("color:" + textColor + "; font-weight: bold;");
        nameLabel->setStyleSheet("color:" + textColor + ";");
        durationLabel->setStyleSheet("color:" + textColor + ";");
        stepsLabel->setStyleSheet("color:" + textColor + ";");
        timeLabel->setStyleSheet("color:" + textColor + ";");

        // 设置大小策略
        rankLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred); // 自适应宽度
        durationLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        stepsLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        timeLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);


        cardLayout->addWidget(rankLabel);
        cardLayout->addWidget(nameLabel);
        cardLayout->addWidget(durationLabel);
        cardLayout->addWidget(stepsLabel);
        cardLayout->addWidget(timeLabel);

        // 最小高度保证内容可见
        card->setMinimumHeight(50);


        m_layout->addWidget(card);
        rank++;
    }
}

