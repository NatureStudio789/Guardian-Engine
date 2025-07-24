#include "GSink.h"

namespace GE
{
    GSink::GSink()
    {
        this->SinkName = "";
        this->TargetPassName = "";
        this->OutputName = "";
    }

    GSink::GSink(const std::string& name)
    {
        this->InitializeSink(name);
    }

    GSink::GSink(const GSink& other)
    {
        this->SinkName = other.SinkName;
        this->TargetPassName = other.TargetPassName;
        this->OutputName = other.OutputName;
    }

    GSink::~GSink()
    {
        this->SinkName.clear();
        this->TargetPassName.clear();
        this->OutputName.clear();
    }

    void GSink::InitializeSink(const std::string& name)
    {
        this->SinkName = name;

        {
            if (this->SinkName.empty())
            {
                throw GUARDIAN_ERROR_EXCEPTION("Empty sink name!");
            }
            const bool NameCharactersValid = std::all_of(this->SinkName.begin(), this->SinkName.end(),
            [](char character)
            {
                return std::isalnum(character) || character == '_';
            });
            if (!NameCharactersValid || std::isdigit(this->SinkName.front()))
            {
                throw GUARDIAN_ERROR_EXCEPTION("Invalid sink name!");
            }
        }
    }

    void GSink::SetTarget(std::string passName, std::string outputName)
    {
        {
            if (passName.empty())
            {
                throw GUARDIAN_ERROR_EXCEPTION("Empty pass name!");
            }

            const bool NameCharactersValid = std::all_of(passName.begin(), passName.end(),
            [](char character)
            {
                return std::isalnum(character) || character == '_';
            });
            if (passName != "$" && (!NameCharactersValid || std::isdigit(passName.front())))
            {
                throw GUARDIAN_ERROR_EXCEPTION("Invalid sink name!");
            }

            this->TargetPassName = passName;
        }

        {
            if (outputName.empty())
            {
                throw GUARDIAN_ERROR_EXCEPTION("Empty output name!");
            }

            const bool NameCharactersValid = std::all_of(outputName.begin(), outputName.end(),
            [](char character)
            {
                return std::isalnum(character) || character == '_';
            });
            if (!NameCharactersValid || std::isdigit(outputName.front()))
            {
                throw GUARDIAN_ERROR_EXCEPTION("Invalid sink name!");
            }

            this->OutputName = outputName;
        }
    }

    void GSink::Apply(std::shared_ptr<GSource> source)
    {

    }

    void GSink::CheckLinkValidate() const
    {

    }

    const std::string& GSink::GetSinkName() const noexcept
    {
        return this->SinkName;
    }

    const std::string& GSink::GetTargetPassName() const noexcept
    {
        return this->TargetPassName;
    }

    const std::string& GSink::GetOutputName() const noexcept
    {
        return this->OutputName;
    }
}
