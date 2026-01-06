#pragma once

#include "PluginProcessor.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (WavefolderProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WavefolderProcessor& processorRef;
    
    // Custom Look and Feel
    punk_dsp::ExamplesLnF myCustomLnF;
    
    // Layout utilities
    juce::TextButton header, params;
    
    // Sliders - Rotary knobs
    juce::Slider driveSlider, outGainSlider, biasPreSlider, biasPostSlider, thresSlider, mixSlider;
    juce::ComboBox wfComboBox;
        
    // Attachments for linking sliders-parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment, outGainAttachment, biasPreAttachment, biasPostAttachment, thresAttachment, mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> wfAttachment;
            
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
