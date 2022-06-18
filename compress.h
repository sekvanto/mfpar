#ifndef COMPRESS_H
#define COMPRESS_H

#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QProcess>
#include <defs.h>
#include <historydata.h>
#include "par/common.h"
#include "par/data.h"
#include "par/archiver.h"
#include "par/stats.h"

namespace Ui {
class Compress;
}

class Compress : public QDialog
{
    Q_OBJECT

public:
    explicit Compress(QSettings *settings, HistoryData *history,
                      QWidget *parent = nullptr);
    ~Compress();

    void stopCompression();
    void compressHuffman(QString fileIn, QString fileOut);
    void compressHuffmanData(Data *data);
    void compressLZ4(QString fileIn, QString fileOut);
    void compressPaq9a(QString fileIn, QString fileOut);

private slots:
    void on_closeButton_clicked();

    void on_cancelButton_clicked();

    void on_chooseSourceButton_clicked();

    void on_chooseDestinationButton_clicked();

    void on_startButton_clicked();

private:
    Ui::Compress *ui;
    QSettings *settings;
    HistoryData *history;

    bool inProcessOfCompression = false;
};

#endif // COMPRESS_H
