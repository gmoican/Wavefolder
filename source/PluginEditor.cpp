#include "PluginEditor.h"

PluginEditor::PluginEditor (WavefolderProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    juce::ignoreUnused (processorRef);
    juce::LookAndFeel::setDefaultLookAndFeel(&myCustomLnF);
    
    // --- LAYOUT ---
    header.setColour (juce::TextButton::buttonColourId, punk_dsp::UIConstants::background.brighter(0.5f)
                                                                               .withAlpha(0.25f)
                      );
    header.setEnabled(false);
    header.setColour(juce::TextButton::textColourOffId, punk_dsp::UIConstants::highlight);
    header.setColour(juce::TextButton::textColourOnId, punk_dsp::UIConstants::highlight);
    header.setButtonText ("Punk DSP - Wavefolder");
    addAndMakeVisible (header);
    
    params.setColour (juce::TextButton::buttonColourId, punk_dsp::UIConstants::background.brighter(0.5f)
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
    biasPreSlider.setName(u8"β\u2080");
    addAndMakeVisible(biasPreSlider);
    
    biasPreAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::biasPreId, biasPreSlider);
    
    // Bias (post-drive)
    biasPostSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    biasPostSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    biasPostSlider.setRange(Parameters::biasMin, Parameters::biasMax, 0.01);
    biasPostSlider.setValue(Parameters::biasDefault);
    biasPostSlider.setName(u8"β\u2081");
    addAndMakeVisible(biasPostSlider);
    
    biasPostAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, Parameters::biasPostId, biasPostSlider);

    // Threshold
    thresSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    thresSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    thresSlider.setRange(Parameters::thresMin, Parameters::thresMax, 0.01);
    thresSlider.setValue(Parameters::thresDefault);
    thresSlider.setName("Thres");
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
    
    // Sizing calculations
    const int numCols = 3;
    const int numRows = 2;

    const int totalWidth = (numCols * (punk_dsp::UIConstants::knobSize + 2 * punk_dsp::UIConstants::margin)) + (10 * 2);
    const int totalHeight = punk_dsp::UIConstants::headerHeight + punk_dsp::UIConstants::comboboxHeight + 2 * punk_dsp::UIConstants::margin + (numRows * (punk_dsp::UIConstants::knobSize + 2 * punk_dsp::UIConstants::margin)) + (10 * 2);
    
    setSize (totalWidth, totalHeight);
}

PluginEditor::~PluginEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (punk_dsp::UIConstants::background);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto area = getLocalBounds();
    
    // --- LAYOUT SETUP ---
    auto headerArea = area.removeFromTop( punk_dsp::UIConstants::headerHeight );
    auto paramsArea = area.reduced( 10 );
    
    header.setBounds(headerArea);
    params.setBounds(paramsArea);
    
    // Flexbox for sliders
    juce::FlexBox fb;
    fb.flexDirection = juce::FlexBox::Direction::row;
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    fb.alignContent = juce::FlexBox::AlignContent::spaceBetween;
    
    // Add components to the FlexBox
    fb.items.add(juce::FlexItem(wfComboBox).withMinWidth(paramsArea.getWidth() - 2 * punk_dsp::UIConstants::margin)
                                            .withMinHeight(punk_dsp::UIConstants::comboboxHeight)
                                            .withMargin(punk_dsp::UIConstants::margin));
    
    fb.items.add(juce::FlexItem(driveSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                            .withMinHeight(punk_dsp::UIConstants::knobSize)
                                            .withMargin(punk_dsp::UIConstants::margin));
    fb.items.add(juce::FlexItem(outGainSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                              .withMinHeight(punk_dsp::UIConstants::knobSize)
                                              .withMargin(punk_dsp::UIConstants::margin));
    fb.items.add(juce::FlexItem(mixSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                          .withMinHeight(punk_dsp::UIConstants::knobSize)
                                          .withMargin(punk_dsp::UIConstants::margin));
    
    fb.items.add(juce::FlexItem(biasPreSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                              .withMinHeight(punk_dsp::UIConstants::knobSize)
                                              .withMargin(punk_dsp::UIConstants::margin));
    fb.items.add(juce::FlexItem(biasPostSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                               .withMinHeight(punk_dsp::UIConstants::knobSize)
                                               .withMargin(punk_dsp::UIConstants::margin));
    fb.items.add(juce::FlexItem(thresSlider).withMinWidth(punk_dsp::UIConstants::knobSize)
                                            .withMinHeight(punk_dsp::UIConstants::knobSize)
                                            .withMargin(punk_dsp::UIConstants::margin));
    
    // Perform the layout
    fb.performLayout(paramsArea);
}
