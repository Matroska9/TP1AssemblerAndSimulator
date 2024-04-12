#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , assembler(new Assembler())
    , simulator(new Simulator())
{
    ui->setupUi(this);

    // Conectar señales
    connectScrollBarSignals();

    connect(simulator, &Simulator::lcdSignalPassed, this, &MainWindow::updateLcdValue);

    connect(simulator, &Simulator::instSignalPassed, this, &MainWindow::updateInstValue);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete assembler;
    delete simulator;
}

// ************************************
//              UI METHODS
// ************************************

// ****** Métodos de los botones ******
// Al presionar el botón 'Abrir archivo (.txt)'
void MainWindow::on_pushButtonOpenTxt_clicked()
{
    QString txtFileName = QFileDialog::getOpenFileName(this,"Abrir archivo de instrucciones", "C://Users//Administrator//Desktop", "*.txt");
    QByteArray txtToExtract;
    try {
        openFile(txtFileName, txtToExtract);
        ui->lineDirectionTxt->setText(txtFileName);
        ui->textEditTxt->setText(txtToExtract);
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    }
}

// Al presionar el botón 'Abrir archivo (.bin)'
void MainWindow::on_pushButtonOpenBin_clicked()
{
    on_pushButtonReset_clicked();
    QString binFileName = QFileDialog::getOpenFileName(this,"Abrir archivo de instrucciones", "C://Users//Administrator//Desktop", "*.bin");
    QByteArray extractedBin;
    QString finalBin;
    try {
        openFile(binFileName, extractedBin);
        ui->lineDirectionBin->setText(binFileName);
        translateStringToReadableBin(extractedBin, finalBin);
        ui->textEditBin->setText(finalBin);

        simulator->loadInstructionByteArray(extractedBin);
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    }
}

// Al presionar el botón 'Crear binario (.bin)'
void MainWindow::on_pushButtonCreateBin_clicked()
{
    on_pushButtonReset_clicked();
    clearLineHighlight(ui->textEditTxt);
    try {
        QString binFileName = assembler->generateBin(ui->textEditTxt->toPlainText(), ui->lineDirectionTxt->text());
        ui->lineDirectionBin->setText(binFileName);

        QByteArray extractedBin;
        QString finalBin;
        openFile(binFileName, extractedBin);
        translateStringToReadableBin(extractedBin, finalBin);
        ui->textEditBin->setText(finalBin);

        simulator->loadInstructionByteArray(extractedBin);
    }
    catch (const FileException& e)
    {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    }
    catch (const InvalidInstructionExceptionWithLine& e)
    {
        paintErrorLineRed(e.getLineNumber());
        QString error(e.what());
        QMessageBox::critical(this, "Error en una instrucción", QString::fromStdString(error.toStdString()));
    }
}

// Al presionar el botón 'Save (.txt)'
void MainWindow::on_pushButtonSaveTxt_clicked()
{
    QString txtFileName = ui->lineDirectionTxt->text();
    QFile txtFile(txtFileName);
    try {
        if(!txtFile.open(QIODevice::WriteOnly))
        {
            throw FileException(txtFile.errorString().toStdString());
        }
        QByteArray aux(ui->textEditTxt->toPlainText().toUtf8());
        txtFile.write(aux);
        txtFile.close();
    } catch (const FileException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString(e.what()));
    }
}

// Al presionar el botón radial 'DebugMode'
void MainWindow::on_radioButtonDebugMode_clicked(bool checked)
{
    if(checked) {
        ui->pushButtonStep->setEnabled(true);
        ui->pushButtonReset->setEnabled(true);
        ui->lineEditOpcode->setEnabled(true);
        ui->lineEditOperando->setEnabled(true);
    }
    else
    {
        ui->pushButtonStep->setEnabled(false);
        ui->pushButtonReset->setEnabled(false);
        ui->lineEditOpcode->setEnabled(false);
        ui->lineEditOperando->setEnabled(false);
    }
}

void MainWindow::on_pushButtonSimulate_clicked()
{
    try {
        simulator->runSimulation(false);
    } catch (const CustomException& e) {
        QMessageBox::critical(this, "Error", QString::fromStdString("El programa tiene un ciclo infinito"));
    }
}

void MainWindow::on_pushButtonStep_clicked()
{
    simulator->runSimulation(true);
}

void MainWindow::on_pushButtonReset_clicked()
{
    simulator->restartProcessor();
    ui->lineEditOpcode->setText("");
    ui->lineEditOperando->setText("");
    ui->lineEditLCD->setText("");
    ui->lineEditAC->setText("");
    clearLineHighlight(ui->textEditBin);
}

// ****** Métodos de edición de texto ******
// Al editar el cuadro de texto de la pestaña 'tabassembler'
void MainWindow::on_textEditTxt_textChanged()
{
    int lineCount = ui->textEditTxt->document()->blockCount();

    QString lineNumbers;
    for (int i = 1; i <= lineCount; ++i) {
        lineNumbers.append(QString::number(i) + "\n");
    }

    ui->textEditLineNumber->setText(lineNumbers);
}

// Al editar el cuadro de texto de la pestaña 'tabSimulator'
void MainWindow::on_textEditBin_textChanged()
{
    int lineCount = ui->textEditBin->document()->blockCount();

    QString lineNumbers;
    for (int i = 1; i <= lineCount; ++i) {
        lineNumbers.append(QString::number(i) + "\n");
    }

    ui->textEditLineNumber_bin->setText(lineNumbers);
}

// ****** Métodos de señales internas ******
void MainWindow::updateLcdValue(QString value)
{
    ui->lineEditLCD->setText(value);
}

void MainWindow::updateInstValue(QString opcode, QString operando, QString  accumulator, int programCounter)
{
    ui->lineEditOpcode->setText(opcode);
    ui->lineEditOperando->setText(operando);
    ui->lineEditAC->setText(accumulator);
    clearLineHighlight(ui->textEditBin);
    paintCurrentLineGreen(programCounter);

    // Ajustar el valor del scrollbar para mostrar la línea resaltada
    QTextCursor cursor = ui->textEditBin->textCursor();
    cursor.movePosition(QTextCursor::Start);
    for (int i = 0; i < programCounter; ++i) {
        cursor.movePosition(QTextCursor::NextBlock);
    }
    ui->textEditBin->setTextCursor(cursor);}


// *****************************************
//              PRIVATE METHODS
// *****************************************

// Conecta las señales de los scrollbar
void MainWindow::connectScrollBarSignals()
{
    connectScrollBar(ui->textEditTxt->verticalScrollBar(), ui->textEditLineNumber->verticalScrollBar());
    connectScrollBar(ui->textEditLineNumber->verticalScrollBar(), ui->textEditTxt->verticalScrollBar());

    connectScrollBar(ui->textEditBin->verticalScrollBar(), ui->textEditLineNumber_bin->verticalScrollBar());
    connectScrollBar(ui->textEditLineNumber_bin->verticalScrollBar(), ui->textEditBin->verticalScrollBar());
}

// Conecta la señal de un scrollbar con otro
void MainWindow::connectScrollBar(QScrollBar *scrollBar1, QScrollBar *scrollBar2)
{
    connect(scrollBar1, &QScrollBar::valueChanged, [=](int value) {
        scrollBar2->setValue(value);
    });
}

// Abre el archivo con el nombre pasado por parámetro y extrae el contenido en un ByteArray
void MainWindow::openFile(QString fileName, QByteArray& txtToExtract)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        throw FileException(file.errorString().toStdString());
    txtToExtract = file.readAll();
    file.close();
}

// Convierte el QByteArray sin formatear en un texto legible en Hex
void MainWindow::translateStringToReadableBin(QByteArray& txtToTranslate, QString& finalBin)
{
    finalBin.clear();

    // Iterar sobre cada carácter en la cadena de texto
    for (int i = 0; i < txtToTranslate.length(); i++) {
        if (i % 4 == 0) {
            finalBin.append("0x");
        }

        // Convertir el carácter a su representación hexadecimal
        QChar currentChar = txtToTranslate.at(i);

        QString hexString = QString("%1").arg(currentChar.unicode(), 2, 16, QLatin1Char('0'));

        // Añadir el byte hexadecimal a la cadena final
        finalBin.append(hexString);
        finalBin.append(' ');

        // Agregar salto de línea después de cada grupo de 4 bytes
        if (i % 4 == 3) {
            finalBin.append("\n");
        }
    }
}

// ***** Métodos de coloreado *****
// Pinta la línea indexada por el parámetro del cuadro de texto de la pestaña 'tabAssembler'
void MainWindow::paintErrorLineRed(int line)
{
    QColor color(255, 0, 0, 100);
    paintLine(ui->textEditTxt, line, color);
}

void MainWindow::paintCurrentLineGreen(int line)
{
    QColor color(110, 220, 0, 100);
    paintLine(ui->textEditBin, line, color);
}

void MainWindow::paintLine(QTextEdit *textEdit, int line, QColor color)
{
    // Obtener el documento de texto
    QTextDocument* document = textEdit->document();
    // Obtener el bloque de texto correspondiente a la línea deseada
    QTextBlock block = document->findBlockByLineNumber(line - 1);

    // Si el bloque es válido, aplicar el formato de texto rojo
    if (block.isValid()) {
        QTextCursor cursor(block);
        QTextBlockFormat format = cursor.blockFormat();
        format.setBackground(color);
        cursor.setBlockFormat(format);
    }
}

// Limpia todas las líneas pintadas
void MainWindow::clearLineHighlight(QTextEdit *textEdit)
{
    QColor color(Qt::white); // Color blanco
    QTextDocument* document = textEdit->document();

    // Iterar sobre todos los bloques de texto y aplicarles el formato de color de fondo blanco
    for (QTextBlock block = document->begin(); block.isValid(); block = block.next()) {
        QTextCursor cursor(block);
        QTextBlockFormat format = cursor.blockFormat();
        format.setBackground(color);
        cursor.setBlockFormat(format);
    }
}



