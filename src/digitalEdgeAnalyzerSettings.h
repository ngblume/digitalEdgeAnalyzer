#ifndef DIGITALEDGE_ANALYZER_SETTINGS
#define DIGITALEDGE_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class digitalEdgeAnalyzerSettings : public AnalyzerSettings
{
public:
	digitalEdgeAnalyzerSettings();
	virtual ~digitalEdgeAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel; // "Channel" object (channel which the LLA operates on)
	double mEdgeSlope; // slope of the edge to look for (to be selected from list, stored as number[double])

protected:
	AnalyzerSettingInterfaceChannel	mInputChannelInterface; // interface fct (type: "Channel") for settings this parameter
	AnalyzerSettingInterfaceNumberList	mEdgeSlopeInterface; // interface fct (type: "NumberList") for settings this parameter
};

#endif //DIGITALEDGE_ANALYZER_SETTINGS
