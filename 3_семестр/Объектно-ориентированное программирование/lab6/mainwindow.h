#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Калькулятор
    void on_addButton_clicked();
    void on_subtractButton_clicked();
    void on_multiplyButton_clicked();
    void on_divideButton_clicked();

    // Текстовый редактор
    void on_reverseLower_clicked();
    void on_reverseUpper_clicked();
    void on_reverseAll_clicked();
    void on_swap_clicked();
    void on_concatLowerUpper_clicked();
    void on_concatUpperLower_clicked();

private:
    // Члены класса для виджетов
    QLineEdit *lineEdit;
    QTextEdit *textEdit;
    QLineEdit *strEdtLower;
    QLineEdit *strEdtUpper;
    QTableWidget *tableWidget;
    int experimentNumber = 1;
};

#endif // MAINWINDOW_H
