#ifndef _GE_GUARDIANSERIALIZABLE_H_
#define _GE_GUARDIANSERIALIZABLE_H_
#include "../File/GuardianFile.h"

namespace GE
{
	class GUARDIAN_API GuardianSerializable : public GuardianFile
	{
	public:
		GuardianSerializable() = default;
		virtual ~GuardianSerializable() = default;

		void Serialize(const GString& filePath, bool changeFilePath = true);
		void Serialize();
		void Deserialize(const GString& filePath, bool changeFilePath = true);
		void Deserialize();

		const GString& GetSerializeData() const;

	protected:
		virtual const GString WriteSerializationData() = 0;
		virtual void ReadSerializationData(const GString& serializationData) = 0;

	private:
		GString SerializationData;
	};
}

#endif