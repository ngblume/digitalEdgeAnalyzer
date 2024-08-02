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

	
	Channel mInputChannel;
	U32 mBitRate;

protected:
	AnalyzerSettingInterfaceChannel	mInputChannelInterface;
	AnalyzerSettingInterfaceInteger	mBitRateInterface;
};

#endif //DIGITALEDGE_ANALYZER_SETTINGS
