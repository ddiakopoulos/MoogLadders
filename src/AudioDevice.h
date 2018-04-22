#pragma once

#ifndef AUDIO_DEVICE_H
#define AUDIO_DEVICE_H

// This file implements a simple sound file player based on RtAudio for testing / example purposes.

#include "Util.h"
#include "RingBuffer.h"
#include "rtaudio/RtAudio.h"

#include <iostream>
#include <memory>

static const unsigned int FRAME_SIZE = 512;
static const int CHANNELS = 2;
static const int BUFFER_LENGTH = FRAME_SIZE * CHANNELS;

struct DeviceInfo
{
	int id;
	int numChannels;
	int sampleRate;
	unsigned int frameSize;
	bool isPlaying = false;
};

class AudioDevice
{
	NO_MOVE(AudioDevice);
	std::unique_ptr<RtAudio> rtaudio;
public:
	static void ListAudioDevices();
	AudioDevice(int numChannels, int sampleRate, int deviceId = -1);
	~AudioDevice();
	bool Open(const int deviceId);
	bool Play(const std::vector<float> & data);
	DeviceInfo info;
};

#endif
