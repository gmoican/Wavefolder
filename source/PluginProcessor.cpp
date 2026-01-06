#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveshaperProcessor::WaveshaperProcessor()
: AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                  .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
#endif
                  .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                  ), apvts(*this, nullptr, "Parameters", createParams())
{
}

WaveshaperProcessor::~WaveshaperProcessor()
{
}

//==============================================================================
const juce::String WaveshaperProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveshaperProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool WaveshaperProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool WaveshaperProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double WaveshaperProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveshaperProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int WaveshaperProcessor::getCurrentProgram()
{
    return 0;
}

void WaveshaperProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String WaveshaperProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void WaveshaperProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout WaveshaperProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Inpout Gain (dB)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::driveId,
                                                           Parameters::driveName,
                                                           juce::NormalisableRange<float>(Parameters::driveMin, Parameters::driveMax, 0.1f),
                                                           Parameters::driveDefault
                                                           )
               );
    
    // Output Gain (dB)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::outGainId,
                                                           Parameters::outGainName,
                                                           juce::NormalisableRange<float>(Parameters::outGainMin, Parameters::outGainMax, 0.1f),
                                                           Parameters::outGainDefault
                                                           )
               );
    
    // Bias (Pre in-gain)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::preBiasId,
                                                           Parameters::preBiasName,
                                                           juce::NormalisableRange<float>(Parameters::biasMin, Parameters::biasMax, 0.01f),
                                                           Parameters::biasDefault
                                                           )
               );
    
    // Bias (Post in-gain)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::postBiasId,
                                                           Parameters::postBiasName,
                                                           juce::NormalisableRange<float>(Parameters::biasMin, Parameters::biasMax, 0.01f),
                                                           Parameters::biasDefault
                                                           )
               );
    
    // Positive Denominator Coefficient
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::coeffPosId,
                                                           Parameters::coeffPosName,
                                                           juce::NormalisableRange<float>(Parameters::coeffMin, Parameters::coeffMax, 0.01f),
                                                           Parameters::coeffDefault
                                                           )
               );
    
    // Negative Denominator Coefficient
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::coeffNegId,
                                                           Parameters::coeffNegName,
                                                           juce::NormalisableRange<float>(Parameters::coeffMin, Parameters::coeffMax, 0.01f),
                                                           Parameters::coeffDefault
                                                           )
               );
    
    // Sag Time (ms)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::sagTimeId,
                                                           Parameters::sagTimeName,
                                                           juce::NormalisableRange<float>(Parameters::sagTimeMin, Parameters::sagTimeMax, 0.1f),
                                                           Parameters::sagTimeDefault
                                                           )
               );
    
    // Harmonics Gain (%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::harmonicsGainId,
                                                           Parameters::harmonicsGainName,
                                                           juce::NormalisableRange<float>(Parameters::harmonicsGainMin, Parameters::harmonicsGainMax, 0.1f),
                                                           Parameters::harmonicsGainDefault
                                                           )
               );
    
    // Harmonics Balance
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::harmonicsBalanceId,
                                                           Parameters::harmonicsBalanceName,
                                                           juce::NormalisableRange<float>(Parameters::harmonicsBalanceMin, Parameters::harmonicsBalanceMax, 0.01f),
                                                           Parameters::harmonicsBalanceDefault
                                                           )
               );
    
    // Harmonics Sidechain
    layout.add(std::make_unique<juce::AudioParameterBool>(
                                                          Parameters::harmonicsSidechainId,
                                                          Parameters::harmonicsSidechainName,
                                                          Parameters::harmonicsSidechainDefault
                                                          )
               );
    
    return layout;
}

//==============================================================================
void WaveshaperProcessor::updateParameters()
{
    tube.setDrive( juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::driveId)->load() ) );
    tube.setOutGain( juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::outGainId)->load() ) );
    
    tube.setBiasPre( apvts.getRawParameterValue(Parameters::preBiasId)->load() );
    tube.setBiasPost( apvts.getRawParameterValue(Parameters::postBiasId)->load() );
    
    tube.setCoeffPos( apvts.getRawParameterValue(Parameters::coeffPosId)->load() );
    tube.setCoeffNeg( apvts.getRawParameterValue(Parameters::coeffNegId)->load() );
    
    tube.setSagTime( apvts.getRawParameterValue(Parameters::sagTimeId)->load() );
    
    tube.setHarmonicGain( apvts.getRawParameterValue(Parameters::harmonicsGainId)->load() / 100.f);
    tube.setHarmonicBalance( apvts.getRawParameterValue(Parameters::harmonicsBalanceId)->load() );
    tube.setHarmonicSidechain( (bool) apvts.getRawParameterValue(Parameters::harmonicsSidechainId)->load() );
}

void WaveshaperProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::dsp::ProcessSpec spec;
    // spec.maximumBlockSize = samplesPerBlock;
    // spec.numChannels = getTotalNumOutputChannels();
    // spec.sampleRate = sampleRate;
    // waveshaper.prepare(spec);
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    
    updateParameters();
}

void WaveshaperProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool WaveshaperProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif
    
    return true;
#endif
}

void WaveshaperProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                     juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Clear any unused output channel
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    // Update params
    updateParameters();
    
    // Process
    tube.processBuffer(buffer);
}

//==============================================================================
bool WaveshaperProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveshaperProcessor::createEditor()
{
    return new PluginEditor (*this);
    // return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void WaveshaperProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void WaveshaperProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveshaperProcessor();
}
