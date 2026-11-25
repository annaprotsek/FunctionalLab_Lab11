# ЛР №11. Основи функціонального програмування в C++ (C++23)

Варіант 8 (частини 1–4), частина 5 (усі 4 завдання), частина 6 — варіант 8 (аналіз гравців).

| Файл | Завдання |
|---|---|
| part1/part1_variant8.cpp | Лямбди та алгоритми: непарні елементи |
| part2/part2_variant8.cpp | Функтор Power та еквівалентна лямбда |
| part3/part3_variant8.cpp | Чиста функція: відсортована копія |
| part4/part4_variant8.cpp | Конвеєр двома способами: середнє абсолютних значень від'ємних |
| part5/task1_*.cpp | remove_if |
| part5/task2_lambda_capture.cpp | захоплення у лямбдах |
| part5/task3_broken.cpp, task3_fixed.cpp | ranges::dangling |
| part5/task4_broken.cpp, task4_fixed.cpp | передавання діапазону R&& |
| part6/part6_variant8.cpp | аналіз гравців (std::ranges) |

## Збирання (x64 Developer PowerShell)

```powershell
& "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64 -HostArch amd64
cd <папка проєкту>
cmake --preset default
cmake --build --preset default
.\run_all.ps1
```

Початкові програми, які не компілюються (завдання 3 та 4 частини 5), збираються окремо:

```powershell
cmake --build build --target task3_broken
cmake --build build --target task4_broken
```
