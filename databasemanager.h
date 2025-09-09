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
    QString createdAt;
};

class DatabaseManager
{
public:
    DatabaseManager(Database *db); // 依赖数据库模块
    int insertPlayer(const QString &name);//添加username
    void insertRecord(int playerId, int duration, int steps);//记录数据
    bool playerExists(const QString &name);
    QList<Record> getTopRecords(int limit = 10);

private:
    Database *m_database;
};

#endif // DATABASEMANAGER_H
