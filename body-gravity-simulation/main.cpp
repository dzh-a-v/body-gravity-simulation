#include "simulation.h"
#include "output.h"

int main() {
    Simulation sim;
    sim.dt = 0.01; // 1 секунда на шаг

    // Земля (массивная, в центре)
    sim.addBody(Body(5.97e24, 6.37e6, { 0, 0 }, { 0, 0 }));

    // Объект на высоте 1000 км, неподвижен
    sim.addBody(Body(1, 1, { 7.37e6, 0 }, { 0, 0 }));

    ConsoleOutput out;

    for (int step = 0; step < 1000000; ++step) {
        if (sim.hasCollision()) {
            std::cout << "Collision happend at t = " << sim.time << " s!\n";
            sim.isFinished = true;
            break;
        }

        sim.step();

        if (step % 50 == 0) {
            out.render(sim);
        }
    }

    return 0;
}