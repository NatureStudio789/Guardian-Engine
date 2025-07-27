#ifndef _GE_GTASK_H_
#define _GE_GTASK_H_
#include "../Renderable/Mesh/GMeshInstance.h"

namespace GE
{
	class GUARDIAN_API GTask
	{
	public:
		GTask();
		GTask(std::shared_ptr<GStep> step, std::shared_ptr<GRenderable> renderable);
		GTask(const GTask& other);
		~GTask();

		void InitializeTask(
			std::shared_ptr<GStep> step, std::shared_ptr<GRenderable> renderable);

		void Execute();

		static std::shared_ptr<GTask> CreateNewTask(
			std::shared_ptr<GStep> step, std::shared_ptr<GRenderable> renderable)
		{
			return std::make_shared<GTask>(step, renderable);
		}

	private:
		std::shared_ptr<GStep> Step;
		std::shared_ptr<GRenderable> Renderable;
	};
}

#endif