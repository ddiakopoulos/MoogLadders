/*
Copyright (c) 2015, Dimitri Diakopoulos All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#if defined(_MSC_VER)
#pragma comment(lib, "dsound.lib")
#endif

#include "AudioDevice.h"
#include "NoiseGenerator.h"

#include "StilsonModel.h"
#include "OberheimVariationModel.h"
#include "SimplifiedModel.h"
#include "ImprovedModel.h"
#include "HuovilainenModel.h"
#include "AaronModel.h"
#include "RKSimulationModel.h"
#include "MicrotrackerModel.h"
#include "MusicDSPModel.h"
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
	
	std::vector<float> noiseSamples = gen.produce(NoiseGenerator::NoiseType::WHITE, desiredSampleRate, desiredChannelCount, 3.0);
	
	StilsonMoog stilsonModel(desiredSampleRate);
	//stilsonModel.Process(noiseSamples.data(), noiseSamples.size());
	
	SimplifiedMoog simplifiedModel(desiredSampleRate);
	//simplifiedModel.Process(noiseSamples.data(), noiseSamples.size());
	
	HuovilainenMoog huovilainenModel(desiredSampleRate);
	//huovilainenModel.Process(noiseSamples.data(), noiseSamples.size());
	
	ImprovedMoog improvedModel(desiredSampleRate);
	//improvedModel.Process(noiseSamples.data(), noiseSamples.size());
	
	MicrotrackerMoog microtrackerModel(desiredSampleRate);
	//microtrackerModel.Process(noiseSamples.data(), noiseSamples.size());

	MusicDSPMoog musicdspModel(desiredSampleRate);
	//musicdspModel.Process(noiseSamples.data(), noiseSamples.size());
	
	AaronMoog aaronModel(desiredSampleRate);
	//aaronModel.Process(noiseSamples.data(), noiseSamples.size());

	RKSimulationMoog rkModel(desiredSampleRate);
	//rkModel.Process(noiseSamples.data(), noiseSamples.size());
	
	OberheimVariationMoog oberheimModel(desiredSampleRate);
	//oberheimModel.Process(noiseSamples.data(), noiseSamples.size());
	
	device.Play(noiseSamples);
	
	return 0;
}
