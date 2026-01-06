#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WavefolderProcessor::WavefolderProcessor()
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

WavefolderProcessor::~WavefolderProcessor()
{
}

//==============================================================================
const juce::String WavefolderProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WavefolderProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool WavefolderProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool WavefolderProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double WavefolderProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WavefolderProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
    // so this should be at least 1, even if you're not really implementing programs.
}

int WavefolderProcessor::getCurrentProgram()
{
    return 0;
}

void WavefolderProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String WavefolderProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void WavefolderProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout WavefolderProcessor::createParams()
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
    
    // Bias (pre-drive)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::biasPreId,
                                                           Parameters::biasPreName,
                                                           juce::NormalisableRange<float>(Parameters::biasMin, Parameters::biasMax, 0.01f),
                                                           Parameters::biasDefault
                                                           )
               );
    
    // Bias (post-drive)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::biasPostId,
                                                           Parameters::biasPostName,
                                                           juce::NormalisableRange<float>(Parameters::biasMin, Parameters::biasMax, 0.01f),
                                                           Parameters::biasDefault
                                                           )
               );
    
    // Fold limit threshold
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::thresId,
                                                           Parameters::thresName,
                                                           juce::NormalisableRange<float>(Parameters::thresMin, Parameters::thresMax, 0.01f),
                                                           Parameters::thresDefault
                                                           )
               );
    
    
    // Mix (%)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
                                                           Parameters::mixId,
                                                           Parameters::mixName,
                                                           juce::NormalisableRange<float>(Parameters::mixMin, Parameters::mixMax, 0.1f),
                                                           Parameters::mixDefault
                                                           )
               );
    
    // Waveshaper options
    juce::StringArray processorChoices { "FoldToRange", "SinFold", "ComboFold" };
    
    layout.add (std::make_unique<juce::AudioParameterChoice>(
                                                             "wavefolder",           // Parameter ID
                                                             "Wavefolder Type",          // Parameter name
                                                             processorChoices,          // Choices
                                                             0                          // Default index
                                                             )
                );
    
    return layout;
}

//==============================================================================
void WavefolderProcessor::updateParameters()
{
    wf.setDrive( juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::driveId)->load() ) );
    wf.setOutGain( juce::Decibels::decibelsToGain( apvts.getRawParameterValue(Parameters::outGainId)->load() ) );
    
    wf.setBiasPre( apvts.getRawParameterValue(Parameters::biasPreId)->load() );
    wf.setBiasPost( apvts.getRawParameterValue(Parameters::biasPostId)->load() );
    
    wf.setThreshold( apvts.getRawParameterValue(Parameters::thresId)->load() );
    wf.setMix( apvts.getRawParameterValue(Parameters::mixId)->load() / 100.0f);
    
    // Get the current processor type
    int typesIndex = (int) apvts.getRawParameterValue ("wavefolder")->load();
    wfType = juce::jlimit(0, 2, typesIndex);
}

void WavefolderProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::dsp::ProcessSpec spec;
    // spec.maximumBlockSize = samplesPerBlock;
    // spec.numChannels = getTotalNumOutputChannels();
    // spec.sampleRate = sampleRate;
    // waveshaper.prepare(spec);
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    
    updateParameters();
}

void WavefolderProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool WavefolderProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WavefolderProcessor::processBlock (juce::AudioBuffer<float>& buffer,
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
    switch (wfType) {
        case 0:
            wf.foldToRangeBuffer(buffer);
            break;
        case 1:
            wf.foldSinBuffer(buffer);
            break;
        case 2:
            wf.comboFoldBuffer(buffer);
            break;
        default:
            wf.foldToRangeBuffer(buffer);
            break;
    }
}

//==============================================================================
bool WavefolderProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WavefolderProcessor::createEditor()
{
    return new PluginEditor (*this);
    // return new juce::GenericAudioProcessorEditor (*this);
}

//==============================================================================
void WavefolderProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void WavefolderProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WavefolderProcessor();
}
