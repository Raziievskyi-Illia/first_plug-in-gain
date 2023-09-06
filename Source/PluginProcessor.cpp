/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
First_PluginAudioProcessor::First_PluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )

    
#endif
   
{   
    Volume_value = -5.0f; 
}

First_PluginAudioProcessor::~First_PluginAudioProcessor()
{
}

//==============================================================================
const juce::String First_PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool First_PluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool First_PluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool First_PluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double First_PluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int First_PluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int First_PluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void First_PluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String First_PluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void First_PluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void First_PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   // previous_gain = pow(10, *treeState.getRawParameterValue("gain") / 20);
    previousGain = juce::Decibels::decibelsToGain<float>(*treeState.getRawParameterValue("gain"));
}

void First_PluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool First_PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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
#endif

void First_PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());



    float currentGain = juce::Decibels::decibelsToGain<float>(*treeState.getRawParameterValue("gain"));

    if (juce::approximatelyEqual(currentGain, previousGain))
    {
        buffer.applyGain(currentGain);
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), previousGain, currentGain);
        previousGain = currentGain;
    }

}

//==============================================================================
bool First_PluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* First_PluginAudioProcessor::createEditor()
{
    return new First_PluginAudioProcessorEditor (*this);
}

//==============================================================================
void First_PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void First_PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{

    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessorValueTreeState::ParameterLayout First_PluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout; 
    layout.add(std::make_unique<juce::AudioParameterFloat>("gain", "gain", juce::NormalisableRange<float>(-48.0f, 0.0f), -5.0f));
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new First_PluginAudioProcessor();
}
