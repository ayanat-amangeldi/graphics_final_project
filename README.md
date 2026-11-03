# Concert Stage

Общий стартовый каркас финального проекта Computer Graphics Fundamentals для команды из четырёх человек. C++17, OpenGL 3.3 core, GLFW и собственные GLSL-шейдеры. Репозиторий: https://github.com/ayanat-amangeldi/graphics_final_project

Сейчас это запускаемая основа для совместной работы. Прямоугольные фигуры обозначают будущие объекты. Полноценное световое шоу, модели группы, instancing толпы, конфетти и LED-анимация ещё не реализованы.

## Что уже есть

- Окно 1280x720, depth test, back-face culling, VAO/VBO/EBO.
- Индексированный куб с нормалями и UV, буферы геометрии загружаются один раз.
- Шейдеры в отдельных `.vert` / `.frag`; ошибки компиляции и линковки выводятся в терминал.
- Model / view / projection, правильная normal matrix для неравномерного масштаба.
- Два нейтральных точечных источника и базовый Blinn-Phong для проверки геометрии.
- Свободная камера, управление с учётом deltaTime, безопасная потеря фокуса окна.
- Единый ввод, общая пауза, состояние пресетов и запросы будущих эффектов.
- FPS в заголовке окна и подсказки внутри окна.
- Подиум, фермы, маркеры музыкантов, шесть корпусов светильников, три маркера зрителей и статичный экран.
- Проверочная иерархия вращения корпуса светильника относительно крепления.
- CMake для macOS/Windows, macOS Makefile и VS Code tasks.

## Как открыть проект

Если репозиторий ещё не клонирован:

```sh
git clone https://github.com/ayanat-amangeldi/graphics_final_project.git
cd graphics_final_project
```

В VS Code выберите File → Open Folder и откройте **саму папку graphics_final_project**, где лежит этот README. Если локальная копия уже есть, повторно клонировать её не нужно. Установите рекомендованные расширения Microsoft C/C++ и CMake Tools. Расширения не заменяют компилятор и CMake.

## macOS

Нужны Xcode Command Line Tools. Если они ещё не установлены:

```sh
xcode-select --install
```

### Вариант 1 — как в Assignment 3

Если GLFW и pkg-config уже использовались в заданиях:

```sh
make
make test
make run
```

Если отсутствуют GLFW/pkg-config, при наличии Homebrew:

```sh
brew install glfw pkg-config
```

Этот вариант создаёт `build/make/concert_stage` и использует системный GLFW.

### Вариант 2 — общая CMake-сборка

Нужен CMake 3.16+ (`brew install cmake`, если используете Homebrew). GLFW уже включён в репозиторий и собирается автоматически:

```sh
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --config Release --parallel
ctest --test-dir build/cmake -C Release --output-on-failure
./build/cmake/concert_stage
```

## Windows

Рекомендуемая конфигурация: Visual Studio Build Tools **2022** с workload **Desktop development with C++**, Windows SDK и CMake 3.21+. Писать код продолжаете в VS Code; Build Tools предоставляет компилятор MSVC. Если уже есть рабочая настройка курса с другим компилятором, согласуйте её перед установкой второго toolchain.

Перезапустите VS Code после установки инструментов. В его терминале PowerShell, из корня репозитория:

```powershell
cmake -S . -B build/cmake -G "Visual Studio 17 2022" -A x64
cmake --build build/cmake --config Release --parallel
ctest --test-dir build/cmake -C Release --output-on-failure
.\build\cmake\Release\concert_stage.exe
```

Отдельно скачивать GLFW, GLAD, DLL или настраивать include-пути не нужно: GLFW собирается статически из `third_party/glfw`, функции OpenGL загружает небольшой модуль проекта через GLFW. Нужен видеодрайвер с поддержкой OpenGL 3.3+; если окно не создаётся, сохраните сообщение из терминала.

## VS Code

- Terminal → Run Build Task запускает `Concert: build` через CMake.
- Terminal → Run Task → `Concert: test maths` проверяет матрицы и координаты.
- На Mac с настройкой A3 можно выбрать `Concert: make on macOS`.
- При использовании CMake Tools выберите компилятор через `CMake: Select a Kit`, затем `CMake: Configure`, `CMake: Build` и `CMake: Run Without Debugging` для target `concert_stage`.
- Не запускайте проект кнопкой Code Runner / Run Code: компилировать только `main.cpp` недостаточно.
- Если меняете компилятор или генератор, используйте новую build-папку или `CMake: Delete Cache and Reconfigure`.

## Управление

При обычном запуске мышь захвачена. Tab освобождает её и снова включает управление. При потере фокуса окна курсор освобождается; нажмите Tab после возвращения.

| Клавиша | Сейчас работает |
| --- | --- |
| WASD, мышь | Перемещение и обзор |
| Q / E | Вниз / вверх |
| Shift | Быстрое движение |
| Tab | Захват / освобождение мыши |
| R | Начальная позиция камеры |
| 1 / 2 / 3 | Выбор пресета в SceneState и цвет корпусов-маркеров |
| Удержание Space | `focusSinger=true`; настоящее наведение света ещё предстоит сделать |
| C | Один запрос конфетти на нажатие, сообщение в терминале; частиц пока нет |
| L | Переключение `ledAnimated`; экран пока остаётся статичным |
| P | Пауза времени сцены и вращения маркеров; камера работает |
| H | Показать / скрыть подробные подсказки |
| Esc | Закрыть окно |

Финальное поведение зарезервированных клавиш описано в [договорённостях команды](docs/TEAM_CONTRACT.md).

## Проверка перед началом работы

Каждый проходит [FIRST_RUN_CHECKLIST](docs/FIRST_RUN_CHECKLIST.md) на своём компьютере. Наличие общей CMake-конфигурации не заменяет проверку Windows.

Для автоматической проверки контекста, шейдеров и отрисовки запустите программу с `--smoke-test`. Она нарисует минимум 120 кадров, проработает минимум секунду и закроется сама:

```sh
./build/make/concert_stage --smoke-test
```

Для CMake замените путь на свой executable; на Windows это `.\build\cmake\Release\concert_stage.exe --smoke-test`.

Дополнительный параметр `--screenshot build/preview.ppm` сохраняет последний кадр smoke test в PPM. Это средство проверки, а не замер финальной производительности. Запускайте тест в обычной графической сессии, с неминимизированным окном.

Шейдеры копируются рядом с исполняемым файлом при сборке. После изменения шейдеров снова выполните build. Параметр `--shader-dir PATH` позволяет явно выбрать другую папку шейдеров. Если переносите executable, переносите рядом и папку `shaders`.

## Распределение работы

| Участник | Модули | Дальнейшая задача |
| --- | --- | --- |
| Участник 1 — автор каркаса, вписать имя | `src/scene/LightShow.cpp`, освещение в Renderer и scene.frag | Настоящие spotlights, иерархия, пресеты, наведение на певца |
| Участник 2 — вписать имя | `src/scene/Stage.cpp` | Сцена, музыканты, инструменты и фермы |
| Участник 3 — вписать имя | `src/scene/Crowd.cpp` | Instancing 120 зрителей и конфетти |
| Участник 4 — вписать имя | `src/scene/LedScreen.cpp`, новые LED-шейдеры, Hud | Анимированный экран и развитие интерфейса, проверка камеры |

Общие заголовки и Renderer меняем по договорённости. Не создаём по отдельному `main()` и окну в каждой части.

## Совместная работа с Git

Первый каркас пока подготовлен локально. До commit проверьте список файлов: `build/` и исполняемые файлы не должны попадать в репозиторий. Для первого commit откройте Source Control в VS Code, проверьте изменения, сделайте Stage, Commit и Push под своим аккаунтом. Если Git просит имя/email, укажите собственные данные.

После публикации каркаса каждый создаёт ветку от актуального `main`, например:

```sh
git switch main
git pull --ff-only origin main
git switch -c feature/spotlights
```

Остальные ветки: `feature/stage`, `feature/crowd`, `feature/led-ui`. Коммитим свою работу регулярно со своего аккаунта. После push открываем pull request; другой участник проверяет сборку и код, затем изменения объединяются в main. Обновляйте свою feature-ветку изменениями из main и разбирайте конфликты до merge.

## AI Use Note

Общий стартовый каркас создан с помощью OpenAI Codex: структура C++-проекта, CMake/Makefile, камера и ввод, базовые шейдеры, модульные заглушки, тесты и инструкции. GLFW взят из официального репозитория и не сгенерирован AI. Команда должна проверить код, понять его и дописать собственные модули. По мере работы обновляйте эту заметку: какие инструменты использовались, для чего и что каждый участник изменил и проверил. Не заявляйте ещё не выполненные изменения и проверки.

## Источники и лицензии

См. [CREDITS.md](CREDITS.md). Документация по сборке: [GLFW](https://www.glfw.org/docs/latest/build.html), [VS Code CMake](https://code.visualstudio.com/docs/cpp/cmake-quickstart), [C++ в VS Code](https://code.visualstudio.com/docs/languages/cpp). Это технические источники; они не заменяют 15 журнальных источников для literature review.
