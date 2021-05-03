#include "SamplerRecorder.hpp"

#include <atomic>
#include <cassert>

SamplerRecorder::SamplerRecorder(sf::Int32 interval) {
    sf::SoundRecorder::setProcessingInterval(sf::milliseconds(interval));
}

SamplerRecorder::~SamplerRecorder() {
    stop();
}

void SamplerRecorder::setDeviceSearch(const std::string& searchName) {
    std::string sName = searchName;
    std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);
    sName.erase(remove_if(sName.begin(), sName.end(), ::isspace), sName.end());

    auto deviceNames = sf::SoundRecorder::getAvailableDevices();
    for (auto& deviceName : deviceNames) {
        std::string name(deviceName);
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());

        if (name.find(sName) != std::string::npos) {
            sf::SoundRecorder::setDevice(deviceName);
            return;
        }
    }
    sf::SoundRecorder::setDevice(deviceNames[0]);
}

std::vector<sf::Int16> SamplerRecorder::getData() {
     std::unique_lock<std::mutex> lock(mutex);
     cv.wait(lock, [this] { return !isRecording || !data.empty(); });
     if (isRecording) {
         assert(!data.empty());
         return data;
     }
     return std::vector<sf::Int16>();
 }



 bool SamplerRecorder::onProcessSamples(sf::Int16 const* samples, std::size_t sampleCount) {
     {
         std::lock_guard<std::mutex> lock(mutex);
         data = std::vector<sf::Int16>(samples, samples + sampleCount);
     }
     cv.notify_one();
     return true;
 }

 bool SamplerRecorder::onStart() {
     isRecording = true;
     return true;
 }

 void SamplerRecorder::onStop() {
     isRecording = false;
     cv.notify_one();
 }
