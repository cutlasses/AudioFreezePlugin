#pragma once

#include "TeensyJuce.h"

#define FREEZE_QUEUE_SIZE_IN_BYTES     1024*50      // 50k

#ifdef TARGET_JUCE
extern int AUDIO_BLOCK_SAMPLES;           // NASTY HACK - make these global for JUCE
extern int AUDIO_SAMPLE_RATE;
#endif

////////////////////////////////////////

class RANDOM_LFO
{
	float				m_min_frequency;
	float				m_max_frequency;
	
	float				m_p_ratio;
	float				m_time;
	
	float				m_prev_value;
	
	void				choose_next_frequency();

	void				set_period( float seconds );
	void				set_frequency( float hz );

public:
	
	RANDOM_LFO( float min_frequency, float max_frequency );

	void				set_frequency_range( float min_frequency, float max_frequency );

	float				next( float time_inc );
};

////////////////////////////////////////

class AUDIO_FREEZE_EFFECT : public TEENSY_AUDIO_STREAM_WRAPPER
{
  uint8     	       	m_buffer[FREEZE_QUEUE_SIZE_IN_BYTES];
  
  float                 m_head;     // read head when audio is frozen, write head when not frozen

  float                 m_speed;

  int                   m_loop_start;   // index of the first sample to play in loop
  int                   m_loop_end;     // index of the last sample to play in loop

  int                   m_sample_size_in_bits;
  int                   m_buffer_size_in_samples;

  bool                  m_freeze_active;
  bool                  m_reverse;
  bool                  m_cross_fade;

  // store 'next' values, otherwise interrupt could be called during calculation of values
  int	                m_next_sample_size_in_bits;
  float                 m_next_length;
  float                 m_next_centre;
  float                 m_next_speed;
  bool					m_next_freeze_active;
	
  RANDOM_LFO			m_wow_lfo;
  RANDOM_LFO			m_flutter_lfo;
	
float					m_wow_amount;
float					m_flutter_amount;
  


  int                   wrap_index_to_loop_section( int index ) const;

  void                  write_sample( int16_t sample, int index );
  int16_t               read_sample( int index ) const;
  
  void                  write_to_buffer( const int16_t* source, int size );
  void                  read_from_buffer( int16_t* dest, int size );
  int16_t               read_sub_sample( float current ) const;
  void                  read_from_buffer_with_speed( int16_t* dest, int size );
  void                  read_from_buffer_with_speed_and_cross_fade( int16_t* dest, int size );
  
  float                 next_head( float inc ) const;

  void                  set_bit_depth_impl( int sample_size_in_bits );
  void                  set_length_impl( float length );
  void                  set_centre_impl( float centre );
  void                  set_speed_impl( float speed );
  void					set_freeze_impl( bool active );

protected:
	
	void				process_audio_in_impl( int channel, const int16_t* sample_data, int num_samples ) override;
	void				process_audio_out_impl( int channel, int16_t* sample_data, int num_samples ) override;
  
public:

  AUDIO_FREEZE_EFFECT();
	
  int                   num_input_channels() const override;
  int                   num_output_channels() const override;

  virtual void          update() override;

  bool                  is_freeze_active() const;
  
  void                  set_freeze( bool active );
  void                  set_length( float length );
  void                  set_centre( float centre );
  void                  set_speed( float speed );
  void                  set_reverse( bool reverse );
  void                  set_cross_fade( bool cross_fade );
  void                  set_bit_depth( int sample_size_in_bits );
	
  void					set_wow_frequency_range( float min_frequency, float max_frequency );
  void					set_wow_amount( float amount );
  void					set_flutter_frequency_range( float min_frequency, float max_frequency );
  void					set_flutter_amount( float amount );
};

