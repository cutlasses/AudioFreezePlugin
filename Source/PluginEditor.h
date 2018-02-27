/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class AudioFreezePluginAudioProcessorEditor  : public AudioProcessorEditor, public ToggleButton::Listener
{
public:
    AudioFreezePluginAudioProcessorEditor (AudioFreezePluginAudioProcessor&);
    ~AudioFreezePluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	
	//// ToggleButton::Listener ////
	void buttonClicked (Button*) override;
	/////////////////////////

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioFreezePluginAudioProcessor& processor;
	
	ScopedPointer<ToggleButton>						m_freeze_button;
	AudioParameterBool* 							m_freeze_param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioFreezePluginAudioProcessorEditor)
};
