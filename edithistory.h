#ifndef EDITHISTORY_H
#define EDITHISTORY_H

#include <QDialog>
#include <historydata.h>
#include <historyentry.h>

namespace Ui {
class EditHistory;
}

class EditHistory : public QDialog
{
    Q_OBJECT

public:
    explicit EditHistory(HistoryData *history, QWidget *parent = nullptr);
    ~EditHistory();

private slots:
    void on_selectAllButton_clicked();

    void on_deselectAllButton_clicked();

    void on_deleteSelectedButton_clicked();

private:
    Ui::EditHistory *ui;
    HistoryData *history;
};

#endif // EDITHISTORY_H
