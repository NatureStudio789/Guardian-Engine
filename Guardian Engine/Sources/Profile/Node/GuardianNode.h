#ifndef _GE_GUARDIANNODE_H_
#define _GE_GUARDIANNODE_H_
#include "../UUID/GuardianUUID.h"

namespace GE
{
	class GUARDIAN_API GuardianNode
	{
	public:
		GuardianNode();
		GuardianNode(const GuardianNode& other);
		~GuardianNode();

		void SetNodeId(const GuardianUUID& id);
		void SetNodeName(const GString& name);

		void SetParent(std::shared_ptr<GuardianNode> parent);
		void SetParent(GuardianNode* parent);

		void AddChildNode(std::shared_ptr<GuardianNode> child);
		void RemoveChildNode(std::shared_ptr<GuardianNode> child);
		void RemoveChildNode(const GuardianUUID& id);
		void RemoveChildNode(const GString& name);

		bool HasParent() const noexcept;
		bool HasChildren() const noexcept;

		const GuardianUUID& GetNodeId() const noexcept;
		const GString& GetNodeName() const noexcept;

		std::shared_ptr<GuardianNode> GetNodeParent();
		std::shared_ptr<GuardianNode> GetChildNode(const GString& name);
		std::shared_ptr<GuardianNode> GetChildNode(const GuardianUUID& id);
		std::map<GString, std::shared_ptr<GuardianNode>> GetChildrenNodeList();

	protected:
		GuardianUUID NodeId;
		GString NodeName;

		std::shared_ptr<GuardianNode> NodeParent;
		std::map<GString, std::shared_ptr<GuardianNode>> NodeChildrenList;
	};
}

#endif