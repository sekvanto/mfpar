#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settings = new QSettings(QDir::currentPath() + "/config_file.ini", QSettings::IniFormat);
    readConfigFile();

    historySortOrder = HistorySortOrder::Latest;
    history = new HistoryData();
    history->loadEntriesFromDB();
    loadHistoryEntries();

    if (settings->value(LIGHT_THEME_DEFAULT).toBool() == false) {
        this->ui->actionUse_dark_theme->setChecked(true);
        setLightTheme(false);
    } else {
        setLightTheme(true);
    }

    connect(history, &HistoryData::updateHistory, this, &MainWindow::on_history_changed);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete history;
    delete ui;
}

void MainWindow::readConfigFile()
{
    if (!settings->contains(ALWAYS_MANUALLY_ALGS)) {
        settings->setValue(ALWAYS_MANUALLY_ALGS, false);
    }
    if (!settings->contains(LIGHT_THEME_DEFAULT)) {
        settings->setValue(LIGHT_THEME_DEFAULT, true);
    }
    if (!settings->contains(HISTORY_ENTRIES_NUM)) {
        settings->setValue(HISTORY_ENTRIES_NUM, 3);
    }
    if (!settings->contains(PREFER_DIR_SAVING)) {
        settings->setValue(PREFER_DIR_SAVING, "~/workspace/temp/");
    }
    if (!settings->contains(PREFER_ALG)) {
        settings->setValue(PREFER_ALG, static_cast<int>(AlgorithmType::ALG_HUFFMAN));
    }
    settings->sync();
}

void MainWindow::setLightTheme(bool light)
{
    lightTheme = light;

    QString fileName;
    if (light) {
        fileName = ":qdarkstyle/light/lightstyle.qss";
    } else {
        fileName = ":qdarkstyle/dark/darkstyle.qss";
    }

    QFile f(fileName);

    if (!f.exists())   {
        qDebug("Unable to set stylesheet, file not found\n");
    }
    else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    on_history_changed();
}

void MainWindow::loadHistoryEntries()
{
    switch (historySortOrder) {
    case HistorySortOrder::Latest:
        on_actionBy_latest_triggered();
        break;
    case HistorySortOrder::Oldest:
        on_actionBy_oldest_triggered();
        break;
    case HistorySortOrder::NameAZ:
        on_actionBy_name_A_Z_triggered();
        break;
    case HistorySortOrder::NameZA:
        on_actionBy_name_Z_A_triggered();
    }
}

void MainWindow::displayHistoryEntry(HistoryEntry entry)
{
    this->ui->textEdit->setFontWeight(2);
    if (entry.isCompressed()) {
        this->ui->textEdit->setTextColor(Qt::red);
        this->ui->textEdit->append("Compressed: ");
    }
    else {
        this->ui->textEdit->setTextColor(Qt::darkGreen);
        this->ui->textEdit->append("Decompressed: ");
    }
    this->ui->textEdit->setFontWeight(1);
    this->ui->textEdit->setTextColor(Qt::black);
    if (!lightTheme) {
        this->ui->textEdit->setTextColor(Qt::white);
    }
    this->ui->textEdit->append(entry.getSourceFileName() + '\n');
}

void MainWindow::on_historyButton_clicked()
{
    EditHistory editHistory(history);
    editHistory.exec();
}


void MainWindow::on_historyDetailsLabel_linkActivated(const QString &link)
{
    Q_UNUSED(link);
}


void MainWindow::on_actionEdit_history_triggered()
{
    EditHistory editHistory(history);
    editHistory.exec();
}


void MainWindow::on_pushButton_clicked()
{
    EditHistory editHistory(history);
    editHistory.exec();
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_actionPreferences_triggered()
{
    Preferences preferences(settings);
    preferences.exec();
    on_history_changed();
}


void MainWindow::on_benchmarkButton_clicked()
{
    Benchmark bench(settings, history);
    bench.exec();
}


void MainWindow::on_compressButton_clicked()
{
    Compress compress(settings, history);
    compress.exec();
}


void MainWindow::on_actionCompress_triggered()
{
    Compress compress(settings, history);
    compress.exec();
}


void MainWindow::on_decompressButton_clicked()
{
    Decompress decompress(settings, history);
    decompress.exec();
}


void MainWindow::on_actionDecompress_triggered()
{
    Decompress decompress(settings, history);
    decompress.exec();
}


void MainWindow::on_exitButton_clicked()
{
    this->close();
}


void MainWindow::on_expandButton_clicked()
{
    if (!historyExpanded) {
        historyEntriesShown = history->size();
        historyExpanded = true;
        loadHistoryEntries();
        ui->expandButton->setText("Collapse");
    } else {
        historyEntriesShown = settings->value(HISTORY_ENTRIES_NUM).toInt();
        historyExpanded = false;
        loadHistoryEntries();
        ui->expandButton->setText("Expand");
    }
}


void MainWindow::on_actionInfo_triggered()
{
    QMessageBox::information(
        this,
        "Data Compression Tool",
        "This tool helps you compress data quickly and easily. " +
        QString("To use it, there are several requirements: ") +
        "paq9a, par, lz4, mysql, mysql connector, linux environment. " +
        "This program will be ported to other platforms as well later."
    );
}

void MainWindow::on_history_changed()
{
    this->ui->textEdit->clear();
    loadHistoryEntries();
}

void MainWindow::on_actionClear_history_triggered()
{
    history->clear();
}


void MainWindow::on_actionBy_oldest_triggered()
{
    historySortOrder = HistorySortOrder::Oldest;
    this->ui->textEdit->clear();
    if (history->size() == 0) {
        return;
    }
    int entriesToDisplay = settings->value(HISTORY_ENTRIES_NUM).toInt();
    if (historyExpanded) {
        entriesToDisplay = history->size();
    }
    int n = qMin(entriesToDisplay, history->size());

    for (int i = 0; i < n; i++) {
        displayHistoryEntry(history->getAt(i));
    }
    historyEntriesShown = n;
}


void MainWindow::on_actionBy_latest_triggered()
{
    historySortOrder = HistorySortOrder::Latest;
    this->ui->textEdit->clear();
    if (history->size() == 0) {
        return;
    }
    int entriesToDisplay = settings->value(HISTORY_ENTRIES_NUM).toInt();
    if (historyExpanded) {
        entriesToDisplay = history->size();
    }
    int n = entriesToDisplay;

    for (int i = history->size()-1; i >= history->size()-n && i >= 0; i--) {
        displayHistoryEntry(history->getAt(i));
    }
    historyEntriesShown = n;
}


void MainWindow::on_actionBy_name_A_Z_triggered()
{
    historySortOrder = HistorySortOrder::NameAZ;
    this->ui->textEdit->clear();
    if (history->size() == 0) {
        return;
    }
    int entriesToDisplay = settings->value(HISTORY_ENTRIES_NUM).toInt();
    if (historyExpanded) {
        entriesToDisplay = history->size();
    }
    int n = qMin(entriesToDisplay, history->size());

    QList<HistoryEntry> list = history->getList();
    std::vector<HistoryEntry> vec;

    for (HistoryEntry &entry : list) {
        vec.push_back(entry);
    }
    std::sort(vec.begin(), vec.end());
    for (int i = 0; i < n; i++) {
        displayHistoryEntry(vec[i]);
    }
}


void MainWindow::on_actionBy_name_Z_A_triggered()
{
    historySortOrder = HistorySortOrder::NameZA;
    this->ui->textEdit->clear();
    if (history->size() == 0) {
        return;
    }
    int entriesToDisplay = settings->value(HISTORY_ENTRIES_NUM).toInt();
    if (historyExpanded) {
        entriesToDisplay = history->size();
    }
    int n = qMin(entriesToDisplay, history->size());

    QList<HistoryEntry> list = history->getList();
    std::vector<HistoryEntry> vec;

    for (HistoryEntry &entry : list) {
        vec.push_back(entry);
    }
    std::sort(vec.rbegin(), vec.rend());
    for (int i = 0; i < n; i++) {
        displayHistoryEntry(vec[i]);
    }
}


void MainWindow::on_actionUse_dark_theme_triggered()
{
    setLightTheme(!ui->actionUse_dark_theme->isChecked());
}

