#ifndef _GE_GSOURCE_H_
#define _GE_GSOURCE_H_
#include "../Task/GTask.h"

namespace GE
{
	class GUARDIAN_API GSource
	{
	public:
		GSource();
		GSource(const std::string& name);
		GSource(const GSource& other);
		virtual ~GSource();

		void InitializeSource(const std::string& name);
		
		virtual void CheckLinkValidate() const;

		const std::string& GetSourceName() const noexcept;
		virtual std::shared_ptr<GApplicable> YieldApplicable();
		virtual std::shared_ptr<GFramebuffer> YieldFramebuffer();

	private:
		std::string SourceName;
	};

	class GUARDIAN_API GDirectFramebufferSource : public GSource
	{
	public:
		GDirectFramebufferSource(const std::string& name, std::shared_ptr<GFramebuffer>& framebuffer) : 
			GSource(name), Framebuffer(framebuffer)
		{

		}
		GDirectFramebufferSource(const GDirectFramebufferSource& other) : 
			GSource(other), Framebuffer(other.Framebuffer)
		{
			this->IsLinked = other.IsLinked;
		}

		void CheckLinkValidate() const override
		{
			
		}
		std::shared_ptr<GFramebuffer> YieldFramebuffer() override
		{
			if (this->IsLinked)
			{
				throw GUARDIAN_ERROR_EXCEPTION("This framebuffer source has been linked!");
			}

			this->IsLinked = true;
			return this->Framebuffer;
		}

		static std::shared_ptr<GDirectFramebufferSource> CreateNewDirectFramebufferSource(
			const std::string& name, std::shared_ptr<GFramebuffer>& framebuffer)
		{
			return std::make_shared<GDirectFramebufferSource>(name, framebuffer);
		}

	private:
		std::shared_ptr<GFramebuffer>& Framebuffer;
		bool IsLinked = false;
	};
	
	template<typename T>
	class GDirectApplicableSource : public GSource
	{
	public:
		GDirectApplicableSource(const std::string& name, std::shared_ptr<T>& applicable) : Applicable(applicable)
		{
			this->InitializeSource(name);
		}
		GDirectApplicableSource(const GDirectApplicableSource& other) : 
			GSource(other), Applicable(other.Applicable)
		{
		}

		void CheckLinkValidate() const override
		{

		}
		std::shared_ptr<GApplicable> YieldApplicable() override
		{
			return this->Applicable;
		}

		static std::shared_ptr<GDirectApplicableSource> CreateNewDirectApplicableSource(
			const std::string& name, std::shared_ptr<GApplicable>& applicable)
		{
			return std::make_shared<GDirectApplicableSource>(name, applicable);
		}

	private:
		std::shared_ptr<T>& Applicable;
	};
}

#endif