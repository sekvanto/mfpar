#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <QProcess>
#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include <cstdio>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <defs.h>
#include <historydata.h>
#include <historyentry.h>
#include "par/data.h"
#include "par/archiver.h"
#include "par/stats.h"

namespace Ui {
class Decompress;
}

class Decompress : public QDialog
{
    Q_OBJECT

public:
    explicit Decompress(QSettings *settings, HistoryData *history,
                        QWidget *parent = nullptr);
    ~Decompress();

    void stopDecompression();
    AlgorithmType detectCompressionAlgorithm(QString fileIn);
    void decompressHuffman(QString fileIn, QString fileOut);
    void decompressLZ4(QString fileIn, QString fileOut);
    void decompressPaq9a(QString fileIn, QString fileOut);

private slots:
    void on_chooseSourceButton_clicked();

    void on_chooseDestinationButton_clicked();

    void on_closeButton_clicked();

    void on_cancelButton_clicked();

    void on_startButton_clicked();

private:
    Ui::Decompress *ui;
    QSettings *settings;
    HistoryData *history;

    bool inProcessOfDecompression = false;
};

#endif // DECOMPRESS_H
