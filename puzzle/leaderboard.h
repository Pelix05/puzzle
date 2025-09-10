#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "databasemanager.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>

class Leaderboard : public QWidget
{
    Q_OBJECT
public:
    Leaderboard(DatabaseManager *manager, QWidget *parent = nullptr);

    // 刷新排行榜显示
    void refresh();


private:
    DatabaseManager *m_dbManager;
    QVBoxLayout *m_layout;
    QScrollArea *m_scrollArea;

    void clearLayout();
    void addCategory(const QString &title, const QList<Record> &records, const QString &color);
};

#endif // LEADERBOARD_H
