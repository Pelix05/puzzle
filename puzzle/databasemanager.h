#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "database.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QList>
#include <QStringList>

struct Record {
    QString playerName;
    int duration;
    int steps;
    int level;
    QString createdAt;
};

class DatabaseManager
{
public:
    DatabaseManager(Database *db); // 依赖数据库模块
    bool playerExists(const QString &name);
    int insertPlayer(const QString &name);
    void insertRecord(int playerId, int duration, int steps, int level);
    QList<Record> getTopRecords(int level, int limit = 10);

private:
    Database *m_database;
};

#endif // DATABASEMANAGER_H
