/*
  ==============================================================================

    LtcReader.cpp
    Created: 29 Jan 2024 11:35:34am
    Author:  joris

  ==============================================================================
*/

#include "LtcReader.h"

LTCReader::LTCReader()
{
	decoder = ltc_decoder_create( 1920, 32 );
}

LTCReader::~LTCReader()
{
}

void LTCReader::processAudio( const float* inBuffer, const size_t length )
{
	unsigned char* bytes = new unsigned char[ length ];
	for ( int i = 0; i < length; i++ )
		bytes[ i ] = static_cast< unsigned char > ( ( ( inBuffer[ i ] / 2.0f ) + 0.5f ) * 255 );

	ltc_decoder_write( decoder, bytes, length, length );
	
	while ( ltc_decoder_read( decoder, &frame ) ) 
	{
		ltc_frame_to_time( &stime, &frame.ltc, 1 );
		listeners.call( &Listener::newFrameReceived );
	}
}

int LTCReader::getCurrentFrame()
{
	return 0;
}

String LTCReader::getCurrentFrameAsString()
{
	char timeString[ 80 ];
	sprintf( timeString, "%02d:%02d:%02d%c%02d",
		stime.hours,
		stime.mins,
		stime.secs,
		( frame.ltc.dfbit ) ? '.' : ':',
		stime.frame

	);
	return String( timeString );
}
