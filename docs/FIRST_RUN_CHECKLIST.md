# Runtime Verification

The build commands in the [README](../README.md) provide the starting point for a clean checkout. The math checks verify transforms, projection, and scene coordinates. The executable also accepts `--smoke-test` for a short OpenGL launch and render check.

## Interactive checks

| Check | Expected result |
| --- | --- |
| Initial scene | The venue, band, walking singer, six light rigs, LED screen, and audience are visible. The FPS counter updates in the HUD and window title |
| Camera | Mouse and WASD change the view, Q/E change height, Shift accelerates movement, and R restores the starting view |
| Cursor and window | Tab toggles capture. Window resizing preserves the scene proportions |
| Light presets | Keys 1–3 change beam color and movement pattern |
| Singer tracking | All six beams follow the walking singer while Space is held. The preset resumes on release |
| Confetti | C emits particles above the stage. They fall and disappear |
| LED | L freezes and resumes the animated pattern |
| Audio | The built-in loop plays on an available output device. Light brightness and audience bounce follow the beat. M mutes audio while the visual rhythm continues |
| Pause and help | P pauses the scene and audio while keeping the camera available. H toggles on-screen help |
| Rendering | A front and side view show stable geometry without visible seams, flickering pairs of surfaces, or black faces |
| Exit | Esc closes the application |

## Performance record

We record the OS, CPU/GPU, memory, build configuration, actual framebuffer resolution, soundtrack, and sample duration. At 1280 × 720, one run covers the regular show and a second covers preset changes, singer tracking, and repeated confetti. For each run, the FPS table records the minimum, mean, and maximum of the sampled readings. A Mac measurement and a grading-machine measurement are reported separately.
