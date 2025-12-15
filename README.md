# Note
I'm resting. I will continue working on this project in January 2026 ;)

# en
## Gravity-based N-Body Simulation (alpha)
This program simulates gravitational interactions between bodies (originally intended for celestial objects, but any parameters can be used).

## Current state
Currently, the program is GUI console-style for 2D space only.

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

Note: it’s sufficient for this MVP. Future versions will use more stable methods (e.g., Velocity Verlet).

## Future plans
[See here](https://github.com/users/dzh-a-v/projects/5) detailed plan.

<!--
Roadmap:
<img width="1243" height="571" alt="image" src="https://github.com/user-attachments/assets/8aca740c-838f-4aef-97f5-862ade6191da" />
-->

## License

Copyright © 2025 Arsen Dzhabrailov (dzh-a-v)
Licensed under **GNU GPL v3.0** — see [LICENSE](LICENSE).
___

# ru
## Симуляция взаимодействия тел через гравитацию.
Это программа для расчёта взаимодействий тел (по задумке -- космических, но можно задать любые параметры) через гравитацию.

## Текущее состояние программы
Сейчас программа имеет простой интерфейс консольного стиля для 2-мерного пространства.

Уже сейчас эта программа считает правильнее онлайн-калькуляторов, которые в большинстве своём делают расчёты только по гравитационной постоянной Земли / какой-л. другой планеты, 
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

Замечание: это достаточно для MVP; в будущем он будет заменён более сложными моделями.

## Планы
[См. здесь](https://github.com/users/dzh-a-v/projects/5) детальный план.

<!--
Дорожная карта:
<img width="1243" height="571" alt="image" src="https://github.com/user-attachments/assets/8aca740c-838f-4aef-97f5-862ade6191da" />
-->

## Лицензия

Copyright © 2025 Arsen Dzhabrailov (dzh-a-v)
Licensed under **GNU GPL v3.0** — see [LICENSE](LICENSE).
