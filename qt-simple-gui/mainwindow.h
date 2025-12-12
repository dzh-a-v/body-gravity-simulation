#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QTimer>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedWidget>
#include <QSlider>  // добавлено

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
    void togglePause();
    void startSimulation();
    void addBodyRow();
    void resetToDefault();
    void removeSelectedBody();

private:
    void updatePropertiesTable(const Simulation& sim);
    void appendToLog(const QString& text);
    int intervalToSliderPos(int interval);
    int sliderPosToInterval(int pos);
    QString formatDouble(double value);

    // UI Pages
    QStackedWidget* stack;
    QWidget* setupPage;
    QWidget* simPage;

    // Setup Page UI
    QLineEdit* dtEdit;
    QLineEdit* maxStepsEdit;
    QPushButton* startButton;

    // Simulation Page UI
    QSplitter* mainSplitter;
    QSplitter* topSplitter;
    QTableWidget* propertiesTable;
    QTextEdit* logView;
    QComboBox* body1Combo;
    QComboBox* body2Combo;
    QLabel* distanceLabel;
    QPushButton* pauseButton;
    QPushButton* restartButton;
    QSlider* speedSlider;

    // Logic
    Simulation* sim;
    QTimer* timer;
    int stepCount;
    int maxSteps;
    double logInterval;
    double lastLogTime;
    bool isRunning;

    QTableWidget* bodiesTable;
    void setBodyRow(int row, double mass, double rad, double x, double y, double vx, double vy);
};