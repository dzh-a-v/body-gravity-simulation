# en
## Gravity-based N-Body Simulation (MVP)
This program simulates gravitational interactions between bodies (originally intended for celestial objects, but any parameters can be used).

## Current state
Currently, the program is console-only.

Despite its simplicity, this program is more physically accurate than most online calculators, which typically assume constant surface gravity and ignore how gravity weakens with distance. It also calculates very
small changes (like movements across the distance less than the size of protons) successfully.

For example, when calculating the free-fall time of an object dropped from 1000 km above Earth’s surface:

Most online tools give ~450 seconds,
This program gives ~510 seconds,
because it correctly accounts for the weaker gravitational acceleration at high altitudes.
At a 1000 km altitude, this results in over 10% difference—a significant improvement in accuracy.

## Physics used
The simulation integrates motion over time (time step dt is set in main.cpp).
Acceleration is computed using Newton’s law of universal gravitation:

a = GM/r^2.

Object position is updated using the Euler method:

v_new = v + a * dt,

r_new = r + v_new * dt.

Note: While Euler integration is simple, it’s sufficient for this MVP. Future versions will use more stable methods (e.g., Velocity Verlet) for orbital motion.

## Future plans
- Add support for initial velocity (to enable orbital trajectories),
- Implement a graphical user interface (Qt),
- Switch to more complex laws of physics,

... and much more!

___

# ru
## Симуляция взаимодействия тел через гравитацию.
Это программа для расчёта взаимодействий тел (по задумке -- космических, но можно задать любые параметры) через гравитацию.

## Текущее состояние программы
Сейчас программа имеет только **консольный вид**. Программа считает передвижения аж меньше размера протона.
Уже сейчас **эта программа считает правильнее онлайн-калькуляторов**, которые в большинстве своём делают расчёты только по гравитационной постоянной Земли / какой-л. другой планеты, 
не учитывая расстояние между объектами. 
Так, при расчёте длительности падения объекта на высоте 1000 км от поверхности Земли другие программы показывают время в 450 секунд (примерно), 
в то время как моя программа -- 510, т.к. на большом расстоянии на тело будет слабее действовать сила тяжести Земли и ускорение набираться будет медленнее.
Как итог, при расстоянии в 1000 км между объектом, совпадающим с Землёй, и материальной точкой разница между этим и другими калькуляторами составляет более 10%, что очень существенно.

## Физика
Расчёты представляют собой дифференциирование по времени (шаг задаётся в main.py), ускорение вычисляется по формуле:

a = GM/r^2.

Положение тела вычисляется методом Эйлера:

v_new = v + a * dt,

r_new = r + v_new * dt.

Замечание: метод Эйлера достаточен для MVP; в будущем он будет заменён более сложными моделями для орбитального движения.

## Планы
В будущем планируется:
- добавить поддержку задачи изначальной траектории движения (чтобы тело могло двигаться по орбите),
- добавить графический интерфейс,
- перейти на более сложную физику,

и многое другое :)

___
### Detailed update plan
**For now:**
- Check current physics between several celestial bodies; fix if needed.

**Near updates:**
- Enable setting the objects during runtime.
- Make GUI with text-style interface.
- Load basic objects (like Earth, Moon, Sun, etc.).

**Big updates:**
- Make GUI with objects displayed.
- Switch to more accurate physics.
