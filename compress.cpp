#include "compress.h"
#include "ui_compress.h"

Compress::Compress(QSettings *settings, HistoryData *history,
                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Compress),
    settings(settings),
    history(history)
{
    ui->setupUi(this);
    ui->warningLabel->setText("");

    if (settings->value(ALWAYS_MANUALLY_ALGS).toBool()) {
        this->ui->autoAlgorithmCheckBox->setDisabled(true);
    }
    this->ui->algorithmButton->setCurrentIndex(settings->value(PREFER_ALG).toInt());
}

Compress::~Compress()
{
    delete ui;
}

void Compress::on_closeButton_clicked()
{
    this->close();
}


void Compress::on_cancelButton_clicked()
{
    if (inProcessOfCompression) {
        stopCompression();
    }
    this->close();
}

void Compress::stopCompression() {
    inProcessOfCompression = false;
}

void Compress::on_chooseSourceButton_clicked()
{
    this->ui->sourceFileNameLine->setText(QFileDialog::getOpenFileName(this, tr("Open Source File"), "~/"));
}

void Compress::on_chooseDestinationButton_clicked()
{
    this->ui->destinationFileNameLine->setText(QFileDialog::getSaveFileName(this, tr("Save File"),
                                               settings->value(PREFER_DIR_SAVING).toString()));
}


void Compress::on_startButton_clicked()
{
    this->inProcessOfCompression = true;

    Data data;
    initData(&data);

    const std::string& fileIn = this->ui->sourceFileNameLine->text().toStdString();
    const std::string& fileOut = this->ui->destinationFileNameLine->text().toStdString();
    data.fileIn = (char*) fileIn.c_str();
    data.fileOut = (char*) fileOut.c_str();
    data.isArchiving = true;

    if (!QFile(data.fileIn).exists() || QString(data.fileOut).isEmpty() || QString(data.fileOut).isNull()) {
        this->inProcessOfCompression = false;
        ui->warningLabel->setText("Please select both files.");
        return;
    }
    ui->warningLabel->setText("");

    data.algorithmType = static_cast<AlgorithmType>(this->ui->algorithmButton->currentIndex());

    clock_t startTime = clock();

    switch (data.algorithmType) {
    case AlgorithmType::ALG_HUFFMAN: {
        compressHuffmanData(&data);
        break;
    }
    case AlgorithmType::ALG_LZ4: {
        compressLZ4(data.fileIn, data.fileOut);
        break;
    }
    case AlgorithmType::ALG_PAQ9: {
        compressPaq9a(data.fileIn, data.fileOut);
        break;
    }
    default: {
        compressHuffmanData(&data);
    }
    }

    clock_t duration = clock() - startTime;
    data.time = (double)duration / CLOCKS_PER_SEC;

    this->ui->progressBar->setValue(100);
    this->ui->elapsedTimeLabel->setText(QString::number(data.time) + " sec");

    history->addEntry(QDateTime::currentDateTime(), this->ui->sourceFileNameLine->text(), true);

    this->inProcessOfCompression = false;
}

void Compress::compressHuffman(QString fileInName, QString fileOutName)
{
    Data data;
    initData(&data);

    const std::string& fileIn = fileInName.toStdString();
    const std::string& fileOut = fileOutName.toStdString();
    data.fileIn = (char*) fileIn.c_str();
    data.fileOut = (char*) fileOut.c_str();
    data.isArchiving = true;

    if (!QFile(data.fileIn).exists() || QString(data.fileOut).isEmpty() || QString(data.fileOut).isNull()) {
        return;
    }
    data.algorithmType = ALG_HUFFMAN;
    compressHuffmanData(&data);
}

void Compress::compressHuffmanData(Data *data)
{
    archive(data);
}

void Compress::compressLZ4(QString fileIn, QString fileOut)
{
    QStringList list;
    list.append(fileIn);
    list.append(fileOut);
    QProcess::execute("lz4", list);
}

void Compress::compressPaq9a(QString fileIn, QString fileOut)
{
    QStringList list;
    list.append("a");
    list.append(fileOut);
    list.append("-c");
    list.append(fileIn);
    QProcess::execute("paq9a", list);
}
