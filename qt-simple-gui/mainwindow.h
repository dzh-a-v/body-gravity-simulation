#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QTimer>

// Предварительное объявление класса Simulation (реализация в твоих файлах)
class Simulation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onSimulationStep();

private:
    void updatePropertiesTable(const Simulation& sim);
    void appendToLog(const QString& text);

    // UI
    QSplitter* splitter;
    QTableWidget* propertiesTable;
    QTextEdit* logView;

    // Логика
    Simulation* sim;
    QTimer* timer;
    int stepCount;
    const int maxSteps = 5000;
    double logInterval; // вывод лога каждые N секунд
    double lastLogTime;
};

#endif // MAINWINDOW_H