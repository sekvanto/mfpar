#include "edithistory.h"
#include "ui_edithistory.h"

EditHistory::EditHistory(HistoryData *history, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditHistory),
    history(history)
{
    ui->setupUi(this);

    this->ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->ui->tableWidget->insertColumn(0);
    this->ui->tableWidget->insertColumn(1);
    this->ui->tableWidget->insertColumn(2);

    int tableWidth = this->ui->tableWidget->width();
    this->ui->tableWidget->setColumnWidth(0, 170);
    this->ui->tableWidget->setColumnWidth(1, 370);
    this->ui->tableWidget->setColumnWidth(2, tableWidth - 170 - 370 - 15);

    this->ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Date"));
    this->ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("File name"));
    this->ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Operation"));

    for (int i = 0; i < history->size(); i++) {
        HistoryEntry entry = history->getAt(i);
        this->ui->tableWidget->insertRow(i);
        this->ui->tableWidget->setItem(i, 0, new QTableWidgetItem(entry.getDate().toString()));
        this->ui->tableWidget->setItem(i, 1, new QTableWidgetItem(entry.getSourceFileName()));
        this->ui->tableWidget->setItem(i, 2, new QTableWidgetItem(entry.isCompressed() ? "Compressed" : "Decompressed"));
    }
}

EditHistory::~EditHistory()
{
    delete ui;
}

void EditHistory::on_selectAllButton_clicked()
{
    this->ui->tableWidget->selectAll();
}


void EditHistory::on_deselectAllButton_clicked()
{
    this->ui->tableWidget->clearSelection();
}


void EditHistory::on_deleteSelectedButton_clicked()
{
    QList<QTableWidgetItem*> selected = this->ui->tableWidget->selectedItems();
    int columnCount = this->ui->tableWidget->columnCount();
    for (int i = selected.size() - columnCount; i >= 0; i -= columnCount) {
        QDateTime date = QDateTime::fromString(selected[i]->text());
        history->deleteEntry(date);
        this->ui->tableWidget->removeRow(selected[i]->row());
    }
}

