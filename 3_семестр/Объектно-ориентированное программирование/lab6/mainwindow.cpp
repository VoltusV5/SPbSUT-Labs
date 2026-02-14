#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QMenuBar>
#include <QTableWidget>
#include <QStringList>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), experimentNumber(1)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    lineEdit = new QLineEdit(this);
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(5);
    QStringList headers = {"№", "Operand 1", "Operand 2", "Operation", "Result"};
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton *addButton = new QPushButton("Add", this);
    QPushButton *subButton = new QPushButton("Subtract", this);
    QPushButton *mulButton = new QPushButton("Multiply", this);
    QPushButton *divButton = new QPushButton("Divide", this);

    QToolBar *toolBar = addToolBar("Operations");
    toolBar->addWidget(addButton);
    toolBar->addWidget(subButton);
    toolBar->addWidget(mulButton);
    toolBar->addWidget(divButton);

    connect(addButton, &QPushButton::clicked, this, &MainWindow::on_addButton_clicked);
    connect(subButton, &QPushButton::clicked, this, &MainWindow::on_subtractButton_clicked);
    connect(mulButton, &QPushButton::clicked, this, &MainWindow::on_multiplyButton_clicked);
    connect(divButton, &QPushButton::clicked, this, &MainWindow::on_divideButton_clicked);

    strEdtLower = new QLineEdit(this);
    strEdtUpper = new QLineEdit(this);

    QPushButton *reverseLowerBtn = new QPushButton("Reverse Lower", this);
    QPushButton *reverseUpperBtn = new QPushButton("Reverse Upper", this);
    QPushButton *reverseAllBtn = new QPushButton("Reverse All", this);
    QPushButton *swapBtn = new QPushButton("Swap", this);
    QPushButton *concatLowerUpperBtn = new QPushButton("Concat Lower+Upper", this);
    QPushButton *concatUpperLowerBtn = new QPushButton("Concat Upper+Lower", this);

    connect(reverseLowerBtn, &QPushButton::clicked, this, &MainWindow::on_reverseLower_clicked);
    connect(reverseUpperBtn, &QPushButton::clicked, this, &MainWindow::on_reverseUpper_clicked);
    connect(reverseAllBtn, &QPushButton::clicked, this, &MainWindow::on_reverseAll_clicked);
    connect(swapBtn, &QPushButton::clicked, this, &MainWindow::on_swap_clicked);
    connect(concatLowerUpperBtn, &QPushButton::clicked, this, &MainWindow::on_concatLowerUpper_clicked);
    connect(concatUpperLowerBtn, &QPushButton::clicked, this, &MainWindow::on_concatUpperLower_clicked);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Калькулятор
    mainLayout->addWidget(new QLabel("Calculator Input (two numbers separated by space):"));
    mainLayout->addWidget(lineEdit);
    mainLayout->addWidget(tableWidget);

    // Текстовый редактор
    mainLayout->addWidget(new QLabel("Text Editor Operations:"));
    mainLayout->addWidget(strEdtLower);
    mainLayout->addWidget(strEdtUpper);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(reverseLowerBtn);
    btnLayout->addWidget(reverseUpperBtn);
    btnLayout->addWidget(reverseAllBtn);
    btnLayout->addWidget(swapBtn);
    btnLayout->addWidget(concatLowerUpperBtn);
    btnLayout->addWidget(concatUpperLowerBtn);

    mainLayout->addLayout(btnLayout);

    central->setLayout(mainLayout);
}

MainWindow::~MainWindow() {}

// ----------------- Калькулятор -----------------
void MainWindow::on_addButton_clicked() {
    QStringList operands = lineEdit->text().split(" ", Qt::SkipEmptyParts);
    if (operands.size() != 2) return;

    bool ok1, ok2;
    double a = operands[0].toDouble(&ok1);
    double b = operands[1].toDouble(&ok2);
    if (!ok1 || !ok2) return;

    double res = a + b;

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(experimentNumber++)));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(a)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(b)));
    tableWidget->setItem(row, 3, new QTableWidgetItem("+"));
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(res)));
}

void MainWindow::on_subtractButton_clicked() {
    QStringList operands = lineEdit->text().split(" ", Qt::SkipEmptyParts);
    if (operands.size() != 2) return;

    bool ok1, ok2;
    double a = operands[0].toDouble(&ok1);
    double b = operands[1].toDouble(&ok2);
    if (!ok1 || !ok2) return;

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(experimentNumber++)));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(a)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(b)));
    tableWidget->setItem(row, 3, new QTableWidgetItem("-"));
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(a - b)));
}

void MainWindow::on_multiplyButton_clicked() {
    QStringList operands = lineEdit->text().split(" ", Qt::SkipEmptyParts);
    if (operands.size() != 2) return;

    bool ok1, ok2;
    double a = operands[0].toDouble(&ok1);
    double b = operands[1].toDouble(&ok2);
    if (!ok1 || !ok2) return;

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(experimentNumber++)));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(a)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(b)));
    tableWidget->setItem(row, 3, new QTableWidgetItem("*"));
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(a * b)));
}

void MainWindow::on_divideButton_clicked() {
    QStringList operands = lineEdit->text().split(" ", Qt::SkipEmptyParts);
    if (operands.size() != 2) return;

    bool ok1, ok2;
    double a = operands[0].toDouble(&ok1);
    double b = operands[1].toDouble(&ok2);
    if (!ok1 || !ok2 || b == 0) return;

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(experimentNumber++)));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(a)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(b)));
    tableWidget->setItem(row, 3, new QTableWidgetItem("/"));
    tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(a / b)));
}

// ----------------- Текстовый редактор -----------------
void MainWindow::on_reverseLower_clicked() {
    QString text = strEdtLower->text();
    std::reverse(text.begin(), text.end());
    strEdtLower->setText(text);
}

void MainWindow::on_reverseUpper_clicked() {
    QString text = strEdtUpper->text();
    std::reverse(text.begin(), text.end());
    strEdtUpper->setText(text);
}

void MainWindow::on_reverseAll_clicked() {
    QString lower = strEdtLower->text();
    QString upper = strEdtUpper->text();
    std::reverse(lower.begin(), lower.end());
    std::reverse(upper.begin(), upper.end());
    strEdtLower->setText(lower);
    strEdtUpper->setText(upper);
}

void MainWindow::on_swap_clicked() {
    QString lower = strEdtLower->text();
    QString upper = strEdtUpper->text();
    strEdtLower->setText(upper);
    strEdtUpper->setText(lower);
}


void MainWindow::on_concatLowerUpper_clicked() {
    QString lower = strEdtLower->text();
    QString upper = strEdtUpper->text();
    strEdtLower->setText(lower + upper);
}

void MainWindow::on_concatUpperLower_clicked() {
    QString lower = strEdtLower->text();
    QString upper = strEdtUpper->text();
    strEdtUpper->setText(upper + lower);
}

