#pragma once

#include <mutex>
#include <queue>

#include <SFML/Audio.hpp>


class SamplerRecorder : public sf::SoundRecorder {
public:
    SamplerRecorder(sf::Int32 interval = 100);

    std::vector<sf::Int16> getData();

    void setDeviceSearch(const std::string &searchName);

    ~SamplerRecorder();

protected:

    bool onProcessSamples(sf::Int16 const* samples, std::size_t sampleCount) override;

    bool onStart() override;

    void onStop() override;

    std::atomic<bool> isRecording{ false };
    std::mutex mutex;
    std::condition_variable cv;
    std::vector<sf::Int16> data;
};