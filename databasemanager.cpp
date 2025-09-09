#include "databasemanager.h"

DatabaseManager::DatabaseManager(Database *db)
    : m_database(db)
{
}

bool DatabaseManager::playerExists(const QString &name)
{
    QSqlQuery query;
    query.prepare("SELECT player_id FROM players WHERE name = :name");
    query.bindValue(":name", name);
    query.exec();
    return query.next();  // 如果有结果，说明用户名存在
}


int DatabaseManager::insertPlayer(const QString &name)
{

    if (playerExists(name)) {
        return -1; // 返回 -1 表示用户名已存在
    }
    QSqlQuery query(m_database->getDatabase());
    query.prepare("INSERT INTO players (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "Insert player failed:" << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

void DatabaseManager::insertRecord(int playerId, int duration, int steps)
{
    QSqlQuery query(m_database->getDatabase());
    query.prepare("INSERT INTO records (player_id, duration, steps) "
                  "VALUES (:player_id, :duration, :steps)");
    query.bindValue(":player_id", playerId);
    query.bindValue(":duration", duration);
    query.bindValue(":steps", steps);

    if (!query.exec()) {
        qDebug() << "Insert record failed:" << query.lastError().text();
    }
}

QList<Record> DatabaseManager::getTopRecords(int limit)
{
    QList<Record> result;
    QSqlQuery query(m_database->getDatabase());
    query.prepare("SELECT players.name, records.duration, records.steps, records.created_at "
                  "FROM records "
                  "JOIN players ON records.player_id = players.player_id "
                  "ORDER BY records.duration ASC "
                  "LIMIT :limit");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        qDebug() << "Query top records failed:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        Record r;
        r.playerName = query.value(0).toString();
        r.duration = query.value(1).toInt();
        r.steps = query.value(2).toInt();
        r.createdAt = query.value(3).toString();
        result.append(r);
    }
    return result;
}
