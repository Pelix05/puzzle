#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

class Database
{
public:
    Database();
    ~Database();

    // 初始化数据库文件和基本表结构
    bool initDatabase();

    // 获取数据库对象
    QSqlDatabase getDatabase() const { return m_db; }

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
