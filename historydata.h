#ifndef HISTORYDATA_H
#define HISTORYDATA_H

#include <QList>
#include <QObject>
#include <jdbc/cppconn/connection.h>
#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <historyentry.h>

using namespace std;
using namespace sql;

class HistoryData : public QObject
{
    Q_OBJECT

public:
    explicit HistoryData(QObject *parent = nullptr);
    ~HistoryData();

    void initList();
    void addEntry(QDateTime date, QString fileIn, bool compress);
    void loadEntriesFromDB();
    void deleteEntry(QDateTime date);
    void clear();
    HistoryEntry getAt(int index);
    QList <HistoryEntry> getList();
    int size();

signals:
    void updateHistory();

private:
    QList<HistoryEntry> entries;
    Driver *driver;
    Connection *connection;
};

#endif // HISTORYDATA_H
