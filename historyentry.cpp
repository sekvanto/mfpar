#include "historyentry.h"

HistoryEntry::HistoryEntry(QDateTime date, QString sourceFileName, bool compressed)
    : date(date),
      sourceFileName(sourceFileName),
      compressed(compressed)
{

}

QDateTime HistoryEntry::getDate()
{
    return date;
}

QString HistoryEntry::getSourceFileName()
{
    return sourceFileName;
}

bool HistoryEntry::isCompressed()
{
    return compressed;
}

bool HistoryEntry::operator < (const HistoryEntry& entry) const
{
    if (sourceFileName.compare(entry.sourceFileName, Qt::CaseInsensitive) < 0) {
        return true;
    } else {
        return false;
    }
}
