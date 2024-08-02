#include "digitalEdgeAnalyzer.h"
#include "digitalEdgeAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

digitalEdgeAnalyzer::digitalEdgeAnalyzer()
:	Analyzer2(),  
	mSettings(),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( &mSettings );
	// Provide frames as V2 as well, to allow post-processing by HLAs
    UseFrameV2();
}

digitalEdgeAnalyzer::~digitalEdgeAnalyzer()
{
	KillThread();
}

void digitalEdgeAnalyzer::SetupResults()
{
	// SetupResults is called each time the analyzer is run. Because the same instance can be used for multiple runs, we need to clear the results each time.
	mResults.reset(new digitalEdgeAnalyzerResults( this, &mSettings ));
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings.mInputChannel );
}

void digitalEdgeAnalyzer::WorkerThread()
{	
	mSampleRateHz = GetSampleRate();
	mInput = GetAnalyzerChannelData( mSettings.mInputChannel );
	mEdgeSlopeAsInt = int( mSettings.mEdgeSlope );

	bool triggerConditionMet = false;
	int currentTriggerSlope = -1;

	for( ; ; )
	{
		// reset helper variables
		triggerConditionMet = false;
		currentTriggerSlope = -1;

		// go to next edge
		mInput->AdvanceToNextEdge();

		// determine slope of edge
		if( mInput->GetBitState() == BIT_HIGH )
			// transition was RISING, since BIT is now HIGH (and must therefore have been LOW before [would not be edge otherwise])
			currentTriggerSlope = 0;
		else if ( mInput->GetBitState() == BIT_LOW )
			// transition was FALLING, since BIT is now LOW (and must therefore have been HIGH before [would not be edge otherwise])
			currentTriggerSlope = 1;
		else
			// weird stuff happened > bit is neither HIGH nor LOW !!!!
			currentTriggerSlope = -1;
		
		if( (mEdgeSlopeAsInt == 0) && (currentTriggerSlope == 0) ) // looking for RISING AND found RISING >> condition met !!
			triggerConditionMet = true;
		else if( (mEdgeSlopeAsInt == 1) && (currentTriggerSlope == 1) ) // looking for FALLING AND found FALLING >> condition met !!
			triggerConditionMet = true;
		else if( (mEdgeSlopeAsInt == 2) && ( (currentTriggerSlope == 0) || (currentTriggerSlope == 1) ) ) // looking for EITHER AND found RISING OR FALLING >> condition met !!
			triggerConditionMet = true;
		else
			triggerConditionMet = false;
		
		// add frame IF trigger condition was met
		if( triggerConditionMet == true )
		{
			// add marker, direction of arrow dependiung on direction of slope
			if ( currentTriggerSlope == 0 ) // RISING
				mResults->AddMarker( mInput->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings.mInputChannel );
			else if ( currentTriggerSlope == 1 ) // FALLING
				mResults->AddMarker( mInput->GetSampleNumber(), AnalyzerResults::DownArrow, mSettings.mInputChannel );
			else // something weird happened > mark with ErrorDot
				mResults->AddMarker( mInput->GetSampleNumber(), AnalyzerResults::ErrorDot, mSettings.mInputChannel );
			
			// Frame (v1)
			Frame frame;
			frame.mData1 = currentTriggerSlope;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = mInput->GetSampleNumber();
			if ( !mInput->WouldAdvancingCauseTransition( 20 ) )
				frame.mEndingSampleInclusive = mInput->GetSampleNumber() + 20; // end is 20 samples further than edge > for proper diplaying
			else if ( !mInput->WouldAdvancingCauseTransition( 5 ) )
				frame.mEndingSampleInclusive = mInput->GetSampleNumber() + 5; // end is 5 samples further
			else
				frame.mEndingSampleInclusive = mInput->GetSampleNumber() + 1; // end is 1 sample further > bare minimum
			// add Frame V1 to results (NOT commited yet)
			mResults->AddFrame( frame ); 

			// FRAME v2
			FrameV2 frame_v2;
			if ( currentTriggerSlope == 0 ) // RISING
				frame_v2.AddString( "slope", "RISING");
			else if ( currentTriggerSlope == 1 ) // FALLING
				frame_v2.AddString( "slope", "FALLING");
			else
				frame_v2.AddString( "slope", "ERROR");
			frame_v2.AddInteger( "value", currentTriggerSlope);
			// add 
			mResults->AddFrameV2( frame_v2, "digitalEdge", frame.mStartingSampleInclusive, frame.mEndingSampleInclusive );

			mResults->CommitResults();
			// ReportProgress:
			// we only checked until this sample. If there were another less than 5 samples away, this will cause error
			// maybe check ahead with "bool WouldAdvancingCauseTransition( U32 num_samples );" ??
			ReportProgress( frame.mStartingSampleInclusive ); 
		}
	}
}

bool digitalEdgeAnalyzer::NeedsRerun()
{
	return false;
}

U32 digitalEdgeAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), &mSettings );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 digitalEdgeAnalyzer::GetMinimumSampleRateHz()
{
	return 25000; // Saleae minimum
}

const char* digitalEdgeAnalyzer::GetAnalyzerName() const
{
	return "digitalEdgeAnalyzer";
}

const char* GetAnalyzerName()
{
	return "digitalEdgeAnalyzer";
}

Analyzer* CreateAnalyzer()
{
	return new digitalEdgeAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}