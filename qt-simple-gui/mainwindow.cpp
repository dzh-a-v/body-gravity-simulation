#include "mainwindow.h"
#include "simulation.h"
#include "physics.h"
#include "helpers.h"

#include <QApplication>
#include <QFont>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedWidget>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <climits>

QString MainWindow::formatDouble(double value) {
    if (value == 0.0) return "0";
    std::stringstream ss;
    ss << std::scientific << std::setprecision(6) << value;
    return QString::fromStdString(ss.str());
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , sim(nullptr)
    , stepCount(0)
    , maxSteps(INT_MAX)
    , logInterval(100.0)
    , lastLogTime(-logInterval)
    , isRunning(false)
{
    // --- Setup Page ---
    setupPage = new QWidget(this);
    QVBoxLayout* setupLayout = new QVBoxLayout(setupPage);

    setupLayout->addWidget(new QLabel("Time step (dt):"));
    dtEdit = new QLineEdit("10.0");
    setupLayout->addWidget(dtEdit);

    setupLayout->addWidget(new QLabel("Max steps (0 = unlimited):"));
    maxStepsEdit = new QLineEdit("0");
    setupLayout->addWidget(maxStepsEdit);

    setupLayout->addWidget(new QLabel("Bodies:"));
    bodiesTable = new QTableWidget(0, 6);
    bodiesTable->setHorizontalHeaderLabels({ "Mass", "Radius", "X", "Y", "VX", "VY" });
    setupLayout->addWidget(bodiesTable);

    QPushButton* addBodyBtn = new QPushButton("Add Body");
    QPushButton* resetBtn = new QPushButton("Reset to Default");
    startButton = new QPushButton("▶ Start Simulation");

    connect(addBodyBtn, &QPushButton::clicked, this, &MainWindow::addBodyRow);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetToDefault);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(addBodyBtn);
    btnLayout->addWidget(resetBtn);
    btnLayout->addWidget(startButton);
    setupLayout->addLayout(btnLayout);

    // --- Simulation Page ---
    simPage = new QWidget(this);

    propertiesTable = new QTableWidget();
    propertiesTable->setColumnCount(5);
    propertiesTable->setHorizontalHeaderLabels({ "ID", "Mass", "Position", "Velocity", "Acceleration" });
    propertiesTable->horizontalHeader()->setStretchLastSection(true);
    propertiesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    propertiesTable->setMinimumHeight(300);
    propertiesTable->horizontalHeader()->setSectionsMovable(true);

    body1Combo = new QComboBox();
    body2Combo = new QComboBox();
    distanceLabel = new QLabel("Distance: —");
    distanceLabel->setFont(QFont("Courier New", 10));
    distanceLabel->setMinimumWidth(220);

    propertiesTable->setStyleSheet("QTableWidget { background-color: #f9f9f9; gridline-color: #ddd; }"
        "QHeaderView::section { background-color: #e0e0e0; padding: 4px; }");
    distanceLabel->setStyleSheet("QLabel { background-color: #e8f4f8; padding: 6px; border-radius: 4px; }");
    logView = new QTextEdit();
    logView->setStyleSheet("QTextEdit { background-color: #fdf6e3; color: #586e75; }");

    connect(body1Combo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateDistance);
    connect(body2Combo, QOverload<int>::of(&QComboBox::activated), this, &MainWindow::updateDistance);

    QWidget* selectorWidget = new QWidget();
    QVBoxLayout* selectorLayout = new QVBoxLayout(selectorWidget);
    selectorLayout->setContentsMargins(5, 5, 5, 5);
    selectorLayout->setSpacing(8);
    selectorLayout->addWidget(new QLabel("Body 1:"));
    selectorLayout->addWidget(body1Combo);
    selectorLayout->addWidget(new QLabel("Body 2:"));
    selectorLayout->addWidget(body2Combo);
    selectorLayout->addWidget(distanceLabel);
    selectorLayout->addStretch();
    selectorWidget->setMaximumWidth(220);

    topSplitter = new QSplitter(Qt::Horizontal);
    topSplitter->addWidget(propertiesTable);
    topSplitter->addWidget(selectorWidget);
    topSplitter->setSizes({ 900, 220 });

    logView->setReadOnly(true);
    logView->setFont(QFont("Courier New", 10));
    logView->append("🌌 Gravity Simulator Log\n");

    mainSplitter = new QSplitter(Qt::Vertical);
    mainSplitter->addWidget(topSplitter);
    mainSplitter->addWidget(logView);
    mainSplitter->setSizes({ 350, 250 });

    pauseButton = new QPushButton("⏸ Pause");
    pauseButton->setMinimumWidth(100);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::togglePause);

    QVBoxLayout* simLayout = new QVBoxLayout(simPage);
    simLayout->addWidget(pauseButton);
    simLayout->addWidget(mainSplitter);

    // --- Main Stack ---
    stack = new QStackedWidget(this);
    stack->addWidget(setupPage);
    stack->addWidget(simPage);
    setCentralWidget(stack);
    resize(1300, 700);
    setWindowTitle("Gravity Simulator — Setup");

    resetToDefault();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onSimulationStep);
}

MainWindow::~MainWindow() {
    delete sim;
}

void MainWindow::addBodyRow() {
    int row = bodiesTable->rowCount();
    bodiesTable->insertRow(row);
    for (int col = 0; col < 6; ++col) {
        bodiesTable->setItem(row, col, new QTableWidgetItem("0"));
    }
}

void MainWindow::resetToDefault() {
    bodiesTable->setRowCount(2);
    setBodyRow(0, 5.97e24, 6.37e6, 0, 0, 0, 0);
    setBodyRow(1, 1000, 1, 7.37e6, 0, 0, 7500);
}

void MainWindow::setBodyRow(int row, double mass, double rad, double x, double y, double vx, double vy) {
    bodiesTable->setItem(row, 0, new QTableWidgetItem(QString::number(mass, 'e', 6)));
    bodiesTable->setItem(row, 1, new QTableWidgetItem(QString::number(rad, 'e', 6)));
    bodiesTable->setItem(row, 2, new QTableWidgetItem(QString::number(x, 'e', 6)));
    bodiesTable->setItem(row, 3, new QTableWidgetItem(QString::number(y, 'e', 6)));
    bodiesTable->setItem(row, 4, new QTableWidgetItem(QString::number(vx, 'e', 6)));
    bodiesTable->setItem(row, 5, new QTableWidgetItem(QString::number(vy, 'e', 6)));
}

void MainWindow::startSimulation() {
    delete sim;
    sim = new Simulation();

    bool ok;
    double dt = dtEdit->text().toDouble(&ok);
    if (!ok || dt <= 0) dt = 10.0;
    sim->dt = dt;

    int maxStepsInput = maxStepsEdit->text().toInt(&ok);
    maxSteps = (ok && maxStepsInput > 0) ? maxStepsInput : INT_MAX;

    for (int i = 0; i < bodiesTable->rowCount(); ++i) {
        auto readCell = [&](int col) -> double {
            QTableWidgetItem* item = bodiesTable->item(i, col);
            if (!item) return 0.0;
            QString text = item->text();
            bool ok; double v = text.toDouble(&ok);
            return ok ? v : 0.0;
            };
        Body b(readCell(0), readCell(1), { readCell(2), readCell(3) }, { readCell(4), readCell(5) });
        sim->addBody(b);
    }

    stack->setCurrentWidget(simPage);
    setWindowTitle("Gravity Simulator — Running");
    stepCount = 0;
    lastLogTime = -logInterval;
    isRunning = true;
    timer->start(50);
}

void MainWindow::togglePause() {
    if (isRunning) {
        timer->stop();
        pauseButton->setText("▶ Resume");
        isRunning = false;
    }
    else {
        timer->start(50);
        pauseButton->setText("⏸ Pause");
        isRunning = true;
    }
    pauseButton->setStyleSheet(isRunning ? "background-color: #ffebee;" : "background-color: #e8f5e9;");
}

void MainWindow::updatePropertiesTable(const Simulation& sim) {
    propertiesTable->setRowCount(static_cast<int>(sim.bodies.size()));
    for (size_t i = 0; i < sim.bodies.size(); ++i) {
        const auto& b = sim.bodies[i];
        int row = static_cast<int>(i);
        propertiesTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        propertiesTable->setItem(row, 1, new QTableWidgetItem(formatDouble(b.mass)));
        propertiesTable->setItem(row, 2, new QTableWidgetItem(b.position.toString()));     // ✅
        propertiesTable->setItem(row, 3, new QTableWidgetItem(b.velocity.toString()));    // ✅
        propertiesTable->setItem(row, 4, new QTableWidgetItem(b.acceleration.toString())); // ✅
    }

    int prev1 = body1Combo->currentIndex();
    int prev2 = body2Combo->currentIndex();

    if (body1Combo->count() != static_cast<int>(sim.bodies.size())) {
        body1Combo->clear();
        body2Combo->clear();
        for (size_t i = 0; i < sim.bodies.size(); ++i) {
            QString name = QString("Body %1").arg(i);
            body1Combo->addItem(name);
            body2Combo->addItem(name);
        }
        if (sim.bodies.size() >= 2) {
            int i1 = (prev1 >= 0 && prev1 < static_cast<int>(sim.bodies.size())) ? prev1 : 0;
            int i2 = (prev2 >= 0 && prev2 < static_cast<int>(sim.bodies.size()) && prev2 != i1) ? prev2 : 1;
            if (i1 == i2 && sim.bodies.size() > 1) i2 = (i1 + 1) % sim.bodies.size();
            body1Combo->setCurrentIndex(i1);
            body2Combo->setCurrentIndex(i2);
        }
    }
    updateDistance();
}

void MainWindow::updateDistance() {
    if (!sim || sim->bodies.size() < 2) {
        distanceLabel->setText("Distance: —");
        return;
    }
    int i1 = body1Combo->currentIndex();
    int i2 = body2Combo->currentIndex();
    if (i1 < 0 || i2 < 0 || i1 >= static_cast<int>(sim->bodies.size()) ||
        i2 >= static_cast<int>(sim->bodies.size()) || i1 == i2) {
        distanceLabel->setText("Distance: —");
        return;
    }
    LD d = Physics::calculateDistance(sim->bodies[i1], sim->bodies[i2]);
    distanceLabel->setText(QString("Distance: %1 m").arg(formatDouble(d)));
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
            double acc = std::sqrt(b.acceleration.x * b.acceleration.x + b.acceleration.y * b.acceleration.y);
            appendToLog(QString("  [%1] pos=%2, vel=%3, acc=%4, |v|=%5, |a|=%6")
                .arg(static_cast<int>(i))
                .arg(b.position.toString())        // ✅
                .arg(b.velocity.toString())        // ✅
                .arg(b.acceleration.toString())    // ✅
                .arg(formatDouble(speed))
                .arg(formatDouble(acc)));
        }
        appendToLog("-----");
    }
}

void MainWindow::appendToLog(const QString& text) {
    logView->append(text);
    QTextCursor c = logView->textCursor();
    c.movePosition(QTextCursor::End);
    logView->setTextCursor(c);
}