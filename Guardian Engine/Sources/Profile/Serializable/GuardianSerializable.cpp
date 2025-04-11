#include "GuardianSerializable.h"
#include "../../Application/Exception/GuardianException.h"

namespace GE
{
    void GuardianSerializable::Serialize(const GString& filePath, bool changeFilePath)
    {
        if (changeFilePath)
        {
            this->InitializeFile(filePath);
        }

        const auto& data = this->WriteSerializationData();
        if (data.empty())
        {
            throw GUARDIAN_ERROR_EXCEPTION("Failed to serialize! - This serialization data is empty!");
        }

        this->SerializationData = data;

        std::stringstream SerializationDataStream(this->SerializationData);
        std::ofstream SerializationFileCreator(filePath);

        SerializationFileCreator << SerializationDataStream.rdbuf();
    }

    void GuardianSerializable::Serialize()
    {
        this->Serialize(this->FileSpecificPath, false);
    }

    void GuardianSerializable::Deserialize(const GString& filePath, bool changeFilePath)
    {
        GString ReadFilePath;
        if (changeFilePath)
        {
            this->InitializeFile(filePath);
            ReadFilePath = this->FileSpecificPath;
        }
        else
        {
            ReadFilePath = filePath;
        }

        std::ifstream SerializationFile(ReadFilePath);
        std::stringstream SerializationFileStringStream;
        SerializationFileStringStream << SerializationFile.rdbuf();

        this->SerializationData = SerializationFileStringStream.str();

        this->Deserialize();
    }

    void GuardianSerializable::Deserialize()
    {
        this->ReadSerializationData(this->SerializationData);
    }
    
    const GString& GuardianSerializable::GetSerializeData() const
    {
        return this->SerializationData;
    }
}
