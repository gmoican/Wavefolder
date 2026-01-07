# Wavefolder
This is a VST3/AU tube modeling plugin made with [JUCE](https://juce.com/). The sole purpose of this plugin is to showcase and test the performance of my `Wavefolder` class in [punk_dsp](https://github.com/gmoican/punk_dsp).

[![Wavefolder](https://github.com/gmoican/Wavefolder/actions/workflows/build.yml/badge.svg)](https://github.com/gmoican/Wavefolder/actions/workflows/build.yml)

![DemoImage](docs/demo.png)

## Introduction

## Features

- **Three wavefolding algorithms**: _fold to range_, _sin-wave folding_ and a combination of both.
- **Flexible processing**: Process individual samples or entire audio buffers.
- **Configurable parameters**:
    - `drive`: Input gain, modifies the waveshaper behaviour.
    - `outGain`: Output gain, intended to be used for compensating the volume difference.
    - `biasPre`: Bias introduced before applying `drive`, modifies the DC offset for asymmetric shaping.
    - `biasPost`: Bias introduced after applying `drive`, modifies the DC offset for asymmetric shaping.
    - `threshold`: Linear threshold for folding the signal.
    - `mix`: For blending the wavefolded signal with the clean input.

## Usage examples

```cpp
// --- PluginProcessor.h ---
#include "punk_dsp/punk_dsp.h"

class PluginProcessor : public juce::AudioProcessor
{
public:
    /* Your processor public stuff
     * ...
     */
private:
    /* Your processor private stuff
     * ...
     */
    punk_dsp::Wavefolder wf;
};

// --- PluginProcessor.cpp ---
void PluginProcessor::updateParameters()
{
    // Your code...

    wf.setDrive( newDrive );      	 // Make sure this values are in linear range
    wf.setOutGain( newOutGain );     // Make sure this values are in linear range
    wf.setBiasPre( newBias );  		 // Between -1.0 and 1.0
    wf.setBiasPost( newBias );    	 // Between -1.0 and 1.0
    wf.setThreshold( newThreshold ); // Between  0.0 and 1.0
    wf.setMix( newMix );			 // Between  0.0 and 1.0

    // Your code...
}

void PunkOTTProcessor::processBlock (juce::AudioBuffer<float>& buffer)
{
    // Sample application
    for (int channel = 0; channel < numChannels; ++channel)
    {
        float* channelData = inputBuffer.getWritePointer(channel);
        for (int sample = 0; sample < numSamples; ++sample)
            channelData[sample] = wf.foldToRangeSample(channelData[sample]);
            channelData[sample] = wf.foldSinSample(channelData[sample]);
            channelData[sample] = wf.comboFoldSample(channelData[sample]);
    }

    // Buffer application
    wf.foldToRangeBuffer(buffer);
    wf.foldSinBuffer(buffer);
    wf.comboFoldBuffer(buffer);
}
```

## Plugins that make use of this compressor
* Nothing for the moment...

