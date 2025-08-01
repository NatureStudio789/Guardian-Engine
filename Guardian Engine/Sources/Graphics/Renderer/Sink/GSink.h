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
		GDirectFramebufferSink(const std::string& name, std::shared_ptr<GFramebuffer>& framebuffer) : Framebuffer(framebuffer)
		{
			this->InitializeSink(name);
		}
		GDirectFramebufferSink(const GDirectFramebufferSink& other) : GSink(other), Framebuffer(other.Framebuffer)
		{
			this->IsLinked = other.IsLinked;
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
			const std::string& name, std::shared_ptr<GFramebuffer>& framebuffer)
		{
			return std::make_shared<GDirectFramebufferSink>(name, framebuffer);
		}

	private:
		std::shared_ptr<GFramebuffer>& Framebuffer;
		bool IsLinked = false;
	};

	class GDirectCameraSink : public GSink
	{
	public:
		GDirectCameraSink(const std::string& name, std::shared_ptr<GCamera>& camera) : Camera(camera)
		{
			this->InitializeSink(name);
		}
		GDirectCameraSink(const GDirectCameraSink& other) : GSink(other), Camera(other.Camera)
		{
			this->IsLinked = other.IsLinked;
		}

		void Apply(std::shared_ptr<GSource> source) override
		{
			this->Camera = source->YieldCamera();
			this->IsLinked = true;
		}
		void CheckLinkValidate() const override
		{
			if (!this->IsLinked)
			{
				throw GUARDIAN_ERROR_EXCEPTION(std::format("Unlinked sink : '{}'", this->GetSinkName()));
			}
		}

		static std::shared_ptr<GDirectCameraSink> CreateNewDirectCameraSink(
			const std::string& name, std::shared_ptr<GCamera>& camera)
		{
			return std::make_shared<GDirectCameraSink>(name, camera);
		}

	private:
		std::shared_ptr<GCamera>& Camera;
		bool IsLinked = false;
	};

	template<typename T>
	class GDirectApplicableSink : public GSink
	{
	public:
		GDirectApplicableSink(const std::string& name, std::shared_ptr<T>& applicable) : Applicable(applicable)
		{
			this->InitializeSink(name);
		}
		GDirectApplicableSink(const GDirectApplicableSink& other) : GSink(other), Applicable(other.Applicable)
		{
			this->IsLinked = other.IsLinked;
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

		static std::shared_ptr<GDirectApplicableSink> CreateNewDirectApplicableSink(
			const std::string& name, std::shared_ptr<T>& applicable)
		{
			return std::make_shared<GDirectApplicableSink>(name, applicable);
		}

	private:
		std::shared_ptr<T>& Applicable;
		bool IsLinked = false;
	};
}

#endif