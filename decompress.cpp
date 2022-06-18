#include "decompress.h"
#include "ui_decompress.h"

Decompress::Decompress(QSettings *settings, HistoryData *history,
                       QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Decompress),
    settings(settings),
    history(history)
{
    ui->setupUi(this);
    this->ui->compressionAlgLabel->setText("");
}

Decompress::~Decompress()
{
    delete ui;
}

void Decompress::on_chooseSourceButton_clicked()
{
    this->ui->sourceFileNameLine->setText(QFileDialog::getOpenFileName(this, tr("Open Source File"), "~/"));
}

void Decompress::on_chooseDestinationButton_clicked()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                           settings->value(PREFER_DIR_SAVING).toString());
    this->ui->destinationFileNameLine->setText(saveFileName);
    QFile file(saveFileName);
    if (file.exists()) {
        file.remove();
    }
}

void Decompress::on_closeButton_clicked()
{
    this->close();
}

void Decompress::stopDecompression() {
    inProcessOfDecompression = false;
}

void Decompress::on_cancelButton_clicked()
{
    if (inProcessOfDecompression) {
        stopDecompression();
    }
    this->close();
}


void Decompress::on_startButton_clicked()
{
    this->inProcessOfDecompression = true;

    Data data;
    initData(&data);

    QString fileIn = this->ui->sourceFileNameLine->text();
    QString fileOut = this->ui->destinationFileNameLine->text();

    if (!QFile(fileIn).exists() || fileOut.isEmpty() || fileOut.isNull()) {
        this->ui->compressionAlgLabel->setText("Please select both files.");
        this->inProcessOfDecompression = false;
        return;
    }

    AlgorithmType algType = detectCompressionAlgorithm(fileIn);

    clock_t startTime = clock();

    switch (algType) {
    case AlgorithmType::ALG_HUFFMAN: {
        decompressHuffman(fileIn, fileOut);
        this->ui->compressionAlgLabel->setText("huffman");
        break;
    }
    case AlgorithmType::ALG_LZ4: {
        decompressLZ4(fileIn, fileOut);
        this->ui->compressionAlgLabel->setText("LZ4");
        break;
    }
    case AlgorithmType::ALG_PAQ9: {
        decompressPaq9a(fileIn, fileOut);
        this->ui->compressionAlgLabel->setText("Paq9a");
        break;
    }
    case AlgorithmType::ALG_UNRECOGNIZED: /* Fallthrough */
    default: {
        this->ui->compressionAlgLabel->setText("Not detected.");
        this->inProcessOfDecompression = false;
        return;
    }
    }

    clock_t duration = clock() - startTime;
    data.time = (double)duration / CLOCKS_PER_SEC;

    this->ui->progressBar->setValue(100);
    this->ui->elapsedTimeLabel->setText(QString::number(data.time) + " sec");

    history->addEntry(QDateTime::currentDateTime(), fileIn, false);

    this->inProcessOfDecompression = false;
}

void Decompress::decompressHuffman(QString fileIn, QString fileOut)
{
    QStringList list;
    list.append("-u");
    list.append(fileIn);
    list.append(fileOut);
    QProcess::execute("./archive", list);
}

void Decompress::decompressLZ4(QString fileIn, QString fileOut)
{
    QStringList list;
    list.append("-d");
    list.append(fileIn);
    list.append(fileOut);
    QProcess::execute("lz4", list);
}

void Decompress::decompressPaq9a(QString fileIn, QString fileOut)
{
    QStringList list;
    list.append("x");
    list.append(fileIn);
    list.append(fileOut);
    QProcess::execute("paq9a", list);
}

AlgorithmType Decompress::detectCompressionAlgorithm(QString fileIn)
{
    const std::string &s1 = fileIn.toStdString();
    char* fileInName = (char*) s1.c_str();

    FILE *file = fopen(fileInName, "rb");

    uint8_t first, second; // Read first two bytes, second is signature

    fread(&first, sizeof(uint8_t), 1, file);
    fread(&second, sizeof(uint8_t), 1, file);

    fclose(file);

    qDebug("%x", second);

    switch (second) {
    case SIGNATURE_HUFFMAN:
        return AlgorithmType::ALG_HUFFMAN;
    case SIGNATURE_LZ4:
        return AlgorithmType::ALG_LZ4;
    case SIGNATURE_PAQ9A:
        return AlgorithmType::ALG_PAQ9;
    }
    return AlgorithmType::ALG_UNRECOGNIZED;
}
