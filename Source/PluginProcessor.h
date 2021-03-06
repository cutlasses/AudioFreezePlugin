/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AUDIO_FREEZE_EFFECT;

//==============================================================================
/**
*/
class AudioFreezePluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    AudioFreezePluginAudioProcessor();
    ~AudioFreezePluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFreezePluginAudioProcessor)
	
	std::unique_ptr<AUDIO_FREEZE_EFFECT>					m_effect;
	
	AudioParameterBool*										m_freeze_active;
	
	AudioParameterFloat*									m_loop_size;
	AudioParameterFloat*									m_loop_centre;
	AudioParameterFloat*									m_speed;
	
	AudioParameterFloat*									m_wow_min_freq;
	AudioParameterFloat*									m_wow_max_freq;
	AudioParameterFloat*									m_wow_amount;
	AudioParameterFloat*									m_flutter_min_freq;
	AudioParameterFloat*									m_flutter_max_freq;
	AudioParameterFloat*									m_flutter_amount;
};
