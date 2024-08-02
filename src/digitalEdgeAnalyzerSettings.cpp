#include "digitalEdgeAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


digitalEdgeAnalyzerSettings::digitalEdgeAnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ), mEdgeSlope(  ) // default values for settings GUI [no channel, no value selected from list]
{
	// cusotmize interface of analyzer with title and tooltips as well as selectable options, etc.
	mInputChannelInterface.SetTitleAndTooltip( "inputChannel", "inputChannel this digitalEdgeAnalyzer operates on..." );
	mInputChannelInterface.SetChannel( mInputChannel );

	// selectable options
	mEdgeSlopeInterface.SetTitleAndTooltip( "edgeSlope", "Specify the slope of the edge to llok for" );
    mEdgeSlopeInterface.AddNumber( 0, "RISING", "rising slope at edge" );
    mEdgeSlopeInterface.AddNumber( 1, "FALLING", "falling slope at edge" );
	mEdgeSlopeInterface.AddNumber( 2, "EITHER", "rising OR falling slope at edge" );
    mEdgeSlopeInterface.SetNumber( mEdgeSlope );

	// add the interface components to the interface
	AddInterface( &mInputChannelInterface );
	AddInterface( &mEdgeSlopeInterface );

	// add some export options (??)
	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	// clear channels and add only relevant channels
	ClearChannels();
	AddChannel( mInputChannel, "inputChannel", false );
}

digitalEdgeAnalyzerSettings::~digitalEdgeAnalyzerSettings()
{
}

bool digitalEdgeAnalyzerSettings::SetSettingsFromInterfaces()
{
	// get values from interface as set by user and store in analyzer object
	mInputChannel = mInputChannelInterface.GetChannel(); 
	mEdgeSlope = mEdgeSlopeInterface.GetNumber();

	ClearChannels();
	AddChannel( mInputChannel, "inputChannel", true );

	return true;
}

void digitalEdgeAnalyzerSettings::UpdateInterfacesFromSettings()
{
	// update values from user configuration in analyzer if changed later on
	mInputChannelInterface.SetChannel( mInputChannel );
	mEdgeSlopeInterface.SetNumber( mEdgeSlope );
}

void digitalEdgeAnalyzerSettings::LoadSettings( const char* settings )
{
	// provide means to load the settings from serlized form
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mEdgeSlope;

	ClearChannels();
	AddChannel( mInputChannel, "inputChannel", true );

	// Update values in interface with loaded values
	UpdateInterfacesFromSettings();
}

const char* digitalEdgeAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mEdgeSlope;

	// return serialized from of settings to be stored
	return SetReturnString( text_archive.GetString() );
}
