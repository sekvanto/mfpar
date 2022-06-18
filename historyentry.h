#ifndef HISTORYENTRY_H
#define HISTORYENTRY_H

#include <QDateTime>

class HistoryEntry
{
private:
    QDateTime date;
    QString sourceFileName;
    bool compressed;

public:
    HistoryEntry(QDateTime date, QString sourceFileName, bool compressed);

    QDateTime getDate();
    QString getSourceFileName();
    bool isCompressed();
    bool operator < (const HistoryEntry& entry) const;
};

#endif // HISTORYENTRY_H
