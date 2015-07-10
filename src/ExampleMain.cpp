#pragma comment(user, "license")

#if defined(_MSC_VER)
#pragma comment(lib, "dsound.lib")
#endif

#include "AudioDevice.h"
#include "NoiseGenerator.h"

#include "StilsonModel.h"
#include "StilsonPirkleModel.h"
#include "SimplifiedModel.h"
#include "ImprovedModel.h"
#include "HuovilainenModel.h"
#include "AaronModel.h"
#include "RKSimulationModel.h"

#include <thread>
#include <chrono>
#include <vector>

int main()
{
    AudioDevice::ListAudioDevices();
    
    int desiredSampleRate = 44100;
    int desiredChannelCount = 2;
    AudioDevice device(desiredChannelCount, desiredSampleRate);
    device.Open(device.info.id);
    
    NoiseGenerator gen;
    
    std::vector<float> noiseSamples = gen.produce(NoiseGenerator::NoiseType::WHITE, desiredSampleRate, desiredChannelCount, 2.0);
    
    StilsonMoog stilsonModel(desiredSampleRate);
    stilsonModel.Process(noiseSamples.data(), noiseSamples.size());
    
    device.Play(noiseSamples);
    
    return 0;
}
