/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Util.h"

namespace
{
	const int BORDER( 20 );
	const int BUTTON_SIZE( 200 );
	const int DIAL_SIZE( 100 );
	const int DIAL_LABEL_HEIGHT( 10 );
	const int DIAL_SEPARATION( 10 );
	const int NUM_DIALS_PER_ROW( 3 );
}

//==============================================================================
AudioFreezePluginAudioProcessorEditor::AudioFreezePluginAudioProcessorEditor (AudioFreezePluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	m_all_dials(),
	m_all_labels(),
	m_freeze_button(nullptr),
	m_freeze_param( dynamic_cast<AudioParameterBool*>(p.getParameters().getFirst()) ),
	m_all_float_parameters(),
	m_num_rows(0)
{
	const OwnedArray<AudioProcessorParameter>& params = p.getParameters();
	
	for( int i = 0; i < params.size(); ++i )
	{
		if( AudioParameterFloat* param = dynamic_cast<AudioParameterFloat*>(params[i]) )
		{
			Slider* slider = new Slider( param->name );
			slider->setRange( param->range.start, param->range.end );
			slider->setSliderStyle( Slider::RotaryHorizontalVerticalDrag );
			slider->setValue( *param );
			slider->setTextBoxStyle( Slider::NoTextBox, false, 0, 0 );
			
			slider->addListener( this );
			addAndMakeVisible( slider );
			
			Label* label = new Label( param->name, param->name );
			label->setJustificationType( Justification::centred );
			label->setFont( Font(DIAL_LABEL_HEIGHT) );
			addAndMakeVisible( label );
			
			m_all_dials.add( slider );
			m_all_labels.add( label );
			
			m_all_float_parameters.push_back( param );
		}
		else if( AudioParameterBool* param = dynamic_cast<AudioParameterBool*>(params[i]) )
		{
			ASSERT_MSG( m_freeze_button == nullptr, "More than one bool parameter?" );
			
			m_freeze_button = new ToggleButton( param->name );
			m_freeze_button->addListener( this );
			addAndMakeVisible( m_freeze_button );
			
			//m_all_bool_parameters.push_back( param );
		}
	}
	
	m_num_rows = m_all_dials.size() / NUM_DIALS_PER_ROW;
	if( m_all_dials.size() % NUM_DIALS_PER_ROW != 0 )
	{
		++m_num_rows;
	}

	const int width      		= ( ( DIAL_SIZE + DIAL_LABEL_HEIGHT ) * NUM_DIALS_PER_ROW ) + (BORDER * 2);
	const int height      		= ( m_num_rows * DIAL_SIZE ) + BUTTON_SIZE + (BORDER * 2);
	setSize( width, height );
	
}

AudioFreezePluginAudioProcessorEditor::~AudioFreezePluginAudioProcessorEditor()
{

}

void AudioFreezePluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	if( AudioParameterFloat* param = m_all_float_parameters[ m_all_dials.indexOf(slider) ] )
	{
		*param = slider->getValue();
	}
}

void AudioFreezePluginAudioProcessorEditor::sliderDragStarted (Slider* slider)
{
	if( AudioParameterFloat* param = m_all_float_parameters[ m_all_dials.indexOf(slider) ] )
	{
		param->beginChangeGesture();
	}
}

void AudioFreezePluginAudioProcessorEditor::sliderDragEnded (Slider* slider)
{
	if( AudioParameterFloat* param = m_all_float_parameters[ m_all_dials.indexOf(slider) ] )
	{
		param->endChangeGesture();
	}
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

	g.setColour (Colours::white);
	g.setFont (15.0f);
}

void AudioFreezePluginAudioProcessorEditor::resized()
{
	Rectangle<int> reduced = getLocalBounds().reduced( BORDER );
	
	auto add_dial = []( Rectangle<int>& row_rect, Slider& dial, int dial_size, Label& label, int label_size )
	{
		Rectangle<int> dial_bounds          = row_rect.removeFromLeft( dial_size );
		const Rectangle<int> label_bounds   = dial_bounds.removeFromBottom( label_size );
		
		dial.setBounds( dial_bounds );
		label.setBounds( label_bounds );
	};
	
	int dial( 0 );
	for( int r = 0; r < m_num_rows; ++r )
	{
		Rectangle<int> row_rect = reduced.removeFromTop( DIAL_SIZE );
		
		const int row_size				= min_val<int>( NUM_DIALS_PER_ROW, static_cast<int>(m_all_dials.size()) - dial );
		
		for( int col = 0; col < row_size; ++col )
		{
			add_dial( row_rect, *m_all_dials[dial], DIAL_SIZE, *m_all_labels[dial], DIAL_LABEL_HEIGHT );
			++dial;
		}
		
		// leave space between each row
		reduced.removeFromTop( DIAL_SEPARATION );
	}
	
	//ASSERT_MSG( m_freeze_button->getWidth() == main_row_rect.getWidth(), "Invalid row width remaining" );
	m_freeze_button->setBounds( reduced );	
}
