#pragma once

#include "PluginProcessor.h"

namespace UIConstants
{
// 5-color palette
const juce::Colour background    = juce::Colour(0xff1B3C53);
const juce::Colour primary       = juce::Colour(0xffDFD0B8);
const juce::Colour secondary     = juce::Colour(0xff456882);
const juce::Colour text          = juce::Colour(0xffE0D9D9);
const juce::Colour highlight     = juce::Colour(0xffD3DAD9);

// Sizing constants
const int knobSize = 80;
const int margin = 20;
}

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (WaveshaperProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WaveshaperProcessor& processorRef;
    
    // Custom Look and Feel
    // punk_dsp::ExamplesLnF myCustomLnF;
    
    // Layout utilities
    juce::TextButton header, params;
    
    // Sliders - Rotary knobs
    juce::Slider driveSlider, outGainSlider, biasPreSlider, biasPostSlider, coeffPosSlider, coeffNegSlider, sagTimeSlider, harmGainSlider, harmBalanceSlider;
    juce::TextButton harmSCButton;
        
    // Attachments for linking sliders-parameters
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment, outGainAttachment, biasPreAttachment, biasPostAttachment, coeffPosAttachment, coeffNegAttachment, sagTimeAttachment, harmGainAttachment, harmBalanceAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> harmSCAttachment;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
