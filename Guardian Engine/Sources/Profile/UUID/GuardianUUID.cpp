#include "GuardianUUID.h"

namespace guardian
{
	static std::random_device RandomDevice;
	static std::mt19937_64 RandomEngine(RandomDevice());
	static std::uniform_int_distribution<uint64_t> UniformDistribution;

	GuardianUUID::GuardianUUID()
	{
		this->UUID = UniformDistribution(RandomEngine);
	}

	GuardianUUID::GuardianUUID(uint64_t uuid)
	{
		this->UUID = uuid;
	}

	GuardianUUID::GuardianUUID(const GuardianUUID& other)
	{
		this->UUID = other.UUID;
	}
}
