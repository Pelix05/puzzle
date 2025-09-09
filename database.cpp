#include "database.h"
#include <QtSql/QSqlQuery>

Database::Database()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("puzzle.db"); // 数据库文件
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::initDatabase()
{
    if (!m_db.open()) {
        qDebug() << "Database open error:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query;
    // 玩家表
    if (!query.exec("CREATE TABLE IF NOT EXISTS players ("
                    "player_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL UNIQUE)")) {
        qDebug() << "Create players table failed:" << query.lastError().text();
        return false;
    }

    // 成绩表
    if (!query.exec("CREATE TABLE IF NOT EXISTS records ("
                    "record_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "player_id INTEGER NOT NULL,"
                    "duration INTEGER NOT NULL,"
                    "steps INTEGER,"
                    "created_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
                    "FOREIGN KEY(player_id) REFERENCES players(player_id))")) {
        qDebug() << "Create records table failed:" << query.lastError().text();
        return false;
    }

    return true;
}
