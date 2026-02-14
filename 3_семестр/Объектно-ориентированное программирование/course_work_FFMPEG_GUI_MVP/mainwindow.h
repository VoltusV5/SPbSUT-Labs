#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QCursor>

class PresetManager; // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectInputFile();
    void updateCustomResolutionVisibility();
    void updateCommandFromGUI();


    void runEncoding();
    void savePreset();
    void loadPreset();
    void deletePreset();
    void readProcessOutput();
    void processFinished(int exitCode);

private:
    // === UI ===
    QLineEdit *inputFileEdit;
    QComboBox *codecCombo;
    QComboBox *containerCombo;
    QComboBox *resolutionCombo;
    QLineEdit *customResolutionEdit;
    QTextEdit *commandDisplay;
    QTextEdit *logDisplay;
    QPushButton *runButton;
    QPushButton *savePresetButton;
    QPushButton *loadPresetButton;
    QPushButton *deletePresetButton;

    // === Логика ===
    QProcess *ffmpegProcess;
    PresetManager *presetManager;
    QString inputFile;

    // === Методы ===
    QWidget* createCentralWidget();
    QString generateFFmpegCommand() const;
};

#endif // MAINWINDOW_H
