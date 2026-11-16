# Scene Architecture

The scene uses right-handed world coordinates. Positive X points right from the audience view, positive Y points up, and positive Z points from the stage toward the audience. [Config.hpp](../include/concert/Config.hpp) defines the stage dimensions, camera settings, fixture mounts, audience layout, and confetti emitters.

| Module | Responsibility |
| --- | --- |
| Stage | Draws the venue, equipment, and band. The singer's pose defines the torso and limb transforms |
| LightShow | Updates six fixtures and uploads their positions, directions, colors, and cone parameters. Singer focus uses the same pose calculation as the stage |
| Crowd | Supplies 120 spectator instances and updates the confetti simulation |
| LedScreen | Advances the LED animation clock when animation is enabled and submits the screen draw |
| Renderer | Owns GPU resources and renders the scene, instanced crowd and confetti, LED screen, and translucent beams |
| Music | Reproduces the loop or an optional local track and supplies the beat pulse for the crowd and lights |

Matrices are column-major and the pipeline applies projection × view × model to each vertex. A child transform is composed as parent × local. The scene shader transforms normals with the inverse transpose of the model matrix before evaluating lighting in world space.

The render loop handles GLFW input and publishes shared state to the scene. It computes camera movement from elapsed frame time. Pausing freezes the animation clock and audio progression while leaving camera movement available. The opaque scene is rendered before the translucent beams. Mesh geometry and crowd instance data are reused, while confetti instance data changes as the particles move.
