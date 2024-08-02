#include "digitalEdgeAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "digitalEdgeAnalyzer.h"
#include "digitalEdgeAnalyzerSettings.h"
#include <iostream>
#include <fstream>

digitalEdgeAnalyzerResults::digitalEdgeAnalyzerResults( digitalEdgeAnalyzer* analyzer, digitalEdgeAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

digitalEdgeAnalyzerResults::~digitalEdgeAnalyzerResults()
{
}

void digitalEdgeAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	// ToDo >> complete / update / check if sufficient later
	// get frame based on index
	Frame frame = GetFrame( frame_index );
	// Frame
	// U64 mData1 - value representing the slope detected for this edge
    // U64 mData2 - not used

	// Clear result stringe before adding the constructed ones
	ClearResultStrings();

	char number_str[ 128 ]; // just the value as string
	char result_short[128]; // max size
	char result_medium[128]; // max size
	char result_long[128]; // max size
	// get mData1 of the frame (U64) as a string with 8-bit length in different displayBase (depending on config later in the GUI)
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, result_short, 128 ); // short result string
	// build result strings depending on data in frame > frame.mData: 0=RISING; 1=FALLING
	switch( frame.mData1 )
	{
		case 0: // RISING
			sprintf( result_short, "[RISE]");
			sprintf( result_medium, "[RISING]");
			sprintf( result_long, "EDGE [RISING]");
			break;
		case 1: // FALLING
			sprintf( result_short, "[FALL]");
			sprintf( result_medium, "[FALLING]");
			sprintf( result_long, "EDGE [FALLING]");
			break;
	}
	// Add result strings
	AddResultString( result_short );
	AddResultString( result_medium );
	AddResultString( result_long );
	
}

void digitalEdgeAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	// ToDo >> complete / update / check if sufficient later
	// start file stream to stream data to
	std::ofstream file_stream( file, std::ios::out );

	// get infos from analyzer
	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	// write header to file stream
	file_stream << "Time [s],Slope,Value" << std::endl;

	// get number of frames as they were identified by the analyzer
	U64 num_frames = GetNumFrames();

	// iterate over these frames
	for( U32 i=0; i < num_frames; i++ )
	{
		// get specific frame to be processed
		Frame frame = GetFrame( i );
		
		// get time in seconds as float converted to string (string since it needs to be written to an ASCII file stream)
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		// get mData1 of the frame (U64) as a string with 8-bit length in different displayBase (depending on config later in the GUI)
		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		char slope_str[128];

		switch( frame.mData1 )
		{
			case 0: // RISING
				sprintf( slope_str, "RISING");
				break;
			case 1: // FALLING
				sprintf( slope_str, "FALLING");
				break;
			default:
				sprintf( slope_str, "ERROR");
				break;
		}

		file_stream << time_str << "," << slope_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void digitalEdgeAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

	char slope_str[128];
		switch( frame.mData1 )
		{
			case 0: // RISING
				sprintf( slope_str, "RISING");
				break;
			case 1: // FALLING
				sprintf( slope_str, "FALLING");
				break;
		}

	AddTabularText( slope_str );
#endif
}

void digitalEdgeAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported
	ClearResultStrings();
    AddResultString( "not supported" );
}

void digitalEdgeAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
	ClearResultStrings();
    AddResultString( "not supported" );
}