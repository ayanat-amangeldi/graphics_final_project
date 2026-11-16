#pragma once
#include <filesystem>
#include <memory>

namespace concert {
struct MusicTrack {
    std::filesystem::path file;
    int bpm = 120;
    double startSeconds = 0;
    double firstBeatOffset = 0;
};

class Music {
public:
    explicit Music(const MusicTrack& track = {});
    ~Music();
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;

    bool available() const;
    void update(bool paused, bool muted, double elapsed);
    float beatPulse() const;
private:
    struct Playback;
    std::unique_ptr<Playback> playback_;
    MusicTrack track_;
    double silentSeconds_ = 0;
};
}
