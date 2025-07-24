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
		GDirectFramebufferSource() : GSource()
		{
			this->IsLinked = false;
			this->Framebuffer = null;
		}
		GDirectFramebufferSource(const std::string& name, std::shared_ptr<GFramebuffer> framebuffer) : GSource(name)
		{
			this->Framebuffer = framebuffer;
		}
		GDirectFramebufferSource(const GDirectFramebufferSource& other) : GSource(other)
		{
			this->IsLinked = other.IsLinked;
			this->Framebuffer = other.Framebuffer;
		}
		~GDirectFramebufferSource() override
		{
			this->IsLinked = false;
			this->Framebuffer = null;
		}

		void InitializeDirectFramebufferSource(
			const std::string& name, std::shared_ptr<GFramebuffer> framebuffer)
		{
			this->InitializeSource(name);

			this->Framebuffer = framebuffer;
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
			const std::string& name, std::shared_ptr<GFramebuffer> framebuffer)
		{
			return std::make_shared<GDirectFramebufferSource>(name, framebuffer);
		}

	private:
		std::shared_ptr<GFramebuffer> Framebuffer;
		bool IsLinked = false;
	};
	
	template<typename T>
	class GDirectApplicableSource : public GSource
	{
	public:
		GDirectApplicableSource() : GSource()
		{
			this->Applicable = null;
		}
		GDirectApplicableSource(const std::string& name, std::shared_ptr<T> applicable)
		{
			this->InitializeDirectApplicableSource(name, applicable);
		}
		GDirectApplicableSource(const GDirectApplicableSource& other) : GSource(other)
		{
			this->Applicable = other.Applicable;
		}
		~GDirectApplicableSource() override
		{
			this->Applicable = null;
		}

		void InitializeDirectApplicableSource(const std::string& name, std::shared_ptr<T> applicable)
		{
			this->InitializeSource(name);

			this->Applicable = applicable;
		}

		void CheckLinkValidate() const override
		{

		}
		std::shared_ptr<GApplicable> YieldApplicable() override
		{
			return this->Applicable;
		}

		static std::shared_ptr<GDirectApplicableSource> CreateNewDirectApplicableSource(
			const std::string& name, std::shared_ptr<GApplicable> applicable)
		{
			return std::make_shared<GDirectApplicableSource>(name, applicable);
		}

	private:
		std::shared_ptr<T> Applicable;
	};
}

#endif