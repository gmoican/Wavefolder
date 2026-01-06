#include "PluginEditor.h"

PluginEditor::PluginEditor (WavefolderProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- LAYOUT ---
    header.setColour (juce::TextButton::buttonColourId, UIConstants::background.brighter(0.5f)
                                                                               .withAlpha(0.25f)
                      );
    header.setEnabled(false);
    header.setButtonText ("Punk DSP - Wavefolder");
    addAndMakeVisible (header);
    
    params.setColour (juce::TextButton::buttonColourId, UIConstants::background.brighter(0.5f)
                                                                               .withAlpha(0.25f)
                      );
    params.setEnabled(false);
    addAndMakeVisible (params);
    
    // Waveshaper choices
    wfComboBox.addItem("FoldToRange", 1);
    wfComboBox.addItem("SinFold", 2);
    wfComboBox.addItem("ComboFold", 3);
    addAndMakeVisible(wfComboBox);
    
    // Attach the ComboBox to the APVTS parameter
    wfAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processorRef.apvts, "wavefolder", wfComboBox);
    
    // Drive
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    driveSlider.setRange(Parameters::driveMin, Parameters::driveMax, 0.1);
    driveSlider.setValue(Parameters::driveDefault);
    driveSlider.setName("Drive");
    addAndMakeVisible(driveSlider);
    
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::driveId, driveSlider);
    
    // Output Gain
    outGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    outGainSlider.setRange(Parameters::outGainMin, Parameters::outGainMax, 0.1);
    outGainSlider.setValue(Parameters::outGainDefault);
    outGainSlider.setName("Out");
    addAndMakeVisible(outGainSlider);
    
    outGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::outGainId, outGainSlider);
    
    // Bias (pre-drive)
    biasPreSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    biasPreSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    biasPreSlider.setRange(Parameters::biasMin, Parameters::biasMax, 0.01);
    biasPreSlider.setValue(Parameters::biasDefault);
    biasPreSlider.setName("Bias (pre)");
    addAndMakeVisible(biasPreSlider);
    
    biasPreAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::biasPreId, biasPreSlider);
    
    // Bias (post-drive)
    biasPostSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    biasPostSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    biasPostSlider.setRange(Parameters::biasMin, Parameters::biasMax, 0.01);
    biasPostSlider.setValue(Parameters::biasDefault);
    biasPostSlider.setName("Bias (post)");
    addAndMakeVisible(biasPostSlider);
    
    biasPostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::biasPostId, biasPostSlider);

    // Threshold
    thresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    thresSlider.setRange(Parameters::thresMin, Parameters::thresMax, 0.01);
    thresSlider.setValue(Parameters::thresDefault);
    thresSlider.setName("Threshold");
    addAndMakeVisible(thresSlider);
    
    thresAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::thresId, thresSlider);
    
    // Mix
    mixSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    mixSlider.setRange(Parameters::mixMin, Parameters::mixMax, 0.01);
    mixSlider.setValue(Parameters::mixDefault);
    mixSlider.setName("Mix");
    addAndMakeVisible(mixSlider);
    
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::mixId, mixSlider);
    
    setSize (330, 350);
}

PluginEditor::~PluginEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (UIConstants::background);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop( 30 );
    auto paramsArea = area.reduced( 10 );
    
    header.setBounds(headerArea);
    params.setBounds(paramsArea);
    
    // --- PARAMS LAYOUT ---
    auto paramsBounds = params.getBounds().reduced(UIConstants::margin);
    
    // Position the ComboBox
    auto comboBoxArea = paramsBounds.removeFromTop(30);
    wfComboBox.setBounds(comboBoxArea);
    
    // First row: 3 sliders
    auto row1 = paramsBounds.removeFromTop(UIConstants::knobSize + UIConstants::margin);
    driveSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    outGainSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    mixSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    
    paramsBounds.removeFromTop(UIConstants::margin);
    
    // Second row: 3 sliders
    auto row2 = paramsBounds.removeFromTop(UIConstants::knobSize + UIConstants::margin);
    biasPreSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    biasPostSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    thresSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    
    paramsBounds.removeFromTop(UIConstants::margin);
}
