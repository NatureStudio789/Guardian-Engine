#include "GuardianThread.h"

namespace GE
{
	GuardianThread::GuardianThread()
	{
		this->ThreadObject = {};
	}

	GuardianThread::GuardianThread(GuardianThread& other)
	{
		this->ThreadObject.swap(other.ThreadObject);
	}

	GuardianThread::~GuardianThread()
	{
		this->ThreadObject = {};
	}

	void GuardianThread::JoinThread()
	{
		if (this->ThreadObject.joinable())
		{
			this->ThreadObject.join();
		}
	}

	void GuardianThread::DetachMainThread()
	{
		this->ThreadObject.detach();
	}
}
