#include "tests.h"
#include "simulation.h"
#include "output.h"

void Tests::test_equilateral_triangle() {
    Simulation sim;
    sim.dt = 1000.0;

    double M = 1e10;
    double R = 10000.0; // расстояние от центра до вершины

    // Углы треугольника
    double a1 = 0;
    double a2 = 2 * M_PI / 3;
    double a3 = 4 * M_PI / 3;

    sim.addBody(Body(M, 10, { R * cos(a1), R * sin(a1) }, { 0, 0 }));
    sim.addBody(Body(M, 10, { R * cos(a2), R * sin(a2) }, { 0, 0 }));
    sim.addBody(Body(M, 10, { R * cos(a3), R * sin(a3) }, { 0, 0 }));

    ConsoleOutput out;
    for (int step = 0; step < 300; ++step) {
        sim.step();
        if (step % 100 == 0) out.render(sim);
    }

    std::cout << "Test completed: three-body symmetric collapse.\n";
}

void Tests::test_earth_satellite() {
    Simulation sim;
    sim.dt = 0.001; // 0.001 секунда на шаг дифференциирования

    // Земля (массивная, в центре)
    sim.addBody(Body(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 }));

    // Объект на высоте 1000 км, неподвижен
    sim.addBody(Body(1, 1, { 7.37e6, 0 }, { 0, 0 }));

    ConsoleOutput out;

    for (int step = 0; step < 10000000; ++step) {
        if (sim.hasCollision()) {
            std::cout << "Collision happend at t = " << sim.time << " s!\n";
            sim.isFinished = true;
            break;
        }

        sim.step();

        if (step % 100000 == 0) {
            out.render(sim);
        }
    }
}

void Tests::test_earth_satellite_testParticle() {
    Simulation sim;
    sim.dt = 0.001; // 0.001 секунда на шаг дифференциирования

    // Земля и спутник — как в 2-тел задаче
    Body earth(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 });
    Body sat(1000, 1, { 7.37e6, 0 }, { 0, 7500 }); // орбитальная скорость ~7500 м/с

    // Третье тело — масса 0 (не должно влиять)
    Body test_particle(0, 1, { 7.37e6 + 1000, 0 }, { 0, 7500 });

    sim.addBody(earth); sim.addBody(sat);
    sim.addBody(test_particle);

    ConsoleOutput out;

    for (int step = 0; step < 10000000; ++step) {
        if (sim.hasCollision()) {
            std::cout << "Collision happend at t = " << sim.time << " s!\n";
            sim.isFinished = true;
            break;
        }

        sim.step();

        if (step % 100000 == 0) {
            out.render(sim);
        }
    }
}

void Tests::test_perturbation_small_mass() {
    Simulation sim;
    sim.dt = 1.0;

    // Основная система
    sim.addBody(Body(1e12, 10, { 0, 0 }, { 0, 0 }));
    sim.addBody(Body(1e10, 5, { 10000, 0 }, { 0, 300 }));

    // Малое тело далеко
    sim.addBody(Body(1e3, 1, { 20000, 0 }, { 0, 200 }));

    ConsoleOutput out;
    for (int step = 0; step < 200; ++step) {
        sim.step();
        if (step % 50 == 0) out.render(sim);
    }

    std::cout << "Test completed: small mass causes minor perturbation.\n";
}