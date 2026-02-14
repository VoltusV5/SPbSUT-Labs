#include "mainwindow.h"
#include "presetmanager.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileInfo>
#include <QClipboard>
#include <QGuiApplication>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ffmpegProcess(new QProcess(this))
    , presetManager(new PresetManager(this))
{
    setCentralWidget(createCentralWidget());
    setWindowTitle("OpenFF GUI - MVP");
    resize(700, 550);

    connect(ffmpegProcess, &QProcess::readyReadStandardOutput, this, &MainWindow::readProcessOutput);
    connect(ffmpegProcess, &QProcess::readyReadStandardError, this, &MainWindow::readProcessOutput);
    connect(ffmpegProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MainWindow::processFinished);
}

MainWindow::~MainWindow() = default;

QWidget* MainWindow::createCentralWidget() {
    QWidget *central = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // === Файл ===
    QHBoxLayout *fileLayout = new QHBoxLayout;
    QLabel *fileLabel = new QLabel("Входной файл:");
    inputFileEdit = new QLineEdit;
    inputFileEdit->setReadOnly(true);
    QPushButton *browseButton = new QPushButton("Обзор...");
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::selectInputFile);
    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(inputFileEdit, 1);
    fileLayout->addWidget(browseButton);
    mainLayout->addLayout(fileLayout);

    // === Параметры ===
    auto addParam = [&](const QString &label, QComboBox *&combo, const QStringList &items) {
        QHBoxLayout *layout = new QHBoxLayout;
        QLabel *lbl = new QLabel(label + ":");
        combo = new QComboBox;
        combo->addItems(items);
        layout->addWidget(lbl);
        layout->addWidget(combo, 1);
        mainLayout->addLayout(layout);
    };

    addParam("Кодек", codecCombo, {"libx264", "libx265"});
    addParam("Контейнер", containerCombo, {"mp4", "mkv"});
    addParam("Разрешение", resolutionCombo, {"480p", "720p", "1080p", "custom"});

    // Custom
    QHBoxLayout *customLayout = new QHBoxLayout;
    QLabel *customLabel = new QLabel("Custom (width:height):");
    customResolutionEdit = new QLineEdit;
    customResolutionEdit->setPlaceholderText("1920:1080");
    customResolutionEdit->setVisible(false);
    customLayout->addWidget(customLabel);
    customLayout->addWidget(customResolutionEdit, 1);
    mainLayout->addLayout(customLayout);

    // === Команда ===
    QLabel *cmdLabel = new QLabel("Команда FFmpeg:");
    QHBoxLayout *cmdLayout = new QHBoxLayout;
    commandDisplay = new QTextEdit;
    commandDisplay->setReadOnly(false);
    commandDisplay->setStyleSheet("QTextEdit { background-color: #f0f0f0; font-family: Consolas; }");
    commandDisplay->setMaximumHeight(80);

    QPushButton *copyCmdButton = new QPushButton("Копировать");
    copyCmdButton->setToolTip("Скопировать команду в буфер обмена");
    connect(copyCmdButton, &QPushButton::clicked, this, [this]() {
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(commandDisplay->toPlainText());
        QMessageBox::information(this, "Скопировано", "Команда скопирована в буфер обмена!");
    });

    cmdLayout->addWidget(commandDisplay, 1);
    cmdLayout->addWidget(copyCmdButton);
    mainLayout->addWidget(cmdLabel);
    mainLayout->addLayout(cmdLayout);

    // === Кнопки ===
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    runButton = new QPushButton("Запустить кодирование");
    runButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; font-weight: bold; padding: 8px; }");

    savePresetButton = new QPushButton("Сохранить пресет");

    QPushButton *loadPresetButton = new QPushButton("Загрузить пресет");
    connect(loadPresetButton, &QPushButton::clicked, this, &MainWindow::loadPreset);

    QPushButton *deletePresetButton = new QPushButton("Удалить пресет");
    connect(deletePresetButton, &QPushButton::clicked, this, &MainWindow::deletePreset);

    buttonLayout->addWidget(runButton);
    buttonLayout->addWidget(savePresetButton);
    buttonLayout->addWidget(loadPresetButton);
    buttonLayout->addWidget(deletePresetButton);
    mainLayout->addLayout(buttonLayout);

    // === Лог ===
    QLabel *logLabel = new QLabel("Лог выполнения:");
    logDisplay = new QTextEdit;
    logDisplay->setReadOnly(true);
    mainLayout->addWidget(logLabel);
    mainLayout->addWidget(logDisplay, 1);

    // === Сигналы ===
    connect(codecCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCommandFromGUI);
    connect(containerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCommandFromGUI);
    connect(resolutionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCommandFromGUI);
    connect(resolutionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateCustomResolutionVisibility);
    connect(customResolutionEdit, &QLineEdit::textChanged, this, &MainWindow::updateCommandFromGUI);

    connect(runButton, &QPushButton::clicked, this, &MainWindow::runEncoding);
    connect(savePresetButton, &QPushButton::clicked, this, &MainWindow::savePreset);

    return central;
}

// === Методы ===

void MainWindow::selectInputFile() {
    inputFile = QFileDialog::getOpenFileName(this, "Выберите видео", "", "Видео (*.mp4 *.mkv *.avi)");
    if (!inputFile.isEmpty()) {
        inputFileEdit->setText(inputFile);
        updateCommandFromGUI();
    }
}

void MainWindow::updateCustomResolutionVisibility() {
    bool isCustom = resolutionCombo->currentText() == "custom";
    customResolutionEdit->setVisible(isCustom);
    if (isCustom && customResolutionEdit->text().isEmpty()) {
        customResolutionEdit->setText("1920:1080");
    }
    updateCommandFromGUI();
}

void MainWindow::updateCommandFromGUI() {
    commandDisplay->setPlainText(generateFFmpegCommand());
}

QString MainWindow::generateFFmpegCommand() const {
    if (inputFile.isEmpty()) return "ffmpeg";

    QString codec = codecCombo->currentText();
    QString container = containerCombo->currentText();
    QString res = resolutionCombo->currentText();

    QString scale;
    if (res == "480p") scale = "scale=854:480";
    else if (res == "720p") scale = "scale=1280:720";
    else if (res == "1080p") scale = "scale=1920:1080";
    else if (res == "custom") {
        QString custom = customResolutionEdit->text().trimmed();
        if (custom.contains(':')) scale = "scale=" + custom;
    }

    QString inputPath = QFileInfo(inputFile).absolutePath();
    QString inputBaseName = QFileInfo(inputFile).completeBaseName();
    QString baseOutput = inputPath + "/" + inputBaseName + "_converted";
    QString outputFile = baseOutput + "." + container;

    // УНИКАЛЬНОЕ ИМЯ БЕЗ ПРОБЕЛОВ И СКОБОК
    int counter = 1;
    QString finalOutput = outputFile;
    while (QFile::exists(finalOutput)) {
        finalOutput = baseOutput + "_" + QString::number(counter) + "." + container;
        counter++;
    }

    QStringList args;
    args << "ffmpeg" << "-i" << inputFile;

    if (!scale.isEmpty() && codec != "copy")
        args << "-vf" << scale;
    if (codec != "copy")
        args << "-c:v" << codec;

    args << finalOutput;

    return args.join(" ");
}

void MainWindow::runEncoding() {

    if (ffmpegProcess->state() != QProcess::NotRunning) {
        QMessageBox::information(this, "Ожидание", "Дождитесь завершения текущего кодирования");
        return;
    }
    // === ГЕНЕРИРУЕМ НОВУЮ КОМАНДУ С УНИКАЛЬНЫМ ИМЕНЕМ ===
    QString newCmd = generateFFmpegCommand();
    commandDisplay->setPlainText(newCmd);

    // === ПАРСИМ КОМАНДУ ===
    QStringList args = newCmd.split(' ', Qt::SkipEmptyParts);
    if (args.isEmpty() || args[0] != "ffmpeg") {
        QMessageBox::warning(this, "Ошибка", "Команда должна начинаться с 'ffmpeg'");
        return;
    }

    args.removeFirst(); // Убираем "ffmpeg"

    // === ПРОВЕРКА ВХОДНОГО ФАЙЛА ===
    int iIdx = args.indexOf("-i");
    if (iIdx == -1 || iIdx + 1 >= args.size()) {
        QMessageBox::warning(this, "Ошибка", "Не указан входной файл после -i");
        return;
    }

    QString inputFile = args[iIdx + 1];
    if (!QFile::exists(inputFile)) {
        QMessageBox::critical(this, "Ошибка", "Файл не существует:\n" + inputFile);
        return;
    }

    logDisplay->clear();
    logDisplay->append("<b>Запуск:</b> " + newCmd.toHtmlEscaped() + "<br>");

    runButton->setEnabled(false);
    // === ЗАПУСК ===
    ffmpegProcess->start("ffmpeg", args);
}

void MainWindow::readProcessOutput() {
    QString out = ffmpegProcess->readAllStandardOutput();
    QString err = ffmpegProcess->readAllStandardError();
    if (!out.isEmpty()) logDisplay->append("<font color='blue'>" + out.trimmed() + "</font>");
    if (!err.isEmpty()) logDisplay->append("<font color='red'>" + err.trimmed() + "</font>");
}

void MainWindow::processFinished(int exitCode) {
    runButton->setEnabled(true);
    logDisplay->append(QString("<br><b>Готово! Код: %1</b>").arg(exitCode));
    if (exitCode == 0)
        logDisplay->append("<font color='green'>Успешно!</font>");
    else
        logDisplay->append("<font color='red'>Ошибка.</font>");
}

void MainWindow::savePreset() {
    bool ok;
    QString name = QInputDialog::getText(this, "Сохранить", "Имя:", QLineEdit::Normal, "default", &ok);
    if (!ok || name.isEmpty()) return;

    presetManager->savePreset(name, codecCombo->currentText(), resolutionCombo->currentText(), containerCombo->currentText());
    QMessageBox::information(this, "OK", "Пресет \"" + name + "\" сохранён");
}

void MainWindow::loadPreset() {
    QStringList names = presetManager->presetNames();
    if (names.isEmpty()) {
        QMessageBox::information(this, "Пресеты", "Нет сохранённых пресетов");
        return;
    }

    bool ok;
    QString selected = QInputDialog::getItem(
        this, "Загрузить пресет", "Выберите пресет:", names, 0, false, &ok
    );

    if (!ok || selected.isEmpty()) return;

    auto preset = presetManager->loadPreset(selected);
    if (preset.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить пресет");
        return;
    }

    codecCombo->setCurrentText(preset["codec"]);
    resolutionCombo->setCurrentText(preset["resolution"]);
    containerCombo->setCurrentText(preset["container"]);
    updateCustomResolutionVisibility();
    updateCommandFromGUI();

    QMessageBox::information(this, "Успех", "Пресет \"" + selected + "\" загружен");
}

void MainWindow::deletePreset() {
    QStringList names = presetManager->presetNames();
    if (names.isEmpty()) {
        QMessageBox::information(this, "Пресеты", "Нет пресетов для удаления");
        return;
    }

    bool ok;
    QString selected = QInputDialog::getItem(
        this, "Удалить пресет", "Выберите пресет для удаления:", names, 0, false, &ok
    );

    if (!ok || selected.isEmpty()) return;

    int ret = QMessageBox::question(
        this, "Подтверждение",
        "Удалить пресет \"" + selected + "\"?\n\nЭто действие нельзя отменить.",
        QMessageBox::Yes | QMessageBox::No
    );

    if (ret == QMessageBox::Yes) {
        presetManager->removePreset(selected);
        QMessageBox::information(this, "Удалено", "Пресет \"" + selected + "\" удалён");
    }
}
