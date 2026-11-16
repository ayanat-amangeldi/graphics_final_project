#include "concert/Scene.hpp"
#include "concert/SingerPose.hpp"

namespace concert {
namespace {
struct Preset {
    Vec3 colors[2];
    float speed, innerAngle, outerAngle, intensity;
};
constexpr Preset presets[] = {
    {{{0.12f, 0.32f, 1.0f}, {0.66f, 0.18f, 1.0f}}, 0.65f, 9, 15, 3.2f},
    {{{1.0f, 0.10f, 0.54f}, {0.06f, 0.82f, 1.0f}}, 2.15f, 7, 12, 3.6f},
    {{{1.0f, 0.62f, 0.13f}, {1.0f, 0.26f, 0.06f}}, 1.15f, 8, 14, 3.1f}
};

Vec3 sweepTarget(int preset, std::size_t index, float phase) {
    const float lane = static_cast<float>(index % 3) - 1;
    const float row = index < 3 ? -1.0f : 1.0f;
    const float angle = phase + index * pi / 3;
    switch (preset) {
        case 1: return {2.7f * lane + 1.9f * std::sin(angle), 1.0f,
                        0.2f + row * 1.4f + 0.9f * std::cos(angle)};
        case 2: return {2.1f * lane + 2.9f * std::sin(angle), 1.0f,
                        0.3f + 2.6f * std::sin(2 * angle)};
        default: return {-2.6f * lane + row * 2.4f * std::sin(phase), 1.0f,
                         -row * 1.7f + 1.2f * std::cos(angle)};
    }
}
}

void LightShow::update(const SceneState& state, float dt) {
    const Preset& preset = presets[state.preset - 1];
    phase_ = std::fmod(phase_ + dt * preset.speed, 2 * pi);

    const Vec3 singerTarget = singerPose(state.animationTime).focusPoint();
    for (std::size_t i = 0; i < lights_.size(); ++i) {
        RigPose& pose = poses_[i];
        const Vec3 anchor = config::rigAnchors[i];
        const Vec3 pivot = anchor + Vec3{0, -0.42f, 0};
        const Vec3 target = state.focusSinger ? singerTarget
                                             : sweepTarget(state.preset, i, phase_);
        const Vec3 aim = normalize(target - pivot);
        const float yaw = std::atan2(-aim.x, -aim.z);
        const float tilt = std::atan2(std::hypot(aim.x, aim.z), -aim.y);

        // Local -Y is the optical axis. The lens lies on that axis, so its
        // offset from the tilt pivot does not change the aiming direction.
        pose.mount = translation(anchor);
        pose.yoke = pose.mount * translation({0, -0.42f, 0}) * rotationY(yaw);
        pose.head = pose.yoke * rotationX(tilt);
        const Vec3 lens = transformPoint(pose.head, {0, -0.32f, 0});
        const Vec3 direction = normalize(lens - transformPoint(pose.head, {0, 0, 0}));
        const Vec3 color = state.focusSinger ? Vec3{1.0f, 0.86f, 0.67f}
                                            : preset.colors[(i + i / 3) % 2];
        const float inner = state.focusSinger ? 4.0f : preset.innerAngle;
        const float outer = state.focusSinger ? 8.0f : preset.outerAngle;
        // Keep the performer readable during focus, with a gentler pulse.
        const float pulse = state.focusSinger ? 0.91f + 0.14f * state.beatPulse
                                             : 0.63f + 0.46f * state.beatPulse;
        lights_[i] = {lens, direction, color, std::cos(radians(inner)),
                      std::cos(radians(outer)), (state.focusSinger ? 1.15f : preset.intensity) * pulse,
                      (state.focusSinger ? 0.065f : 0.18f) * pulse};
    }
}

void LightShow::configure(Renderer& renderer, const SceneState&) const {
    renderer.setSpotlights(lights_);
}

void LightShow::draw(Renderer& renderer, const SceneState&) const {
    const Vec3 bracket{0.17f, 0.20f, 0.26f};
    const Vec3 housing{0.10f, 0.13f, 0.18f};
    for (std::size_t i = 0; i < lights_.size(); ++i) {
        const RigPose& pose = poses_[i];
        renderer.box(pose.mount * translation({0, 0.09f, 0}) * scaling({0.50f, 0.20f, 0.42f}), bracket);
        renderer.box(pose.yoke * translation({0, 0.29f, 0}) * scaling({0.69f, 0.14f, 0.22f}), bracket);
        for (float side : {-1.0f, 1.0f}) {
            renderer.box(pose.yoke * translation({side * 0.31f, 0.12f, 0})
                         * scaling({0.08f, 0.38f, 0.20f}), bracket);
        }
        renderer.box(pose.head * scaling({0.51f, 0.48f, 0.44f}), housing);
        renderer.box(pose.head * translation({0, -0.26f, 0}) * scaling({0.55f, 0.10f, 0.48f}), bracket);
        renderer.box(pose.head * translation({0, -0.31f, 0}) * scaling({0.41f, 0.02f, 0.35f}),
                     lights_[i].color, false, 1.1f);
    }
}

void LightShow::drawBeams(Renderer& renderer) const {
    renderer.drawBeams(lights_);
}
}
