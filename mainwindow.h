#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "assembler.h"
#include "simulator.h"
#include "exceptions.h"
#include <QMainWindow>
#include <QTextEdit>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots para manejar los eventos de los botones y widgets
    void on_pushButtonOpenTxt_clicked();
    void on_pushButtonOpenBin_clicked();
    void on_pushButtonSaveTxt_clicked();
    void on_pushButtonCreateBin_clicked();
    void on_textEditTxt_textChanged();
    void on_textEditBin_textChanged();
    void on_radioButtonDebugMode_clicked(bool checked);
    void on_pushButtonSimulate_clicked();
    void on_pushButtonStep_clicked();
    void on_pushButtonReset_clicked();

    // Slots para actualizar los valores en la interfaz
    void updateLcdValue(QString value);
    void updateInstValue(QString opcode, QString operando, QString  accumulator, int programCounter);

private:
    // Métodos privados para la funcionalidad interna
    void connectScrollBarSignals();
    void connectScrollBar(QScrollBar *scrollBar1, QScrollBar *scrollBar2);
    void paintErrorLineRed(int line);
    void paintCurrentLineGreen(int line);
    void paintLine(QTextEdit *textEdit, int line, QColor color);
    void clearLineHighlight(QTextEdit *textEdit);
    void openFile(QString fileName, QByteArray& txtToExtract);
    void translateStringToReadableBin(QByteArray& txtToTranslate, QString& finalBin);

    // Otras variables miembro necesarias
    Ui::MainWindow *ui;
    Assembler *assembler;
    Simulator *simulator;
};

#endif // MAINWINDOW_H
