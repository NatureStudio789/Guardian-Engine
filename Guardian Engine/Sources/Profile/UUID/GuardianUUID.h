#ifndef _GE_GUARDIANUUID_H_
#define _GE_GUARDIANUUID_H_
#include "../Core/GuardianCore.h"

namespace guardian
{
	class GUARDIAN_API GuardianUUID
	{
	public:
		GuardianUUID();
		GuardianUUID(uint64_t uuid);
		GuardianUUID(const GuardianUUID& other);
		~GuardianUUID() = default;

		operator uint64_t() const { return this->UUID; }

	private:
		uint64_t UUID;
	};
}

namespace std
{
	template<>
	struct hash<guardian::GuardianUUID>
	{
		size_t operator()(const guardian::GuardianUUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};
}

#endif