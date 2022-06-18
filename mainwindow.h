#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>
#include <QList>
#include <vector>
#include <edithistory.h>
#include <preferences.h>
#include <benchmark.h>
#include <compress.h>
#include <decompress.h>
#include <defs.h>
#include <historyentry.h>
#include <historydata.h>
#include <par/data.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_historyButton_clicked();

    void on_historyDetailsLabel_linkActivated(const QString &link);

    void on_actionEdit_history_triggered();

    void on_pushButton_clicked();

    void on_actionExit_triggered();

    void on_actionPreferences_triggered();

    void on_benchmarkButton_clicked();

    void on_compressButton_clicked();

    void on_actionCompress_triggered();

    void on_decompressButton_clicked();

    void on_actionDecompress_triggered();

    void on_exitButton_clicked();

    void on_expandButton_clicked();

    void on_actionInfo_triggered();

    void on_history_changed();

    void on_actionClear_history_triggered();

    void on_actionBy_oldest_triggered();

    void on_actionBy_latest_triggered();

    void on_actionBy_name_A_Z_triggered();

    void on_actionBy_name_Z_A_triggered();

    void on_actionUse_dark_theme_triggered();

private:
    Ui::MainWindow *ui;
    QSettings *settings;
    bool lightTheme = true;
    HistoryData *history;

    int historyEntriesShown;
    HistorySortOrder historySortOrder;
    bool historyExpanded = false;

    void readConfigFile();
    void setLightTheme(bool light);
    void loadHistoryEntries();
    void displayHistoryEntry(HistoryEntry entry);
};
#endif // MAINWINDOW_H
