#include "mainwindow.h"
#include "simulation.h" // твой файл с Simulation и Body
#include "helpers.h"

#include <QVBoxLayout>
#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <iostream>
#include <iomanip>
#include <sstream>

// Помощник: форматирование double в строку с научной нотацией
QString formatDouble(double value) {
    if (std::abs(value) < 1e-15) return "0";
    return QString::number(value, 'e', 3);
}

QString formatVec2(const Vec2& v) {
    return QString("(%1, %2)")
        .arg(formatDouble(v.x))
        .arg(formatDouble(v.y));
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , sim(new Simulation())
    , stepCount(0)
    , logInterval(100.0) // лог каждые 100 секунд симуляции
    , lastLogTime(-logInterval)
{
    // === UI ===
    propertiesTable = new QTableWidget(this);
    propertiesTable->setColumnCount(7);
    propertiesTable->setHorizontalHeaderLabels({ "ID", "Mass", "Position", "Velocity", "Acceleration", "|vel|", "|acc|"});
    propertiesTable->horizontalHeader()->setStretchLastSection(true);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // только чтение

    logView = new QTextEdit(this);
    logView->setReadOnly(true);
    logView->setFont(QFont("Courier New", 10)); // моноширинный шрифт
    logView->append("🌌 Gravity Simulator Log\n");

    splitter = new QSplitter(Qt::Vertical, this);
    splitter->addWidget(propertiesTable);
    splitter->addWidget(logView);
    splitter->setSizes({ 200, 400 }); // высота: верхняя панель ~200px, нижняя — остальное

    setCentralWidget(splitter);
    resize(1000, 600);
    setWindowTitle("Gravity Simulator — Text UI");

    // === Инициализация симуляции ===
    // Пример: Земля + спутник
    sim->addBody(Body(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 }));
    sim->addBody(Body(1000, 1, { 7.37e6, 0 }, { 0, 7500 }));
    sim->dt = 10.0; // шаг интегрирования

    // === Таймер ===
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onSimulationStep);
    timer->start(50); // обновление каждые 50 мс (20 FPS)

    // Первое обновление
    updatePropertiesTable(*sim);
}

MainWindow::~MainWindow() {
    delete sim;
}

void MainWindow::onSimulationStep() {
    if (stepCount >= maxSteps) {
        timer->stop();
        appendToLog("Simulation finished (max steps reached).");
        return;
    }

    sim->step(); // <-- твой метод Simulation::step()
    stepCount++;

    // Обновляем таблицу свойств
    updatePropertiesTable(*sim);

    // Добавляем в лог, если прошло достаточно времени
    if (sim->time - lastLogTime >= logInterval) {
        lastLogTime = sim->time;

        QString logLine = QString("t = %1 s").arg(sim->time, 0, 'f', 1);
        appendToLog(logLine);

        for (size_t i = 0; i < sim->bodies.size(); ++i) {
            const auto& b = sim->bodies[i];
            QString bodyLine = QString("  [%1] pos=%2 vel=%3 acc=%4")
                .arg(static_cast<int>(i))
                .arg(formatVec2(b.position))
                .arg(formatVec2(b.velocity))
                .arg(formatVec2(b.acceleration));
            appendToLog(bodyLine);
        }
        appendToLog("---");
    }
}

void MainWindow::updatePropertiesTable(const Simulation& sim) {
    propertiesTable->setRowCount(static_cast<int>(sim.bodies.size()));
    for (size_t i = 0; i < sim.bodies.size(); ++i) {
        const auto& b = sim.bodies[i];
        int row = static_cast<int>(i);
        propertiesTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        propertiesTable->setItem(row, 1, new QTableWidgetItem(formatDouble(b.mass)));
        propertiesTable->setItem(row, 2, new QTableWidgetItem(formatVec2(b.position)));
        propertiesTable->setItem(row, 3, new QTableWidgetItem(formatVec2(b.velocity)));
        propertiesTable->setItem(row, 4, new QTableWidgetItem(formatVec2(b.acceleration)));
        propertiesTable->setItem(row, 5, new QTableWidgetItem(formatDouble(sqrt(b.velocity.x * b.velocity.x + b.velocity.y * b.velocity.y))));
        propertiesTable->setItem(row, 6, new QTableWidgetItem(formatDouble(sqrt(b.acceleration.x * b.acceleration.x + b.acceleration.y * b.acceleration.y))));
    }
}

void MainWindow::appendToLog(const QString& text) {
    logView->append(text);
    // Автопрокрутка вниз
    QTextCursor cursor = logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    logView->setTextCursor(cursor);
}