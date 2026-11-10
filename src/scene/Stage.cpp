#include "concert/Scene.hpp"

namespace concert {

void Stage::draw(Renderer& r, const SceneState&) const {
    // Main floor
    r.box(
        {0, -0.1f, 8},
        {40, 0.2f, 40},
        {0.065f, 0.085f, 0.11f},
        true
    );

    // Main stage platform
    r.box(
        config::stageCenter,
        config::stageSize,
        {0.11f, 0.14f, 0.18f}
    );

    // Front edge of the stage
    r.box(
        {0, 0.85f, 4.02f},
        {16, 0.08f, 0.04f},
        {0.1f, 0.7f, 0.85f},
        false,
        0.7f
    );

    // Raised rear platform
    r.box(
        {0, 1.0f, -2.5f},
        {14.0f, 0.25f, 1.5f},
        {0.08f, 0.10f, 0.14f}
    );

    // Lighting trusses
    for (float z : {-3.0f, 3.0f}) {

        // Upper horizontal truss
        r.box(
            {0, 7.3f, z},
            {16, 0.25f, 0.25f},
            {0.22f, 0.25f, 0.30f}
        );

        // Vertical supports
        for (float x : {-7.8f, 7.8f}) {
            r.box(
                {x, 4.1f, z},
                {0.25f, 6.2f, 0.25f},
                {0.22f, 0.25f, 0.30f}
            );
        }

        // Lower horizontal support
        r.box(
            {0, 1.2f, z},
            {15.6f, 0.18f, 0.18f},
            {0.18f, 0.20f, 0.24f}
        );
    }

    // Front lighting bar
    r.box(
        {0, 5.8f, 3.2f},
        {14.5f, 0.18f, 0.18f},
        {0.25f, 0.28f, 0.32f}
    );

    // =========================================================
    // SPEAKERS
    // =========================================================

    // Left main speaker
    r.box(
        {-9.0f, 1.8f, 2.5f},
        {1.4f, 2.8f, 1.2f},
        {0.035f, 0.04f, 0.05f}
    );

    // Left speaker front panel
    r.box(
        {-9.0f, 1.8f, 1.88f},
        {1.0f, 2.2f, 0.08f},
        {0.12f, 0.14f, 0.17f},
        false,
        0.15f
    );

    // Right main speaker
    r.box(
        {9.0f, 1.8f, 2.5f},
        {1.4f, 2.8f, 1.2f},
        {0.035f, 0.04f, 0.05f}
    );

    // Right speaker front panel
    r.box(
        {9.0f, 1.8f, 1.88f},
        {1.0f, 2.2f, 0.08f},
        {0.12f, 0.14f, 0.17f},
        false,
        0.15f
    );

    // =========================================================
    // STAGE EQUIPMENT
    // =========================================================

    // Amplifier - left
    r.box(
        {-5.5f, 1.35f, -2.0f},
        {1.5f, 0.9f, 1.0f},
        {0.07f, 0.08f, 0.10f}
    );

    // Amplifier - right
    r.box(
        {5.5f, 1.35f, -2.0f},
        {1.5f, 0.9f, 1.0f},
        {0.07f, 0.08f, 0.10f}
    );

    // Equipment rack
    r.box(
        {6.0f, 2.0f, -1.0f},
        {1.2f, 2.0f, 0.9f},
        {0.05f, 0.06f, 0.08f}
    );

    // Equipment rack front panel
    r.box(
        {6.0f, 2.0f, -0.54f},
        {0.9f, 1.5f, 0.06f},
        {0.16f, 0.18f, 0.22f},
        false,
        0.1f
    );

    // Monitor speaker on stage - left
    r.box(
        {-3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );

    // Monitor speaker on stage - right
    r.box(
        {3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );
}

} // namespace concert