#include "mainwindow.h"
#include "simulation.h"

#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <QVBoxLayout>
#include <sstream>
#include <iomanip>
#include <cmath>

// === Вспомогательные функции форматирования ===

QString MainWindow::formatDouble(double value) {
    if (value == 0.0) {
        return "0";
    }
    std::stringstream ss;
    ss << std::scientific << std::setprecision(6) << value;
    return QString::fromStdString(ss.str());
}

QString MainWindow::formatVec2(const Vec2& v) {
    return QString("(%1, %2)")
        .arg(formatDouble(v.x))
        .arg(formatDouble(v.y));
}

// === Конструктор ===

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , sim(new Simulation())
    , stepCount(0)
    , logInterval(100.0)
    , lastLogTime(-logInterval)
{
    // --- UI: таблица ---
    propertiesTable = new QTableWidget(this);
    propertiesTable->setColumnCount(5);
    propertiesTable->setHorizontalHeaderLabels({ "ID", "Mass", "Position", "Velocity", "Acceleration" });
    propertiesTable->horizontalHeader()->setStretchLastSection(true);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTable->setMinimumHeight(150);

    // --- UI: выбор тел ---
    body1Combo = new QComboBox(this);
    body2Combo = new QComboBox(this);
    distanceLabel = new QLabel("Distance: —", this);
    distanceLabel->setFont(QFont("Courier New", 10));
    distanceLabel->setMinimumWidth(200);

    connect(body1Combo, QOverload<int>::of(&QComboBox::activated),
        this, &MainWindow::updateDistance);
    connect(body2Combo, QOverload<int>::of(&QComboBox::activated),
        this, &MainWindow::updateDistance);

    // --- Верхний сплиттер (таблица + выбор) ---
    topSplitter = new QSplitter(Qt::Horizontal, this);
    topSplitter->addWidget(propertiesTable);
    topSplitter->addWidget(new QLabel("Body 1:", this));
    topSplitter->addWidget(body1Combo);
    topSplitter->addWidget(new QLabel("Body 2:", this));
    topSplitter->addWidget(body2Combo);
    topSplitter->addWidget(distanceLabel);
    topSplitter->setSizes({ 400, 50, 80, 50, 80, 200 });

    // --- Лог ---
    logView = new QTextEdit(this);
    logView->setReadOnly(true);
    logView->setFont(QFont("Courier New", 10));
    logView->append("🌌 Gravity Simulator Log\n");

    // --- Основной сплиттер (верх / низ) ---
    mainSplitter = new QSplitter(Qt::Vertical, this);
    mainSplitter->addWidget(topSplitter);
    mainSplitter->addWidget(logView);
    mainSplitter->setSizes({ 250, 350 });

    setCentralWidget(mainSplitter);
    resize(1000, 600);
    setWindowTitle("Gravity Simulator — Text UI");

    // --- Инициализация симуляции ---
    sim->addBody(Body(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 }));
    sim->addBody(Body(1000, 1, { 7.37e6, 0 }, { 0, 7500 }));
    sim->addBody(Body(1, 1, { 7e7, 0 }, { 0, 10000 }));
    sim->dt = 10.0;

    // --- Таймер ---
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onSimulationStep);
    timer->start(50);

    // Первое обновление
    updatePropertiesTable(*sim);
}

MainWindow::~MainWindow() {
    delete sim;
}

// === Обновление таблицы и комбобоксов ===

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
    }

    // === Preserve current selections ===
    int prevIndex1 = body1Combo->currentIndex();
    int prevIndex2 = body2Combo->currentIndex();

    // Only update combo box contents if count changed
    if (body1Combo->count() != static_cast<int>(sim.bodies.size())) {
        body1Combo->clear();
        body2Combo->clear();
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            QString name = QString("Body %1").arg(i);
            body1Combo->addItem(name);
            body2Combo->addItem(name);
        }

        // Restore previous selection if valid, otherwise use defaults
        if (sim.bodies.size() >= 2) {
            int newIdx1 = (prevIndex1 >= 0 && prevIndex1 < static_cast<int>(sim.bodies.size())) ? prevIndex1 : 0;
            int newIdx2 = (prevIndex2 >= 0 && prevIndex2 < static_cast<int>(sim.bodies.size()) && prevIndex2 != newIdx1) ? prevIndex2 : 1;
            // Ensure they are not the same
            if (newIdx1 == newIdx2 && sim.bodies.size() > 1) {
                newIdx2 = (newIdx1 + 1) % sim.bodies.size();
            }
            body1Combo->setCurrentIndex(newIdx1);
            body2Combo->setCurrentIndex(newIdx2);
        }
    }

    updateDistance();
}

// === Расчёт и отображение расстояния ===

void MainWindow::updateDistance() {
    if (sim->bodies.size() < 2) {
        distanceLabel->setText("Distance: —");
        return;
    }

    int id1 = body1Combo->currentIndex();
    int id2 = body2Combo->currentIndex();

    if (id1 < 0 || id2 < 0 || id1 >= static_cast<int>(sim->bodies.size()) ||
        id2 >= static_cast<int>(sim->bodies.size()) || id1 == id2) {
        distanceLabel->setText("Distance: —");
        return;
    }

    const auto& b1 = sim->bodies[id1];
    const auto& b2 = sim->bodies[id2];
    double dx = b1.position.x - b2.position.x;
    double dy = b1.position.y - b2.position.y;
    double dist = std::sqrt(dx * dx + dy * dy);

    distanceLabel->setText(QString("Distance: %1 m").arg(formatDouble(dist)));
}

// === Шаг симуляции ===

void MainWindow::onSimulationStep() {
    if (stepCount >= maxSteps) {
        timer->stop();
        appendToLog("⏹ Simulation finished (max steps reached).");
        return;
    }

    sim->step();
    stepCount++;
    updatePropertiesTable(*sim);

    if (sim->time - lastLogTime >= logInterval) {
        lastLogTime = sim->time;
        appendToLog(QString("t = %1 s").arg(sim->time, 0, 'f', 1));
        for (size_t i = 0; i < sim->bodies.size(); ++i) {
            const auto& b = sim->bodies[i];
            appendToLog(QString("  [%1] pos=%2 vel=%3 acc=%4")
                .arg(static_cast<int>(i))
                .arg(formatVec2(b.position))
                .arg(formatVec2(b.velocity))
                .arg(formatVec2(b.acceleration)));
        }
        appendToLog(QString("DEBUG: bodies count = %1").arg(sim->bodies.size()));
        appendToLog("---");
    }
}

// === Добавление в лог с автопрокруткой ===

void MainWindow::appendToLog(const QString& text) {
    logView->append(text);
    QTextCursor cursor = logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    logView->setTextCursor(cursor);
}
