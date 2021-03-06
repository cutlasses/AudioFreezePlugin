/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include <array>

#include "AudioFreezeEffect.h"
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Util.h"

namespace
{
	// mixdown N channels of buffer into channel 0, using the weights array
	template< size_t N >
	void mix_down( AudioSampleBuffer& buffer, std::array<float, N > weights )
	{
		jassert( buffer.getNumChannels() == N );
		
		for( int s = 0; s < buffer.getNumSamples(); ++s )
		{
			float mixed = 0.0f;
			for( int c = 0; c < buffer.getNumChannels(); ++c )
			{
				mixed += buffer.getSample( c, s ) * weights[ c ];
			}
			
			buffer.setSample( 0, s, mixed );
		}
	}
	
	// mix buffer b2 into b1 using weights w1, w2
	void mix_into( AudioSampleBuffer& b1, AudioSampleBuffer& b2, int channel, float w1, float w2 )
	{
		jassert( b1.getNumChannels() > channel && b2.getNumChannels() > channel );
		
		for( int s = 0; s < b1.getNumSamples(); ++s )
		{
			float mixed = ( b1.getSample( channel, s ) * w1 ) + ( b2.getSample( channel, s ) * w2 );
			
			b1.setSample( channel, s, mixed );
		}
	}
}


//==============================================================================
AudioFreezePluginAudioProcessor::AudioFreezePluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
	m_effect( std::make_unique<AUDIO_FREEZE_EFFECT>() )
{
	addParameter( m_freeze_active = new AudioParameterBool(	"freeze",     			// parameterID
														   	"Freeze!",     			// parameter name
														   	false ) );       		// default value
/*
 AudioParameterFloat*									m_loop_size;
 AudioParameterFloat*									m_loop_centre;
 AudioParameterFloat*									m_speed;
 
 AudioParameterFloat*									m_wow_min_freq;
 AudioParameterFloat*									m_wow_max_freq;
 AudioParameterFloat*									m_wow_amount;
 AudioParameterFloat*									m_flutter_min_freq;
 AudioParameterFloat*									m_flutter_max_freq;
 AudioParameterFloat*									m_flutter_amount;
 */
	
	
	addParameter( m_loop_size = new AudioParameterFloat(		"loop_size",     		// parameterID
													   			"Loop Size",     		// parameter name
													   			0.0f,           		// minimum value
													   			1.0f,   				// maximum value
													   			1.0f ) );       		// default value

	addParameter( m_loop_centre = new AudioParameterFloat(		"loop_centre",     		// parameterID
																"Loop Centre",     		// parameter name
																0.0f,           		// minimum value
																1.0f,   				// maximum value
																0.5f ) );       		// default value
	
	addParameter( m_speed = new AudioParameterFloat(			"speed",     			// parameterID
																"Speed",     			// parameter name
																0.0f,           		// minimum value
																1.0f,   				// maximum value
																0.5f ) );       		// default value
	
	addParameter( m_wow_min_freq = new AudioParameterFloat(		"wow_min_freq",     	// parameterID
																"Wow Min Freq",     	// parameter name
																0.1f,           		// minimum value
																5.0f,   				// maximum value
																1.0f ) );       		// default value
	
	addParameter( m_wow_max_freq = new AudioParameterFloat(		"wow_max_freq",     	// parameterID
														   		"Wow Max Freq",     	// parameter name
														   		0.1f,           		// minimum value
														   		5.0f,   				// maximum value
														   		1.0f ) );       		// default value
	
	addParameter( m_wow_amount = new AudioParameterFloat(		"wow_amount",     		// parameterID
																"Wow Amount",     		// parameter name
																0.0f,           		// minimum value
																1.0f,   				// maximum value
																0.25f ) );       		// default value
	
	addParameter( m_flutter_min_freq = new AudioParameterFloat(	"flutter_min_freq",		// parameterID
															   	"Flutter Min Freq",  	// parameter name
																10.0f,           		// minimum value
															    40.0f,   				// maximum value
															    10.0f ) );       		// default value
	
	addParameter( m_flutter_max_freq = new AudioParameterFloat(	"flutter_max_freq",		// parameterID
															   	"Flutter Max Freq", 	// parameter name
															   	10.0f,           		// minimum value
															   	40.0f,   				// maximum value
															   	10.0f ) );       		// default value
	
	addParameter( m_flutter_amount = new AudioParameterFloat(	"flutter_amount",   	// parameterID
																 "Flutter Amount",  	// parameter name
																 0.0f,           		// minimum value
																 1.0f,   				// maximum value
																 0.25f ) );       		// default value
	
}

AudioFreezePluginAudioProcessor::~AudioFreezePluginAudioProcessor()
{
}

//==============================================================================
const String AudioFreezePluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioFreezePluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioFreezePluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioFreezePluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioFreezePluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioFreezePluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioFreezePluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioFreezePluginAudioProcessor::setCurrentProgram (int index)
{
}

const String AudioFreezePluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudioFreezePluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void AudioFreezePluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void AudioFreezePluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudioFreezePluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void AudioFreezePluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals no_denormals;
	
	const double current_sample_rate	= getSampleRate();
	const int current_block_size		= getBlockSize();
	AUDIO_BLOCK_SAMPLES					= current_block_size;
	AUDIO_SAMPLE_RATE					= trunc_to_int( current_sample_rate );
	 
	bool stereo(false);
	if( buffer.getNumChannels() > 1 )
	{
		jassert( buffer.getNumChannels() == 2 );
		
		stereo = true;
	}
	
	if( stereo )
	{
		// mix the 2 channels down into one
		constexpr std::array<float, 2> stereo_weights = { 0.5f, 0.5f };
		mix_down( buffer, stereo_weights );
	}
	
	m_effect->pre_process_audio( buffer, m_effect->num_input_channels(), m_effect->num_output_channels() );
	
	m_effect->set_freeze( *m_freeze_active );
	m_effect->set_length( *m_loop_size );
	m_effect->set_centre( *m_loop_centre );
	m_effect->set_speed( *m_speed );
	
	m_effect->set_wow_frequency_range( *m_wow_min_freq, *m_wow_max_freq );
	m_effect->set_wow_amount( *m_wow_amount );
	m_effect->set_flutter_frequency_range( *m_flutter_min_freq, *m_flutter_max_freq );
	m_effect->set_flutter_amount( *m_flutter_amount );
	
	m_effect->update();
	
	AudioSampleBuffer output( m_effect->num_output_channels(), buffer.getNumSamples() );
	m_effect->post_process_audio( output );
	
	// copy our mixed output to channel 0 of buffer
	buffer.copyFrom( 0, 0, output, 0, 0, output.getNumSamples() );
	
	if( stereo )
	{
		buffer.copyFrom( 1, 0, output, 0, 0, output.getNumSamples() );
	}
}

//==============================================================================
bool AudioFreezePluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* AudioFreezePluginAudioProcessor::createEditor()
{
    return new AudioFreezePluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioFreezePluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void AudioFreezePluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioFreezePluginAudioProcessor();
}
