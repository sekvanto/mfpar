#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <compress.h>
#include <decompress.h>
#include <historydata.h>

namespace Ui {
class Benchmark;
}

class Benchmark : public QDialog
{
    Q_OBJECT

public:
    explicit Benchmark(QSettings *settings, HistoryData *history,
                       QWidget *parent = nullptr);
    ~Benchmark();

private slots:
    void on_chooseTestFileButton_clicked();

    void on_cancelButton_clicked();

    void on_cancelCustomButton_clicked();

    void on_startButton_clicked();

private:
    Ui::Benchmark *ui;
    QSettings *settings;
    HistoryData *history;

    void benchmark(Compress *compress, Decompress *decompress,
                   QString dictName, QString outName,
                   AlgorithmType alg);
};

#endif // BENCHMARK_H
