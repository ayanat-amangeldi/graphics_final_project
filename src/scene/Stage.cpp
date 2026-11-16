#include "concert/Scene.hpp"
#include "concert/SingerPose.hpp"

namespace concert {

namespace {
void drawVenue(Renderer& r) {
    constexpr Vec3 wall{0.035f, 0.045f, 0.095f};
    constexpr Vec3 panel{0.055f, 0.067f, 0.14f};
    constexpr Vec3 violet{0.32f, 0.13f, 0.85f};
    constexpr Vec3 cyan{0.08f, 0.65f, 0.95f};

    r.box({0,-0.12f,8},{40,0.24f,40},{0.042f,0.053f,0.086f},true);
    r.box({0,5,-5.25f},{26,10,0.35f},wall,false,0.12f);
    r.box({0,10.08f,10},{26,0.24f,31},{0.022f,0.028f,0.059f},false,0.1f);
    for (float side : {-1.0f, 1.0f}) {
        r.box({side*8,9.93f,9},{0.052f,0.035f,24},violet,false,0.24f);
        r.box({side*13,5,10},{0.35f,10,30},wall,false,0.08f);
        for (int i=0; i<7; ++i) {
            const float x=side*(5.7f+i*0.94f);
            const float height=3.1f+0.64f*i;
            r.box({x,5,-4.95f},{0.69f,7.4f,0.24f},panel,false,0.16f);
            r.box({x,4.7f,-4.79f},{0.055f,height,0.06f},
                  i%2==0?violet:cyan,false,0.48f);
        }
        for (float z : {-2.0f, 4.0f, 10.0f, 16.0f}) {
            r.box({side*12.78f,4.5f,z},{0.15f,8,0.65f},panel,false,0.13f);
            r.box({side*12.66f,6,z},{0.08f,3.2f,0.045f},violet,false,0.36f);
        }
        r.box({side*11.1f,0.015f,11},{0.05f,0.02f,17},cyan,false,0.33f);
    }
    r.box({0,9,-4.86f},{25,0.055f,0.08f},violet,false,0.50f);
    r.box({0,4,-4.1f},{10.52f,4.53f,0.43f},panel,false,0.13f);
    for (float side : {-1.0f, 1.0f}) {
        r.box({side*5.17f,4,-3.83f},{0.055f,4.40f,0.09f},cyan,false,0.70f);
        r.box({0,4+side*2.18f,-3.83f},{10.4f,0.055f,0.09f},violet,false,0.63f);
    }
}

void drawSinger(Renderer& r, const SingerPose& pose) {
    constexpr Vec3 jacket{0.55f,0.11f,0.22f}, trim{0.70f,0.46f,0.22f};
    constexpr Vec3 skin{0.72f,0.48f,0.32f}, trousers{0.065f,0.067f,0.10f};
    const auto box = [&](const Mat4& parent, Vec3 centre, Vec3 size, Vec3 color) {
        r.box(parent * translation(centre) * scaling(size), color);
    };
    const Mat4& torso = pose.upperBody;
    box(torso, {0,1.10f,0}, {0.64f,0.68f,0.39f}, jacket);
    box(torso, {0,1.12f,0.204f}, {0.17f,0.55f,0.025f}, trim);
    box(torso, {0,1.69f,0}, {0.43f,0.46f,0.42f}, skin);
    box(torso, {0,1.92f,-0.023f}, {0.445f,0.14f,0.43f}, {0.07f,0.034f,0.027f});

    for (float side : {-1.0f, 1.0f}) {
        const float swing = side * std::sin(pose.gaitPhase) * pose.stride;
        const float lift = std::max(0.0f, side * std::cos(pose.gaitPhase)) * pose.stride;
        const Mat4 thigh = pose.root * translation({side * 0.165f,0.78f,0})
                           * rotationX(-0.56f * swing);
        const Mat4 shin = thigh * translation({0,-0.36f,0}) * rotationX(0.46f * lift);
        const Vec3 ankle = transformPoint(shin, {0,-0.36f,0});
        box(thigh, {0,-0.18f,0}, {0.25f,0.38f,0.27f}, trousers);
        box(shin, {0,-0.18f,0}, {0.20f,0.38f,0.22f}, trousers);
        box(translation(ankle) * rotationY(std::atan2(pose.root.m[8],pose.root.m[10])),
            {0,-0.012f,0.06f}, {0.235f,0.094f,0.32f}, {0.031f,0.034f,0.055f});
    }

    const float armSwing = 0.42f * std::sin(pose.gaitPhase) * pose.stride;
    const Mat4 shoulder = torso * translation({-0.38f,1.39f,0}) * rotationX(armSwing);
    const Mat4 elbow = shoulder * translation({0,-0.28f,0}) * rotationX(-0.13f);
    box(shoulder, {0,-0.13f,0}, {0.18f,0.29f,0.20f}, jacket);
    box(elbow, {0,-0.12f,0}, {0.16f,0.26f,0.18f}, jacket);
    box(elbow, {0,-0.27f,0}, {0.15f,0.15f,0.16f}, skin);

    const Mat4 micShoulder = torso * translation({0.40f,1.39f,0}) * rotationX(-0.87f);
    const Mat4 micElbow = micShoulder * translation({0,-0.26f,0}) * rotationX(-1.36f);
    box(micShoulder, {0,-0.12f,0}, {0.18f,0.28f,0.20f}, jacket);
    box(micElbow, {-0.08f,-0.12f,0}, {0.27f,0.25f,0.16f}, jacket);
    const Mat4 hand = micElbow * translation({-0.23f,-0.25f,0});
    box(hand, {0,0,0}, {0.16f,0.15f,0.16f}, skin);
    box(hand, {0,-0.072f,0}, {0.072f,0.20f,0.075f}, {0.064f,0.066f,0.087f});
    box(hand, {0,-0.193f,0}, {0.11f,0.10f,0.11f}, {0.30f,0.32f,0.38f});
}

void drawSpeakerDetails(Renderer& r) {
    for (float x : {-9.0f, 9.0f}) {
        r.box({x,1.8f,3.13f},{1.18f,2.46f,0.06f},{0.065f,0.09f,0.16f},false,0.18f);
        for (float y : {1.32f,2.28f}) {
            r.box({x,y,3.17f},{0.83f,0.78f,0.06f},{0.035f,0.045f,0.08f},false,0.10f);
            r.box({x,y,3.22f},{0.49f,0.46f,0.06f},{0.09f,0.12f,0.22f},false,0.20f);
        }
        r.box({x,2.97f,3.17f},{0.45f,0.035f,0.05f},{0.10f,0.60f,0.95f},false,0.85f);
    }
}
}

void Stage::draw(Renderer& r, const SceneState& state) const {
    drawVenue(r);

    r.box(
        config::stageCenter,
        config::stageSize,
        {0.11f, 0.14f, 0.18f}
    );

    r.box(
        {0, 0.85f, 4.02f},
        {16, 0.08f, 0.04f},
        {0.1f, 0.7f, 0.85f},
        false,
        0.7f
    );

    r.box(
        {0, 1.0f, -2.5f},
        {14.0f, 0.25f, 1.5f},
        {0.08f, 0.10f, 0.14f}
    );

    for (float z : {-3.0f, 3.0f}) {

        r.box(
            {0, 7.3f, z},
            {16, 0.25f, 0.25f},
            {0.095f, 0.135f, 0.23f}, false, 0.22f
        );

        for (float x : {-7.8f, 7.8f}) {
            r.box(
                {x, 4.1f, z},
                {0.25f, 6.2f, 0.25f},
                {0.095f, 0.135f, 0.23f}, false, 0.22f
            );
        }

        r.box(
            {0, 1.2f, z},
            {15.6f, 0.18f, 0.18f},
            {0.18f, 0.20f, 0.24f}
        );
    }

    constexpr Vec3 steel{0.10f,0.16f,0.27f};
    r.box({0,7.68f,3},{16,0.11f,0.14f},steel,false,0.19f);
    for (int i=0; i<16; ++i) {
        const float x=-7.5f+i;
        const float angle=std::atan2(i%2==0 ? 0.35f : -0.35f,1.0f);
        r.box(translation({x,7.49f,3}) * rotationZ(angle) * scaling({1.08f,0.065f,0.09f}),
              steel,false,0.19f);
    }

    r.box(
        {-9.0f, 1.8f, 2.5f},
        {1.4f, 2.8f, 1.2f},
        {0.035f, 0.04f, 0.05f}
    );

    r.box(
        {9.0f, 1.8f, 2.5f},
        {1.4f, 2.8f, 1.2f},
        {0.035f, 0.04f, 0.05f}
    );

    drawSpeakerDetails(r);

    r.box(
        {-5.5f, 1.35f, -2.0f},
        {1.5f, 0.9f, 1.0f},
        {0.07f, 0.08f, 0.10f}
    );

    r.box(
        {5.5f, 1.35f, -2.0f},
        {1.5f, 0.9f, 1.0f},
        {0.07f, 0.08f, 0.10f}
    );

    r.box(
        {6.0f, 2.0f, -1.0f},
        {1.2f, 2.0f, 0.9f},
        {0.05f, 0.06f, 0.08f}
    );

    r.box(
        {6.0f, 2.0f, -0.54f},
        {0.9f, 1.5f, 0.06f},
        {0.16f, 0.18f, 0.22f},
        false,
        0.1f
    );

    r.box(
        {-3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );

    r.box(
        {3.5f, 1.15f, 2.0f},
        {1.0f, 0.5f, 0.8f},
        {0.05f, 0.06f, 0.08f}
    );

    drawSinger(r, singerPose(state.animationTime));

    Vec3 guitaristFeet = {-4.0f, 1.0f, -0.5f};

    r.box(
        guitaristFeet + Vec3{0, 0.8f, 0},
        {0.75f, 1.2f, 0.5f},
        {0.12f, 0.35f, 0.72f}
    );

    r.box(
        guitaristFeet + Vec3{0, 1.65f, 0},
        {0.5f, 0.55f, 0.5f},
        {0.68f, 0.43f, 0.28f}
    );

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

    r.box(
        guitaristFeet + Vec3{0.45f, 0.75f, 0.35f},
        {0.7f, 0.8f, 0.22f},
        {0.55f, 0.20f, 0.06f}
    );

    r.box(
        guitaristFeet + Vec3{0.45f, 1.35f, 0.35f},
        {0.15f, 1.2f, 0.12f},
        {0.30f, 0.16f, 0.07f}
    );

    r.box(
        guitaristFeet + Vec3{0.45f, 1.95f, 0.35f},
        {0.25f, 0.3f, 0.15f},
        {0.22f, 0.12f, 0.05f}
    );

    Vec3 drummerFeet = {4.0f, 1.0f, -1.2f};

    r.box(
        drummerFeet + Vec3{0, 0.8f, 0},
        {0.75f, 1.2f, 0.5f},
        {0.18f, 0.55f, 0.30f}
    );

    r.box(
        drummerFeet + Vec3{0, 1.65f, 0},
        {0.5f, 0.55f, 0.5f},
        {0.68f, 0.43f, 0.28f}
    );

    r.box(
        {4.0f, 1.05f, -0.7f},
        {3.0f, 0.12f, 1.8f},
        {0.09f, 0.09f, 0.11f}
    );

    r.box(
        {4.0f, 1.45f, -0.45f},
        {1.2f, 0.8f, 0.8f},
        {0.48f, 0.08f, 0.08f}
    );

    r.box(
        {3.2f, 1.75f, -0.45f},
        {0.55f, 0.55f, 0.55f},
        {0.55f, 0.12f, 0.10f}
    );

    r.box(
        {4.8f, 1.75f, -0.45f},
        {0.55f, 0.55f, 0.55f},
        {0.55f, 0.12f, 0.10f}
    );

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

}