/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class First_PluginAudioProcessorEditor  : public juce::AudioProcessorEditor , juce::Slider::Listener
{
public:
    First_PluginAudioProcessorEditor (First_PluginAudioProcessor&);
    ~First_PluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override; 
   
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    First_PluginAudioProcessor& audioProcessor;
    juce::Slider sl_volume; 
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (First_PluginAudioProcessorEditor)
};
