#include "concert/Scene.hpp"

namespace concert {

void Stage::draw(Renderer& r, const SceneState&) const {

    // =========================================================
    // FLOOR
    // =========================================================

    r.box(
        {0, -0.1f, 8},
        {40, 0.2f, 40},
        {0.065f, 0.085f, 0.11f},
        true
    );

    // =========================================================
    // MAIN STAGE
    // =========================================================

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

    // =========================================================
    // LIGHTING TRUSSES
    // =========================================================

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

    // Left speaker front
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

    // Right speaker front
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

    // Left amplifier
    r.box(
        {-5.5f, 1.35f, -2.0f},
        {1.5f, 0.9f, 1.0f},
        {0.07f, 0.08f, 0.10f}
    );

    // Right amplifier
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

    // Equipment rack front
    r.box(
        {6.0f, 2.0f, -0.54f},
        {0.9f, 1.5f, 0.06f},
        {0.16f, 0.18f, 0.22f},
        false,
        0.1f
    );

    // Stage monitor - left
    r.box(
        {-3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );

    // Stage monitor - right
    r.box(
        {3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );

    // =========================================================
    // SINGER
    // =========================================================

    Vec3 singerFeet = config::singerFeet;

    // Singer body
    r.box(
        singerFeet + Vec3{0, 0.8f, 0},
        {0.8f, 1.2f, 0.5f},
        {0.75f, 0.12f, 0.18f}
    );

    // Singer head
    r.box(
        singerFeet + Vec3{0, 1.65f, 0},
        {0.5f, 0.55f, 0.5f},
        {0.72f, 0.48f, 0.32f}
    );

    // Singer left leg
    r.box(
        singerFeet + Vec3{-0.2f, 0.4f, 0},
        {0.25f, 0.8f, 0.25f},
        {0.05f, 0.06f, 0.08f}
    );

    // Singer right leg
    r.box(
        singerFeet + Vec3{0.2f, 0.4f, 0},
        {0.25f, 0.8f, 0.25f},
        {0.05f, 0.06f, 0.08f}
    );

    // Microphone stand
    r.box(
        singerFeet + Vec3{0.65f, 0.95f, 0.25f},
        {0.06f, 1.9f, 0.06f},
        {0.08f, 0.08f, 0.09f}
    );

    // Microphone
    r.box(
        singerFeet + Vec3{0.65f, 1.85f, 0.25f},
        {0.18f, 0.25f, 0.18f},
        {0.15f, 0.16f, 0.18f},
        false,
        0.2f
    );

    // Singer position marker
    r.box(
        singerFeet + Vec3{0, 0.03f, 0},
        {0.7f, 0.04f, 0.7f},
        {0.95f, 0.55f, 0.08f},
        false,
        0.5f
    );

    // =========================================================
    // GUITARIST
    // =========================================================

    Vec3 guitaristFeet = {-4.0f, 1.0f, -0.5f};

    // Body
    r.box(
        guitaristFeet + Vec3{0, 0.8f, 0},
        {0.75f, 1.2f, 0.5f},
        {0.12f, 0.35f, 0.72f}
    );

    // Head
    r.box(
        guitaristFeet + Vec3{0, 1.65f, 0},
        {0.5f, 0.55f, 0.5f},
        {0.68f, 0.43f, 0.28f}
    );

    // Legs
    r.box(
        guitaristFeet + Vec3{-0.2f, 0.4f, 0},
        {0.25f, 0.8f, 0.25f},
        {0.06f, 0.07f, 0.10f}
    );

    r.box(
        guitaristFeet + Vec3{0.2f, 0.4f, 0},
        {0.25f, 0.8f, 0.25f},
        {0.06f, 0.07f, 0.10f}
    );

    // Guitar body
    r.box(
        guitaristFeet + Vec3{0.45f, 0.75f, 0.35f},
        {0.7f, 0.8f, 0.22f},
        {0.55f, 0.20f, 0.06f}
    );

    // Guitar neck
    r.box(
        guitaristFeet + Vec3{0.45f, 1.35f, 0.35f},
        {0.15f, 1.2f, 0.12f},
        {0.30f, 0.16f, 0.07f}
    );

    // Guitar head
    r.box(
        guitaristFeet + Vec3{0.45f, 1.95f, 0.35f},
        {0.25f, 0.3f, 0.15f},
        {0.22f, 0.12f, 0.05f}
    );

    // =========================================================
    // DRUMMER
    // =========================================================

    Vec3 drummerFeet = {4.0f, 1.0f, -1.2f};

    // Body
    r.box(
        drummerFeet + Vec3{0, 0.8f, 0},
        {0.75f, 1.2f, 0.5f},
        {0.18f, 0.55f, 0.30f}
    );

    // Head
    r.box(
        drummerFeet + Vec3{0, 1.65f, 0},
        {0.5f, 0.55f, 0.5f},
        {0.68f, 0.43f, 0.28f}
    );

    // Drum platform
    r.box(
        {4.0f, 1.05f, -0.7f},
        {3.0f, 0.12f, 1.8f},
        {0.09f, 0.09f, 0.11f}
    );

    // Bass drum
    r.box(
        {4.0f, 1.45f, -0.45f},
        {1.2f, 0.8f, 0.8f},
        {0.48f, 0.08f, 0.08f}
    );

    // Left drum
    r.box(
        {3.2f, 1.75f, -0.45f},
        {0.55f, 0.55f, 0.55f},
        {0.55f, 0.12f, 0.10f}
    );

    // Right drum
    r.box(
        {4.8f, 1.75f, -0.45f},
        {0.55f, 0.55f, 0.55f},
        {0.55f, 0.12f, 0.10f}
    );

    // Cymbal stands
    r.box(
        {3.0f, 2.0f, -1.0f},
        {0.06f, 1.5f, 0.06f},
        {0.65f, 0.55f, 0.12f}
    );

    r.box(
        {5.0f, 2.0f, -1.0f},
        {0.06f, 1.5f, 0.06f},
        {0.65f, 0.55f, 0.12f}
    );

    // Cymbals
    r.box(
        {3.0f, 2.75f, -1.0f},
        {0.8f, 0.08f, 0.8f},
        {0.75f, 0.60f, 0.15f},
        false,
        0.15f
    );

    r.box(
        {5.0f, 2.75f, -1.0f},
        {0.8f, 0.08f, 0.8f},
        {0.75f, 0.60f, 0.15f},
        false,
        0.15f
    );

    // =========================================================
    // LIGHTING ATTACHMENT POINTS
    // =========================================================

    r.box(
        {-6.0f, 7.05f, -3.0f},
        {0.25f, 0.25f, 0.25f},
        {0.9f, 0.25f, 0.08f},
        false,
        0.5f
    );

    r.box(
        {0.0f, 7.05f, -3.0f},
        {0.25f, 0.25f, 0.25f},
        {0.9f, 0.25f, 0.08f},
        false,
        0.5f
    );

    r.box(
        {6.0f, 7.05f, -3.0f},
        {0.25f, 0.25f, 0.25f},
        {0.9f, 0.25f, 0.08f},
        false,
        0.5f
    );
}

} // namespace concert