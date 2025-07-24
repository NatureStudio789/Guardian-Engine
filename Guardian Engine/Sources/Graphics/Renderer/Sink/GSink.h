#ifndef _GE_GSINK_H_
#define _GE_GSINK_H_
#include "../Source/GSource.h"

namespace GE
{
	class GUARDIAN_API GSink
	{
	public:
		GSink();
		GSink(const std::string& name);
		GSink(const GSink& other);
		virtual ~GSink();

		void InitializeSink(const std::string& name);
		void SetTarget(std::string passName, std::string outputName);
		virtual void Apply(std::shared_ptr<GSource> source);
		virtual void CheckLinkValidate() const;

		const std::string& GetSinkName() const noexcept;
		const std::string& GetTargetPassName() const noexcept;
		const std::string& GetOutputName() const noexcept;

	private:
		std::string SinkName;
		std::string TargetPassName;
		std::string OutputName;
	};

	class GDirectFramebufferSink : public GSink
	{
	public:
		GDirectFramebufferSink() : GSink()
		{
			this->IsLinked = false;
			this->Framebuffer = null;
		}
		GDirectFramebufferSink(const std::string& name, std::shared_ptr<GFramebuffer> framebuffer)
		{
			this->InitializeDirectFramebufferSink(name, framebuffer);
		}
		GDirectFramebufferSink(const GDirectFramebufferSink& other) : GSink(other)
		{
			this->Framebuffer = other.Framebuffer;
			this->IsLinked = other.IsLinked;
		}
		~GDirectFramebufferSink() override
		{
			this->Framebuffer = null;
			this->IsLinked = false;
		}

		void InitializeDirectFramebufferSink(const std::string& name, std::shared_ptr<GFramebuffer> framebuffer)
		{
			this->InitializeSink(name);

			this->Framebuffer = framebuffer;
		}

		void Apply(std::shared_ptr<GSource> source) override
		{
			this->Framebuffer = source->YieldFramebuffer();
			this->IsLinked = true;
		}
		void CheckLinkValidate() const override
		{
			if (!this->IsLinked)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("Unlinked sink : '{}'", this->GetSinkName()));
			}
		}

		static std::shared_ptr<GDirectFramebufferSink> CreateNewDirectFramebufferSink(
			const std::string& name, std::shared_ptr<GFramebuffer> framebuffer)
		{
			return std::make_shared<GDirectFramebufferSink>(name, framebuffer);
		}

	private:
		std::shared_ptr<GFramebuffer> Framebuffer;
		bool IsLinked = false;
	};

	template<typename T>
	class GDirectApplicableSink : public GSink
	{
	public:
		GDirectApplicableSink() : GSink()
		{
			this->IsLinked = false;
			this->Applicable = null;
		}
		GDirectApplicableSink(const std::string& name, std::shared_ptr<T> applicable)
		{
			this->InitializeDirectFramebufferSink(name, applicable);
		}
		GDirectApplicableSink(const GDirectFramebufferSink& other) : GSink(other)
		{
			this->Applicable = other.Applicable;
			this->IsLinked = other.IsLinked;
		}
		~GDirectApplicableSink() override
		{
			this->Applicable = null;
			this->IsLinked = false;
		}

		void InitializeDirectApplicableSink(const std::string& name, std::shared_ptr<T> applicable)
		{
			this->InitializeSink(name);

			this->Applicable = applicable;
		}

		void Apply(std::shared_ptr<GSource> source) override
		{
			this->Applicable = source->YieldApplicable();
			this->IsLinked = true;
		}
		void CheckLinkValidate() const override
		{
			if (!this->IsLinked)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("Unlinked sink : '{}'", this->GetSinkName()));
			}
		}

		static std::shared_ptr<GDirectFramebufferSink> CreateNewDirectApplicableSink(
			const std::string& name, std::shared_ptr<T> applicable)
		{
			return std::make_shared<GDirectFramebufferSink>(name, applicable);
		}

	private:
		std::shared_ptr<T> Applicable;
		bool IsLinked = false;
	};
}

#endif