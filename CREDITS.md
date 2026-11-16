# Credits

## Included library

- **GLFW 3.4**, GLFW contributors. Official source: https://github.com/glfw/glfw/tree/3.4
  - Commit: `7b6aead9fb88b3623e3b3725ebb42670cbe4c579`.
  - License: zlib/libpng, retained verbatim in `third_party/glfw/LICENSE.md`.
  - Source is included unchanged apart from omitting repository metadata and GitHub configuration. It is built by CMake with documentation, examples and tests disabled. No binary dependency is committed.

- **miniaudio 0.11.23**, David Reid (mackron), [upstream source](https://github.com/mackron/miniaudio/tree/0.11.23). We use the MIT No Attribution option in the included `third_party/miniaudio/LICENSE`. The header is included unchanged. The [official playback documentation](https://miniaud.io/docs/manual/index.html) informed use of the device callback.
- **120 BPM instrumental loop**. Original synthesized kick, clap, hi-hat, bass, and pluck parts generated at runtime by project code in `src/Music.cpp`.

## Project code and visuals

- The [AI use note](README.md#credits-and-ai-use) documents assistance with the project code, assets, and documentation.
- Current scene uses procedurally constructed unit cubes. There are no downloaded models, textures, or external fonts in the application.
- The course's Assignment 3 informed our macOS compiler and GLFW setup.

## Technical documentation consulted

- GLFW, Building applications: https://www.glfw.org/docs/latest/build.html
- GLFW, Compiling GLFW: https://www.glfw.org/docs/3.3/compile_guide.html
- Microsoft, CMake Tools quick start: https://code.visualstudio.com/docs/cpp/cmake-quickstart
- Microsoft, C/C++ for Visual Studio Code: https://code.visualstudio.com/docs/languages/cpp

The technical references document the tools used in the project.
