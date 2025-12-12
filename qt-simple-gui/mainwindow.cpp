#include "mainwindow.h"
#include "simulation.h"
#include "helpers.h"

#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <sstream>
#include <iomanip>
#include <cmath>

// === Format helpers ===

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

void MainWindow::togglePause() {
    if (isRunning) {
        // Останавливаем
        timer->stop();
        pauseButton->setText("▶ Resume");
        isRunning = false;
    }
    else {
        // Возобновляем
        timer->start(50); // тот же интервал, что и в конструкторе
        pauseButton->setText("⏸ Pause");
        isRunning = true;
    }
    pauseButton->setStyleSheet(
        isRunning ? "background-color: #ffebee;" : "background-color: #e8f5e9;"
    );
}

// === Constructor ===

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , sim(new Simulation())
    , stepCount(0)
    , logInterval(100.0)
    , lastLogTime(-logInterval)
    , isRunning(true)
{
    // --- UI: table ---
    propertiesTable = new QTableWidget(this);
    propertiesTable->setColumnCount(5);
    propertiesTable->setHorizontalHeaderLabels({ "ID", "Mass", "Position", "Velocity", "Acceleration" });
    propertiesTable->horizontalHeader()->setStretchLastSection(true);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTable->setMinimumHeight(300); // #50: make table higher
    propertiesTable->horizontalHeader()->setSectionsMovable(true);

    // --- UI: bodies choice (VERTICAL layout) ---
    body1Combo = new QComboBox(this);
    body2Combo = new QComboBox(this);
    distanceLabel = new QLabel("Distance: —", this);
    distanceLabel->setFont(QFont("Courier New", 10));
    distanceLabel->setMinimumWidth(220);

    // Стили для цвета и читаемости
    propertiesTable->setStyleSheet(
        "QTableWidget { background-color: #f9f9f9; gridline-color: #ddd; }"
        "QHeaderView::section { background-color: #e0e0e0; padding: 4px; }"
    );
    distanceLabel->setStyleSheet("QLabel { background-color: #e8f4f8; padding: 6px; border-radius: 4px; }");
    logView = new QTextEdit(this);
    logView->setStyleSheet("QTextEdit { background-color: #fdf6e3; color: #586e75; }");

    connect(body1Combo, QOverload<int>::of(&QComboBox::activated),
        this, &MainWindow::updateDistance);
    connect(body2Combo, QOverload<int>::of(&QComboBox::activated),
        this, &MainWindow::updateDistance);

    // --- Вертикальный layout для выбора тел (#49) ---
    QWidget* selectorWidget = new QWidget(this);
    QVBoxLayout* selectorLayout = new QVBoxLayout(selectorWidget);
    selectorLayout->setContentsMargins(5, 5, 5, 5);
    selectorLayout->setSpacing(8);

    selectorLayout->addWidget(new QLabel("Body 1:", this));
    selectorLayout->addWidget(body1Combo);
    selectorLayout->addWidget(new QLabel("Body 2:", this));
    selectorLayout->addWidget(body2Combo);
    selectorLayout->addWidget(distanceLabel);
    selectorLayout->addStretch(); // толкает элементы вверх

    // Ограничиваем ширину панели выбора (#51)
    selectorWidget->setMaximumWidth(220); // узкая вертикальная панель

    // --- Top horizontal splitter: table + selector ---
    topSplitter = new QSplitter(Qt::Horizontal, this);
    topSplitter->addWidget(propertiesTable);
    topSplitter->addWidget(selectorWidget);
    topSplitter->setSizes({ 900, 220 }); // #51: make selector narrower

    // --- Log ---
    logView->setReadOnly(true);
    logView->setFont(QFont("Courier New", 10));
    logView->append("🌌 Gravity Simulator Log\n");

    // --- Main vertical splitter ---
    mainSplitter = new QSplitter(Qt::Vertical, this);
    mainSplitter->addWidget(topSplitter);
    mainSplitter->addWidget(logView);
    mainSplitter->setSizes({ 350, 250 });

    // --- Pause/Resume button ---
    pauseButton = new QPushButton("⏸ Pause", this);
    pauseButton->setMinimumWidth(100);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::togglePause);

    // Добавим кнопку **над** основным сплиттером
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(pauseButton);
    mainLayout->addWidget(mainSplitter);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    //setCentralWidget(mainSplitter);
    resize(1300, 700);
    setWindowTitle("Gravity Simulator — Text UI");

    // --- Simulation initialization ---
    sim->addBody(Body(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 }));
    sim->addBody(Body(1000, 1, { 7.37e6, 0 }, { 0, 7500 }));
    sim->addBody(Body(1, 1, { 7e7, 0 }, { 0, 10000 }));
    sim->dt = 10.0;

    // --- Timer ---
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onSimulationStep);
    timer->start(50);

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

    if (body1Combo->count() != static_cast<int>(sim.bodies.size())) {
        body1Combo->clear();
        body2Combo->clear();
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            QString name = QString("Body %1").arg(i);
            body1Combo->addItem(name);
            body2Combo->addItem(name);
        }

        if (sim.bodies.size() >= 2) {
            int newIdx1 = (prevIndex1 >= 0 && prevIndex1 < static_cast<int>(sim.bodies.size())) ? prevIndex1 : 0;
            int newIdx2 = (prevIndex2 >= 0 && prevIndex2 < static_cast<int>(sim.bodies.size()) && prevIndex2 != newIdx1) ? prevIndex2 : 1;
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
    LD dx = b1.position.x - b2.position.x;
    LD dy = b1.position.y - b2.position.y;
    LD dist = std::sqrt(dx * dx + dy * dy);

    distanceLabel->setText(QString("Distance: %1 m").arg(formatDouble(dist)));
}

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
            double speed = std::sqrt(b.velocity.x * b.velocity.x + b.velocity.y * b.velocity.y);
            double accMag = std::sqrt(b.acceleration.x * b.acceleration.x + b.acceleration.y * b.acceleration.y);
            appendToLog(QString("  [%1] pos=%2, vel=%3, acc=%4, |v|=%5, |a|=%6")
                .arg(static_cast<int>(i))
                .arg(formatVec2(b.position))
                .arg(formatVec2(b.velocity))
                .arg(formatVec2(b.acceleration))
                .arg(formatDouble(speed))
                .arg(formatDouble(accMag)));
        }
        appendToLog("-----");
    }
}

void MainWindow::appendToLog(const QString& text) {
    logView->append(text);
    QTextCursor cursor = logView->textCursor();
    cursor.movePosition(QTextCursor::End);
    logView->setTextCursor(cursor);
}