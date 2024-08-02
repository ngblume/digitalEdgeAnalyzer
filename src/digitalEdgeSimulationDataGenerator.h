#ifndef DIGITALEDGE_SIMULATION_DATA_GENERATOR
#define DIGITALEDGE_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class digitalEdgeAnalyzerSettings;

class digitalEdgeSimulationDataGenerator
{
public:
	digitalEdgeSimulationDataGenerator();
	~digitalEdgeSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, digitalEdgeAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	digitalEdgeAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //DIGITALEDGE_SIMULATION_DATA_GENERATOR