#pragma once

#include <mutex>
#include <queue>

#include <SFML/Audio.hpp>


class SamplerRecorder : public sf::SoundRecorder {
public:
    SamplerRecorder(sf::Int32 interval = 100);
    ~SamplerRecorder();

    void setDeviceSearch(const std::string &searchName);

    std::vector<sf::Int16> getData();

protected:

    bool onProcessSamples(sf::Int16 const* samples, std::size_t sampleCount) override;

    bool onStart() override;

    void onStop() override;

    std::atomic<bool> isRecording{ false };
    std::mutex mutex;
    std::condition_variable cv;
    std::vector<sf::Int16> data;  //New data
    std::vector<sf::Int16> pdata; //Previous data
};