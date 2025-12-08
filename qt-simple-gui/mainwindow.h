#pragma once

#include "body.h"
#include "simulation.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QTimer>
#include <QComboBox>
#include <QLabel>
#include <QSignalMapper>

class Simulation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onSimulationStep();
    void updateDistance();

private:
    void updatePropertiesTable(const Simulation& sim);
    void appendToLog(const QString& text);
    QString formatDouble(double value);
    QString formatVec2(const Vec2& v);

    // UI
    QSplitter* mainSplitter;        // Vertical
    QSplitter* topSplitter;         // Horizontal
    QTableWidget* propertiesTable;
    QTextEdit* logView;
    QComboBox* body1Combo;
    QComboBox* body2Combo;
    QLabel* distanceLabel;

    // Logic
    Simulation* sim;
    QTimer* timer;
    int stepCount;
    const int maxSteps = 5000;
    LD logInterval;
    LD lastLogTime;
};