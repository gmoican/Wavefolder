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

    // Bias (Pre in-gain) & (Post in-gain)
    constexpr auto preBiasId = "biasPre";
    constexpr auto preBiasName = "Bias (Pre)";
    constexpr auto postBiasId = "biasPost";
    constexpr auto postBiasName = "Bias (Post)";
    constexpr auto biasDefault = 0.0f;
    constexpr auto biasMin = -1.0f;
    constexpr auto biasMax = 1.0f;

    // Denominator coefficients (Positive-side & Negative-side)
    constexpr auto coeffPosId = "coeffPos";
    constexpr auto coeffPosName = "Coeff (Pos)";
    constexpr auto coeffNegId = "coeffNeg";
    constexpr auto coeffNegName = "Coeff (Neg)";
    constexpr auto coeffDefault = 1.0f;
    constexpr auto coeffMin = 0.0f;
    constexpr auto coeffMax = 5.0f;

    // Sag Time
    constexpr auto sagTimeId = "sag";
    constexpr auto sagTimeName = "Sag Time (ms)";
    constexpr auto sagTimeDefault = 100.0f;
    constexpr auto sagTimeMin = 0.1f;
    constexpr auto sagTimeMax = 100.0f;

    // Harmonics Gain
    constexpr auto harmonicsGainId = "harmGain";
    constexpr auto harmonicsGainName = "Extra Harmonics (%)";
    constexpr auto harmonicsGainDefault = 5.0f;
    constexpr auto harmonicsGainMin = 0.0f;
    constexpr auto harmonicsGainMax = 50.0f;

    // Harmonics Balance
    constexpr auto harmonicsBalanceId = "harmBalance";
    constexpr auto harmonicsBalanceName = "Harmonics Balance";
    constexpr auto harmonicsBalanceDefault = 0.5f;
    constexpr auto harmonicsBalanceMin = 0.0f;
    constexpr auto harmonicsBalanceMax = 1.0f;

    // Harmonics Sidechain
    constexpr auto harmonicsSidechainId = "harmSidechain";
    constexpr auto harmonicsSidechainName = "Harmonics Sidechain";
    constexpr auto harmonicsSidechainDefault = true;
}

class WaveshaperProcessor : public juce::AudioProcessor
{
public:
    WaveshaperProcessor();
    ~WaveshaperProcessor() override;

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
    
    punk_dsp::TubeModel tube;
    
    // =============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveshaperProcessor)
};
