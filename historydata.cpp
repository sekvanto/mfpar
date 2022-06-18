#include "historydata.h"

HistoryData::HistoryData(QObject *parent) : QObject(parent)
{
    try{
       driver=get_driver_instance();
       connection=driver->connect("localhost",
                         "root", "cz879hii");
       connection->setAutoCommit(0);
       connection->setSchema("historyentries");
    }catch(SQLException &ex){
       cout<<"Exception occurred "<<ex.what()<<endl;
    }
}

HistoryData::~HistoryData()
{
    connection->close();
    delete connection;
}

void HistoryData::addEntry(QDateTime date, QString fileIn, bool compress)
{
    HistoryEntry entry(date, fileIn, compress);
    entries.append(entry);

    PreparedStatement *pstmt=connection->prepareStatement
       ("INSERT INTO HistoryEntries ( date, fileIn, compressed) VALUES (?,?,?)");
    try{
       pstmt->setString(1,date.toString().toStdString());
       pstmt->setString(2,fileIn.toStdString());
       pstmt->setInt(3, compress ? 1 : 0);
       pstmt->executeUpdate();
       connection->commit();
    }catch(SQLException &ex){
       cout<<"Exception occurred "<<ex.what()<<endl;
    }
    pstmt->close();
    delete pstmt;
    emit updateHistory();
}

void HistoryData::loadEntriesFromDB()
{
    ResultSet *resultSet=NULL;
    PreparedStatement *pstmt=connection->prepareStatement
         ("SELECT * FROM HistoryEntries");
    try{
       resultSet=pstmt->executeQuery();
       while(resultSet->next()){
          QDateTime date;
          QString fileIn;
          bool compress;
          date = QDateTime::fromString(QString::fromStdString(resultSet->getString("date")));
          fileIn = QString::fromStdString(resultSet->getString("fileIn"));
          compress = resultSet->getInt("compressed");
          HistoryEntry entry(date, fileIn, compress);
          entries.append(entry);
       }
    }catch(SQLException &ex){
       cout<<"Exception occurred "<<ex.what()<<endl;
    }
    resultSet->close();
    pstmt->close();
    delete pstmt;
    delete resultSet;
}

void HistoryData::deleteEntry(QDateTime date)
{
    PreparedStatement *pstmt=connection->prepareStatement
       ("DELETE FROM HistoryEntries WHERE date=?");
    try{
       pstmt->setString(1,date.toString().toStdString());
       pstmt->executeUpdate();
       connection->commit();
       for (int i = 0; i < entries.size(); i++) {
           if (entries[i].getDate() == date) {
               entries.removeAt(i);
               break;
           }
       }
    }catch(SQLException &ex){
       cout<<"Exception occurred "<<ex.what()<<endl;
    }
    pstmt->close();
    delete pstmt;
    emit updateHistory();
}

void HistoryData::clear()
{
    PreparedStatement *pstmt=connection->prepareStatement
       ("DELETE FROM HistoryEntries");
    try{
       pstmt->executeUpdate();
       connection->commit();
       entries.clear();
    }catch(SQLException &ex){
       cout<<"Exception occurred "<<ex.what()<<endl;
    }
    pstmt->close();
    delete pstmt;
    emit updateHistory();
}

HistoryEntry HistoryData::getAt(int index)
{
    return entries[index];
}

int HistoryData::size()
{
    return entries.size();
}

QList<HistoryEntry> HistoryData::getList() {
    return entries;
}
