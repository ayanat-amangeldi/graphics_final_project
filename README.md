# Concert Stage

Concert Stage is our real-time 3D project for Computer Graphics Fundamentals. We built a small concert venue in C++17 with OpenGL 3.3 Core and separate GLSL vertex and fragment shaders. GLFW handles the window and input, and miniaudio handles playback.

## Features

- A low-poly venue with a stage, light trusses, loudspeakers, amplifiers, a guitarist, a drummer, and a singer who walks across the stage with a handheld microphone.
- Six moving spotlights with three presets. Each light rig is built as a mount, a rotating yoke, and a tilting head. Holding Space makes all six lights track the singer.
- Per-fragment Blinn–Phong lighting, translucent light beams, a procedural floor grid, and an animated LED screen.
- A crowd of 120 spectators drawn with GPU instancing. Spectators sway, wave, and bounce with the music.
- Confetti emitted from both sides of the stage, a built-in instrumental loop at 120 BPM, and beat-driven spotlight intensity.
- A free camera, an on-screen FPS counter, and an in-window control guide.

## Build and run

The application requires an OpenGL 3.3 Core-capable graphics driver and a C++17 toolchain. The commands below run from the repository root. A clean checkout plays the built-in instrumental loop. CMake builds the bundled GLFW by default, and miniaudio is compiled into the application.

```sh
git clone https://github.com/ayanat-amangeldi/graphics_final_project.git
cd graphics_final_project
```

### macOS with Make

This is our assignment-style build with Apple Clang, GLFW, and pkg-config. Xcode Command Line Tools are required. The Homebrew command supplies GLFW and pkg-config if they are absent.

```sh
brew install glfw pkg-config
make
make test
make run
```

### macOS with CMake

This build requires Xcode Command Line Tools and CMake 3.21 or later for the commands below. With Homebrew, CMake can be installed with `brew install cmake`.

```sh
cmake -S . -B build/cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/cmake --config Release --parallel
ctest --test-dir build/cmake -C Release --output-on-failure
./build/cmake/concert_stage
```

### Windows with CMake

This build targets x64 and uses Visual Studio 2022 Build Tools with the Desktop development with C++ workload, Windows SDK, and CMake 3.21 or later. The commands are for PowerShell.

```powershell
cmake -S . -B build/cmake -G "Visual Studio 17 2022" -A x64
cmake --build build/cmake --config Release --parallel
ctest --test-dir build/cmake -C Release --output-on-failure
.\build\cmake\Release\concert_stage.exe
```

## Controls

| Input | Action |
| --- | --- |
| W / A / S / D | Move the camera |
| Mouse | Look around while the cursor is captured |
| Q / E | Lower or raise the camera |
| Shift | Move faster |
| Tab | Toggle cursor capture |
| R | Reset the camera |
| 1 / 2 / 3 | Select the slow blue and violet, fast pink and cyan, or crossed gold and orange light pattern |
| Hold Space | Aim all six spotlights at the moving singer. Releasing Space restores the selected pattern |
| C | Release confetti |
| L | Toggle LED screen animation |
| M | Mute or restore audio while the beat continues |
| P | Pause or resume the music and scene animation. The camera remains active |
| H | Show or hide control help |
| Esc | Close the application |

## Audio

The shared version uses our synthesized 120 BPM instrumental. The visual beat is derived from the audio playback position and drives both spotlight intensity and audience motion. The executable also accepts local WAV or MP3 files through `--music FILE`, with optional `--music-start SEC`, `--music-bpm BPM`, and `--beat-offset SEC` arguments.

Our local demonstration configuration uses *Danza Kuduro* by Don Omar and Lucenzo from 00:36 at 130 BPM. The recording is stored locally and excluded from Git. The GitHub version includes the synthesized loop as its default soundtrack.

## Project structure

| Path | Responsibility |
| --- | --- |
| [src/main.cpp](src/main.cpp) | OpenGL setup, input dispatch, frame timing, and the render loop |
| [include/concert](include/concert) | Interfaces for Shader, Mesh, Camera, Renderer, Scene, and Music, plus scene configuration and transform math |
| [src/scene](src/scene) | Venue, performers, spotlight rigs, audience, confetti, and LED screen |
| [shaders](shaders) | Lighting, procedural textures, beam blending, instanced effects, and HUD rendering |
| [src/Music.cpp](src/Music.cpp) | Loop synthesis, local audio decoding, playback, and beat timing |
| [tests](tests) | Checks for transforms, projection, and scene coordinates |
| [docs](docs) | Runtime verification and scene architecture |
| [third_party](third_party) | GLFW and miniaudio source and licenses |

Procedural texturing is handled by the floor and LED fragment shaders. The shader programs, indexed geometry, and camera logic have their own modules.

## Team contributions

| Contributor | Contributions |
| --- | --- |
| Ayanat Amangeldi | Shared application scaffold, build setup, spotlight rigs and singer tracking, venue polish, singer walk cycle, and music synchronization |
| Aruzhan Turaly | Stage layout, trusses, speakers, scene equipment, musicians, and instruments |
| `kerimbayzarina` | Instanced audience, spectator motion, and the confetti particle system |
| `haweeks` | Animated LED screen and HUD improvements |

## Verification

The macOS Make build, math tests, and OpenGL smoke test were verified on an Apple M4 Mac. The smoke test opens the scene, checks for OpenGL errors while rendering, and exits automatically. The [runtime checklist](docs/FIRST_RUN_CHECKLIST.md) covers manual controls and FPS measurements at 1280 × 720.

```sh
make test
make smoke
```

## Credits and AI use

[CREDITS.md](CREDITS.md) records third-party libraries, licenses, and audio provenance. We used OpenAI Codex to help prepare the shared scaffold and documentation and to assist with the spotlight and beam shaders, visual polish, singer animation, instrumental synthesis, and beat synchronization.

We wrote our own explanations of how the project works and used OpenAI Codex to suggest clearer, more academic English. We reviewed those suggestions and kept the wording that accurately described our implementation. We assembled the report ourselves in Microsoft Word and manually captured and inserted screenshots of the source code and the running application. Our use of AI for the report was limited to editing selected passages. The team prepared the complete document in Word.
