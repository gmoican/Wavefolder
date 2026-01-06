#include "PluginEditor.h"

PluginEditor::PluginEditor (WaveshaperProcessor& p)
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
    header.setButtonText ("Punk DSP - TubeModel");
    addAndMakeVisible (header);
    
    params.setColour (juce::TextButton::buttonColourId, UIConstants::background.brighter(0.5f)
                                                                               .withAlpha(0.25f)
                      );
    params.setEnabled(false);
    addAndMakeVisible (params);
    
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
    
    // Bias (Pre-Drive)
    biasPreSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    biasPreSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    biasPreSlider.setRange(Parameters::biasMin, Parameters::biasMax, 0.01);
    biasPreSlider.setValue(Parameters::biasDefault);
    biasPreSlider.setName("Bias (pre)");
    addAndMakeVisible(biasPreSlider);
    
    biasPreAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::preBiasId, biasPreSlider);

    // Bias (Post-Drive)
    biasPostSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    biasPostSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    biasPostSlider.setRange(Parameters::biasMin, Parameters::biasMax, 0.01);
    biasPostSlider.setValue(Parameters::biasDefault);
    biasPostSlider.setName("Bias (post)");
    addAndMakeVisible(biasPostSlider);
    
    biasPostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::postBiasId, biasPostSlider);
    
    // Positive Denominator Coefficients
    coeffPosSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    coeffPosSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    coeffPosSlider.setRange(Parameters::coeffMin, Parameters::coeffMax, 0.01);
    coeffPosSlider.setValue(Parameters::coeffDefault);
    coeffPosSlider.setName("Coeff (pos)");
    addAndMakeVisible(coeffPosSlider);
    
    coeffPosAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::coeffPosId, coeffPosSlider);
    
    // Negative Denominator Coefficients
    coeffNegSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    coeffNegSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    coeffNegSlider.setRange(Parameters::coeffMin, Parameters::coeffMax, 0.01);
    coeffNegSlider.setValue(Parameters::coeffDefault);
    coeffNegSlider.setName("Coeff (neg)");
    addAndMakeVisible(coeffNegSlider);
    
    coeffNegAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::coeffNegId, coeffNegSlider);
    
    // Sag Time
    sagTimeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sagTimeSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    sagTimeSlider.setRange(Parameters::sagTimeMin, Parameters::sagTimeMax, 0.1);
    sagTimeSlider.setValue(Parameters::sagTimeDefault);
    sagTimeSlider.setName("Sag (ms)");
    addAndMakeVisible(sagTimeSlider);
    
    sagTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::sagTimeId, sagTimeSlider);
    
    // Harmonic Gain (%)
    harmGainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    harmGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    harmGainSlider.setRange(Parameters::harmonicsGainMin, Parameters::harmonicsGainMax, 0.1);
    harmGainSlider.setValue(Parameters::harmonicsGainDefault);
    harmGainSlider.setName("Harmonics (%)");
    addAndMakeVisible(harmGainSlider);
    
    harmGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::harmonicsGainId, harmGainSlider);
    
    // Harmonic Balance
    harmBalanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    harmBalanceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    harmBalanceSlider.setRange(Parameters::harmonicsBalanceMin, Parameters::harmonicsBalanceMax, 0.1);
    harmBalanceSlider.setValue(Parameters::harmonicsBalanceDefault);
    harmBalanceSlider.setName("Harmonic balance");
    addAndMakeVisible(harmBalanceSlider);
    
    harmBalanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::harmonicsBalanceId, harmBalanceSlider);
    
    // Harmonic Balance
    harmSCButton.setClickingTogglesState(true);
    harmSCButton.setName("Harmonics Sidechain");
    addAndMakeVisible(harmSCButton);
    
    harmSCAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processorRef.apvts, Parameters::harmonicsSidechainId, harmSCButton);
    
    setSize (540, 280);
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
    
    // First row: 3 sliders
    auto row1 = paramsBounds.removeFromTop(UIConstants::knobSize + UIConstants::margin);
    driveSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    outGainSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    harmGainSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    harmBalanceSlider.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    row1.removeFromLeft(UIConstants::margin);
    harmSCButton.setBounds(row1.removeFromLeft(UIConstants::knobSize));
    
    paramsBounds.removeFromTop(UIConstants::margin);
    
    // Second row: 3 sliders
    auto row2 = paramsBounds.removeFromTop(UIConstants::knobSize + UIConstants::margin);
    biasPreSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    biasPostSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    coeffPosSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    coeffNegSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    row2.removeFromLeft(UIConstants::margin);
    sagTimeSlider.setBounds(row2.removeFromLeft(UIConstants::knobSize));
    
    paramsBounds.removeFromTop(UIConstants::margin);
}
