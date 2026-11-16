#include "concert/Music.hpp"
#include "concert/Math.hpp"
#include <atomic>
#include <cstdint>
#include <iostream>
#include <vector>

#define MA_NO_FLAC
#define MA_NO_ENCODING
#define MA_NO_ENGINE
#define MA_NO_NULL
#define MINIAUDIO_IMPLEMENTATION
#include "../third_party/miniaudio/miniaudio.h"

namespace concert {
namespace {
constexpr std::uint32_t sampleRate = 48000;
constexpr std::uint32_t samplesPerBeat = sampleRate * 60 / 120;
constexpr int loopBeats = 32;
constexpr float tau = 2 * pi;

float frequency(int midi) {
    return 440.0f * std::exp2((midi - 69) / 12.0f);
}

float envelope(float time, float attack, float decay, float duration) {
    const float onset = std::min(time / attack, 1.0f);
    const float release = std::clamp((duration - time) / 0.025f, 0.0f, 1.0f);
    return onset * std::exp(-time * decay) * release;
}

std::vector<float> makeLoop() {
    std::vector<float> samples(samplesPerBeat * loopBeats);
    constexpr int bassNotes[] = {38, 34, 41, 36};
    constexpr int chords[][4] = {{62,65,69,72}, {58,62,65,69}, {60,65,69,72}, {60,64,67,74}};
    std::uint32_t noiseState = 0x1329D5F7u;
    float previousNoise = 0;
    for (std::uint32_t i = 0; i < samples.size(); ++i) {
        const int beat = i / samplesPerBeat;
        const int chord = (beat / 4) % 4;
        const float beatTime = float(i % samplesPerBeat) / sampleRate;
        const int eighth = beatTime >= 0.25f ? 1 : 0;
        const float eighthTime = beatTime - eighth * 0.25f;

        noiseState = noiseState * 1664525u + 1013904223u;
        const float noise = float(noiseState >> 8) / 8388608.0f - 1.0f;
        const float highNoise = (noise - previousNoise) * 0.5f;
        previousNoise = noise;

        // Four-on-the-floor kick, clap on beats two and four, offbeat hi-hat.
        const float kickPhase = tau * (48 * beatTime + 2.7f * (1 - std::exp(-39 * beatTime)));
        const float kick = 0.46f * std::sin(kickPhase) * envelope(beatTime, 0.003f, 10, 0.34f);
        const float clap = beat % 2 == 1
            ? 0.21f * (0.65f * noise + 0.35f * highNoise)
                  * envelope(beatTime, 0.002f, 25, 0.15f)
            : 0.0f;
        const float hat = (eighth ? 0.16f : 0.064f) * highNoise
                          * envelope(eighthTime, 0.002f, 38, 0.10f);

        const float bassPhase = tau * frequency(bassNotes[chord] + (beat % 4 == 3 ? 12 : 0)) * eighthTime;
        const float bass = 0.20f * (std::sin(bassPhase) + 0.21f * std::sin(2 * bassPhase))
                           * envelope(eighthTime, 0.009f, 7, 0.24f);
        const int note = ((beat % 4) * 2 + eighth + (beat / 16)) % 4;
        const float leadPhase = tau * frequency(chords[chord][note]) * eighthTime;
        const float pluck = 0.13f * (std::sin(leadPhase) + 0.20f * std::sin(2 * leadPhase))
                            * envelope(eighthTime, 0.006f, 10, 0.24f);
        samples[i] = std::clamp(kick + clap + hat + bass + pluck, -0.94f, 0.94f);
    }
    return samples;
}

std::vector<float> loadTrack(const MusicTrack& track) {
    auto config = ma_decoder_config_init(ma_format_f32, 1, sampleRate);
    void* decoded = nullptr;
    ma_uint64 frameCount = 0;
    if (ma_decode_file(track.file.u8string().c_str(), &config, &frameCount, &decoded) != MA_SUCCESS)
        throw std::runtime_error("Cannot decode music file. Use a WAV or MP3: " + track.file.string());
    const auto freeAudio = [](void* data) { ma_free(data, nullptr); };
    const std::unique_ptr<void, decltype(freeAudio)> audio(decoded, freeAudio);
    if (track.startSeconds >= static_cast<double>(frameCount) / sampleRate)
        throw std::runtime_error("Music start is outside the audio file.");
    const auto start = static_cast<ma_uint64>(std::llround(track.startSeconds * sampleRate));
    const double framesPerBeat = sampleRate * 60.0 / track.bpm;
    const auto beatCount = std::floor((frameCount - start) / framesPerBeat);
    const auto length = static_cast<ma_uint64>(std::llround(beatCount * framesPerBeat));
    if (length < sampleRate / 5 || length > UINT32_MAX)
        throw std::runtime_error("Invalid music segment length.");

    // Loop over a whole number of beats and soften the splice at both ends.
    const auto* pcm = static_cast<const float*>(audio.get());
    std::vector<float> samples(pcm + start, pcm + start + length);
    constexpr std::size_t fade = sampleRate / 40;
    for (std::size_t i = 0; i < fade; ++i) {
        const float gain = static_cast<float>(i) / fade;
        samples[i] *= gain;
        samples[samples.size() - 1 - i] *= gain;
    }
    return samples;
}
}

struct Music::Playback {
    ma_device device{};
    std::vector<float> samples;
    std::atomic<std::uint32_t> cursor{0};
    std::atomic<bool> paused{false}, muted{false};
    float gain = 0;
    bool ready = false;

    explicit Playback(const MusicTrack& track)
        : samples(track.file.empty() ? makeLoop() : loadTrack(track)) {
        ma_device_config config = ma_device_config_init(ma_device_type_playback);
        config.playback.format = ma_format_f32;
        config.playback.channels = 1;
        config.sampleRate = sampleRate;
        config.periodSizeInMilliseconds = 10;
        config.periods = 2;
        config.pUserData = this;
        config.dataCallback = renderAudio;
        const auto initialized = ma_device_init(nullptr, &config, &device);
        if (initialized != MA_SUCCESS) return;
        ready = ma_device_start(&device) == MA_SUCCESS;
        if (!ready) ma_device_uninit(&device);
    }
    ~Playback() {
        if (ready) ma_device_uninit(&device);
    }

    static void renderAudio(ma_device* device, void* output, const void*, ma_uint32 frames) {
        auto& playback = *static_cast<Playback*>(device->pUserData);
        auto* pcm = static_cast<float*>(output);
        if (playback.paused.load(std::memory_order_relaxed)) {
            std::fill_n(pcm, frames, 0.0f);
            playback.gain = 0;
            return;
        }
        const float target = playback.muted.load(std::memory_order_relaxed) ? 0.0f : 0.70f;
        std::uint32_t frame = playback.cursor.load(std::memory_order_relaxed);
        for (ma_uint32 i = 0; i < frames; ++i) {
            playback.gain += (target - playback.gain) * 0.004f;
            pcm[i] = playback.samples[frame] * playback.gain;
            if (++frame == playback.samples.size()) frame = 0;
        }
        playback.cursor.store(frame, std::memory_order_relaxed);
    }
};

Music::Music(const MusicTrack& track) : playback_(std::make_unique<Playback>(track)), track_(track) {
    std::cout << "Music: " << (available() ? "audio playback active" : "audio unavailable, visual beat active")
              << " (" << track_.bpm << " BPM)\n";
    if (!track_.file.empty()) {
        std::cout << "Music track: " << track_.file.filename().string()
                  << " | start: " << track_.startSeconds << " s"
                  << " | first beat offset: " << track_.firstBeatOffset << " s\n";
    }
}
Music::~Music() = default;
bool Music::available() const { return playback_->ready; }

void Music::update(bool paused, bool muted, double elapsed) {
    playback_->paused.store(paused, std::memory_order_relaxed);
    playback_->muted.store(muted, std::memory_order_relaxed);
    if (!available() && !paused)
        silentSeconds_ += elapsed;
}

float Music::beatPulse() const {
    const double seconds = available()
        ? double(playback_->cursor.load(std::memory_order_relaxed)) / sampleRate
        : silentSeconds_;
    const double beats = (seconds - track_.firstBeatOffset) * track_.bpm / 60.0;
    const float phase = float(beats - std::floor(beats));
    const float envelope = std::max(0.0f, 1.0f - phase / 0.64f);
    return envelope * envelope;
}
}
