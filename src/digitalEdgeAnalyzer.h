#ifndef DIGITALEDGE_ANALYZER_H
#define DIGITALEDGE_ANALYZER_H

#include <Analyzer.h>
#include "digitalEdgeAnalyzerSettings.h"
#include "digitalEdgeAnalyzerResults.h"
#include "digitalEdgeSimulationDataGenerator.h"
#include <memory>

class ANALYZER_EXPORT digitalEdgeAnalyzer : public Analyzer2
{
public:
	digitalEdgeAnalyzer();
	virtual ~digitalEdgeAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	digitalEdgeAnalyzerSettings mSettings;
	std::unique_ptr<digitalEdgeAnalyzerResults> mResults;
	AnalyzerChannelData* mInput;

	digitalEdgeSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U64 Trigger;
	U64 mEdgeSlopeAsInt;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //DIGITALEDGE_ANALYZER_H
