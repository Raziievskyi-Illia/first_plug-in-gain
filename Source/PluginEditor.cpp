/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
First_PluginAudioProcessorEditor::First_PluginAudioProcessorEditor (First_PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    getLookAndFeel().setColour(juce::Slider::textBoxTextColourId, juce::Colours::white); 
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour::fromRGB(59, 55, 59));
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightgrey);//gainsboro
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colour::fromRGB(101, 110, 119));
    sl_volume.setSliderStyle(juce::Slider::LinearVertical);
    sl_volume.setRange(-48.f, 0.0f, 1); 
    sl_volume.setValue(-1.0); 
    sl_volume.addListener(this); 
    sl_volume.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 80, 30);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 300);
    addAndMakeVisible(sl_volume); 
    sl_volume.setSkewFactorFromMidPoint(-17.0); 
    sliderAttachment.reset(new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.treeState , "gain" , sl_volume));
}

First_PluginAudioProcessorEditor::~First_PluginAudioProcessorEditor()
{
}

//==============================================================================
void First_PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::slategrey);

   
}

void First_PluginAudioProcessorEditor::resized()
{

    sl_volume.setBounds(getLocalBounds()); 
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void First_PluginAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &sl_volume)
    {
        audioProcessor.Volume_value = pow(10 , sl_volume.getValue()/20); 

    }
}
