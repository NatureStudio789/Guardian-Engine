#include "GUUID.h"

namespace GE
{
	static std::random_device RandomDevice;
	static std::mt19937_64 RandomEngine(RandomDevice());
	static std::uniform_int_distribution<unsigned long long> UniformDistribution;


	GUUID::GUUID()
	{
		this->Generate();
	}

	GUUID::GUUID(unsigned long long uuid)
	{
		this->UUID = uuid;
	}

	GUUID::GUUID(const GUUID& other)
	{
		this->UUID = other.UUID;
	}

	GUUID::~GUUID()
	{
		this->UUID = 0;
	}

	void GUUID::Generate()
	{
		this->UUID = UniformDistribution(RandomEngine);
	}

	GUUID::operator unsigned long long() const
	{
		return this->UUID;
	}
}
