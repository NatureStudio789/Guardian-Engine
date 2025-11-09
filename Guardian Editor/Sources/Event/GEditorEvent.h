#ifndef _GE_GEDITOREVENT_H_
#define _GE_GEDITOREVENT_H_
#include <GEngine.h>

namespace GE
{
#define GE_REGISTRY_EVENT(name, publicMembers, privateMembers)\
	class G##name : public GEvent\
	{\
	public:\
		publicMembers\
	\
	private:\
		privateMembers\
	};

	GE_REGISTRY_EVENT(SearchModelAssetEvent, ,
		UINT ModelAssetId;
		std::string ModelAssetName;);
}

#endif