#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "punk_dsp/punk_dsp.h"

#if (MSVC)
#include "ipps.h"
#endif

namespace Parameters
{
    // In Gain
    constexpr auto driveId = "drive";
    constexpr auto driveName = "Drive (dB)";
    constexpr auto driveDefault = 0.0f;
    constexpr auto driveMin = -30.0f;
    constexpr auto driveMax = 60.0f;

    // Out Gain
    constexpr auto outGainId = "outGain";
    constexpr auto outGainName = "Output Gain (dB)";
    constexpr auto outGainDefault = 0.0f;
    constexpr auto outGainMin = -30.0f;
    constexpr auto outGainMax = 30.0f;

    // Bias (pre-drive & post-drive)
    constexpr auto biasPreId = "biasPre";
    constexpr auto biasPreName = "Bias (pre-drive)";
    constexpr auto biasPostId = "biasPost";
    constexpr auto biasPostName = "Bias (post-drive)";
    constexpr auto biasDefault = 0.0f;
    constexpr auto biasMin = -1.0f;
    constexpr auto biasMax = 1.0f;

    // Fold limit threshold
    constexpr auto thresId = "thres";
    constexpr auto thresName = "Threshold";
    constexpr auto thresDefault = 0.7f;
    constexpr auto thresMin = 0.05f;
    constexpr auto thresMax = 1.0f;

    // Mix (%)
    constexpr auto mixId = "mix";
    constexpr auto mixName = "Mix (%)";
    constexpr auto mixDefault = 100.0f;
    constexpr auto mixMin = 0.0f;
    constexpr auto mixMax = 100.0f;
}

class WavefolderProcessor : public juce::AudioProcessor
{
public:
    WavefolderProcessor();
    ~WavefolderProcessor() override;

    // DEFAULT STUFF ===============================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // ===== MY STUFF ===============================================================
    juce::AudioProcessorValueTreeState apvts;
    
    void updateParameters();

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    
    punk_dsp::Wavefolder wf;
    int wfType = 0; // Index for choosing wavefolder
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavefolderProcessor)
};
