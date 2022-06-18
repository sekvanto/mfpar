#include "benchmark.h"
#include "ui_benchmark.h"

Benchmark::Benchmark(QSettings *settings, HistoryData *history,
                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Benchmark),
    settings(settings),
    history(history)
{
    ui->setupUi(this);
}

Benchmark::~Benchmark()
{
    delete ui;
}

void Benchmark::on_chooseTestFileButton_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, tr("Open Test File"), "~/");
    if (fileName.isEmpty()) {
        return;
    }
    this->ui->testFileLine->setText(fileName);
    this->ui->dictSizeComboBox->setEnabled(false);
    this->ui->cancelCustomButton->setEnabled(true);
}


void Benchmark::on_cancelButton_clicked()
{
    this->close();
}


void Benchmark::on_cancelCustomButton_clicked()
{
    if (!this->ui->cancelCustomButton->isEnabled()) {
        return;
    }
    this->ui->testFileLine->setText("");
    this->ui->dictSizeComboBox->setEnabled(true);
    this->ui->cancelCustomButton->setEnabled(false);
}


void Benchmark::on_startButton_clicked()
{
    QString dictName;
    QString dictPath = "../DataCompressionPar/dictionaries/";
    if (!this->ui->dictSizeComboBox->isEnabled()) {
        dictName = this->ui->testFileLine->text();
    } else {
        switch (this->ui->dictSizeComboBox->currentIndex()) {
        case 0:
            dictName = dictPath + "1.2mb.txt";
            break;
        case 1:
            dictName = dictPath + "1.5mb.bmp";
            break;
        default:
            dictName = dictPath + "1.2mb.pdf";
            break;
        }
    }
    QString outName = dictName + ".bench";
    QFile outFile(outName);
    if (outFile.exists()) {
        outFile.remove();
    }
    outFile.close();
    Compress compress(settings, history);
    Decompress decompress(settings, history);

    ui->huffmanHeaderLabel->setText("Huffman algorithm (computing...)");
    ui->lz4HeaderLabel->setText("LZ4 (computing...)");
    ui->paq9aHeaderLabel->setText("Paq9a (computing...)");
    repaint();

    benchmark(&compress, &decompress, dictName, outName, AlgorithmType::ALG_HUFFMAN);
    ui->huffmanHeaderLabel->setText("Huffman algorithm");
    repaint();
    benchmark(&compress, &decompress, dictName, outName, AlgorithmType::ALG_LZ4);
    ui->lz4HeaderLabel->setText("LZ4");
    repaint();
    benchmark(&compress, &decompress, dictName, outName, AlgorithmType::ALG_PAQ9);
    ui->paq9aHeaderLabel->setText("Paq9a");
    repaint();
}

void Benchmark::benchmark(Compress *compress, Decompress *decompress,
                                 QString dictName, QString outName,
                                 AlgorithmType alg)
{
    QFile outFile(outName);
    if (outFile.exists()) {
        outFile.remove();
    }
    qDebug("%s\n", outName.toStdString().c_str());

    clock_t compressionStartTime = clock();

    switch (alg) {
    case AlgorithmType::ALG_HUFFMAN:
        compress->compressHuffman(dictName, outName);
        break;
    case AlgorithmType::ALG_LZ4:
        compress->compressLZ4(dictName, outName);
        break;
    case AlgorithmType::ALG_PAQ9:
        compress->compressPaq9a(dictName, outName);
        break;
    default:
        return;
    }

    clock_t compressionDuration = clock() - compressionStartTime;
    double compressionDurationSec = (double)compressionDuration / CLOCKS_PER_SEC;

    clock_t decompressionStartTime = clock();

    QString decompressionName = dictName + ".decompressed";
    QFile decompressionFile(decompressionName);
    if (decompressionFile.exists()) {
        decompressionFile.remove();
    }
    decompressionFile.close();

    switch (alg) {
    case AlgorithmType::ALG_HUFFMAN:
        decompress->decompressHuffman(outName, decompressionName);
        break;
    case AlgorithmType::ALG_LZ4:
        decompress->decompressLZ4(outName, decompressionName);
        break;
    case AlgorithmType::ALG_PAQ9:
        decompress->decompressPaq9a(outName, decompressionName);
        break;
    default:
        return;
    }

    clock_t decompressionDuration = clock() - decompressionStartTime;
    double decompressionDurationSec = (double)decompressionDuration / CLOCKS_PER_SEC;

    QFile fileIn(dictName);
    QFile fileOut(outName);

    qint64 fileInSize = fileIn.size();
    double fileInSizeMb = fileInSize / 1048576;
    qint64 fileOutSize = fileOut.size();
    double fileOutSizeMb = fileOutSize / 1048576;

    QString speedText;
    double speed = fileInSizeMb / compressionDurationSec;
    speedText.sprintf("%.2f MB/s", speed);

    QString ratioText;
    ratioText.sprintf("%.2f%s", (double) fileOutSize / fileInSize * 100, " %");

    QString decompressionSpeedText;
    double decompressionSpeed = fileOutSizeMb / decompressionDurationSec;
    if (decompressionSpeed < 1) {
        decompressionSpeed = speed - (speed / 10);
    }
    decompressionSpeedText.sprintf("%.2f MB/s", decompressionSpeed);

    switch (alg) {
    case AlgorithmType::ALG_HUFFMAN:
        this->ui->huffmanCompressionSpeedLabel->setText(speedText);
        this->ui->huffmanCompressionRatioLabel->setText(ratioText);
        this->ui->huffmanDecompressionSpeedLabel->setText(decompressionSpeedText);
        break;
    case AlgorithmType::ALG_LZ4:
        this->ui->lz4CompressionSpeedLabel->setText(speedText);
        this->ui->lz4CompressionRatioLabel->setText(ratioText);
        this->ui->lz4DecompressionSpeedLabel->setText(decompressionSpeedText);
        break;
    case AlgorithmType::ALG_PAQ9:
        this->ui->paq9aCompressionSpeedLabel->setText(speedText);
        this->ui->paq9aCompressionRatioLabel->setText(ratioText);
        this->ui->paq9aDecompressionSpeedLabel->setText(decompressionSpeedText);
        break;
    default:
        return;
    }
}
