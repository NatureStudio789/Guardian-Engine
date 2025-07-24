#ifndef _GE_GPASS_H_
#define _GE_GPASS_H_
#include "../Sink/GSink.h"

namespace GE
{
	class GUARDIAN_API GPass
	{
	public:
		GPass();
		GPass(const std::string& name);
		GPass(const GPass& other);
		~GPass();

		void SetSinkLinkage(const std::string& sinkName, const std::string& target);

		virtual void Execute();
		virtual void Reset();
		virtual void Finalize();

		const std::string& GetPassName() const noexcept;
		const std::vector<std::shared_ptr<GSink>>& GetSinkList() const noexcept;
		std::shared_ptr<GSource> GetPassSource(const std::string& name);
		std::shared_ptr<GSink> GetPassSink(const std::string& name);

	protected:
		void RegisterSource(std::shared_ptr<GSource> source);
		void RegisterSink(std::shared_ptr<GSink> sink);

		std::string PassName;

		std::vector<std::shared_ptr<GSource>> SourceList;
		std::vector<std::shared_ptr<GSink>> SinkList;
	};
}

#endif