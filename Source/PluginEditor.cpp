/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

namespace
{
	const int BORDER( 20 );
	const int BUTTON_SIZE( 200 );
}

//==============================================================================
AudioFreezePluginAudioProcessorEditor::AudioFreezePluginAudioProcessorEditor (AudioFreezePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	m_freeze_button(nullptr),
	m_freeze_param( dynamic_cast<AudioParameterBool*>(p.getParameters().getFirst()) )
{
	m_freeze_button = new ToggleButton( "Freeze" );
	m_freeze_button->addListener( this );
	addAndMakeVisible( m_freeze_button );

	const int width      		= BUTTON_SIZE + (BORDER * 2);
	const int height      		= BUTTON_SIZE + (BORDER * 2);
	setSize( width, height );
	
}

AudioFreezePluginAudioProcessorEditor::~AudioFreezePluginAudioProcessorEditor()
{

}

void AudioFreezePluginAudioProcessorEditor::buttonClicked (Button*)
{
	*m_freeze_param = !(*m_freeze_param);
}

//==============================================================================
void AudioFreezePluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

	
}

void AudioFreezePluginAudioProcessorEditor::resized()
{
	Rectangle<int> reduced = getLocalBounds().reduced( BORDER );
	m_freeze_button->setBounds( reduced );
}
