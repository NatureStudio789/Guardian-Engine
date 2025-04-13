#include "GuardianNode.h"
#include "../../Application/Exception/GuardianException.h"

namespace GE
{
	GuardianNode::GuardianNode()
	{
		this->NodeId = GuardianUUID();
		this->NodeName = "";
		this->NodeParent = null;
		this->NodeChildrenList.clear();
	}

	GuardianNode::GuardianNode(const GuardianNode& other)
	{
		this->NodeId = other.NodeId;
		this->NodeName = other.NodeName;
		this->NodeParent = other.NodeParent;
		this->NodeChildrenList = other.NodeChildrenList;
	}

	GuardianNode::~GuardianNode()
	{
		this->NodeId = 0;
		this->NodeName.clear();
		this->NodeParent = null;
		this->NodeChildrenList.clear();
	}

	void GuardianNode::SetNodeId(const GuardianUUID& id)
	{
		this->NodeId = id;
	}

	void GuardianNode::SetNodeName(const GString& name)
	{
		this->NodeName = name;
	}

	void GuardianNode::SetParent(std::shared_ptr<GuardianNode> parent)
	{
		this->NodeParent = parent;
	}

	void GuardianNode::SetParent(GuardianNode* parent)
	{
		this->NodeParent.reset(parent);
	}

	void GuardianNode::AddChildNode(std::shared_ptr<GuardianNode> child)
	{
		if (child->HasParent())
		{
			child->NodeParent->RemoveChildNode(child);
		}

		this->NodeChildrenList[child->GetNodeName()] = child;

		child->SetParent(this);
	}

	void GuardianNode::RemoveChildNode(std::shared_ptr<GuardianNode> child)
	{
		if (!this->HasChildren())
		{
			return;
		}

		GString RemoveKey;
		RemoveKey.clear();
		for (auto& c : this->NodeChildrenList)
		{
			if (c.second == child)
			{
				RemoveKey = c.first;
				break;
			}
		}

		if (RemoveKey.empty())
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No child node found : '{}', with id : '{}', named : '{}'", (unsigned long)child.get(), 
				(unsigned int)child->GetNodeId(), child->GetNodeName()));
		}
		else
		{
			this->NodeChildrenList.erase(RemoveKey);
		}
	}

	void GuardianNode::RemoveChildNode(const GuardianUUID& id)
	{
		if (!this->HasChildren())
		{
			return;
		}

		GString RemoveKey;
		RemoveKey.clear();
		for (auto& child : this->NodeChildrenList)
		{
			if (child.second->GetNodeId() == id)
			{
				RemoveKey = child.first;
				break;
			}
		}

		if (RemoveKey.empty())
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No child node found with id : '{}'", (unsigned int)id));
		}
		else
		{
			this->NodeChildrenList.erase(RemoveKey);
		}
	}

	void GuardianNode::RemoveChildNode(const GString& name)
	{
		if (!this->HasChildren())
		{
			return;
		}

		if (this->NodeChildrenList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No child node found with name : '{}'", name));
		}

		this->NodeChildrenList.erase(name);
	}

	bool GuardianNode::HasParent() const noexcept
	{
		return (bool)this->NodeParent;
	}

	bool GuardianNode::HasChildren() const noexcept
	{
		return !this->NodeChildrenList.empty();
	}

	const GuardianUUID& GuardianNode::GetNodeId() const noexcept
	{
		return this->NodeId;
	}

	const GString& GuardianNode::GetNodeName() const noexcept
	{
		return this->NodeName;
	}

	std::shared_ptr<GuardianNode> GuardianNode::GetNodeParent()
	{
		return this->NodeParent;
	}

	std::shared_ptr<GuardianNode> GuardianNode::GetChildNode(const GString& name)
	{
		if (this->NodeChildrenList.count(name) <= 0)
		{
			throw GUARDIAN_ERROR_EXCEPTION(std::format("No node found with name : '{}' !", name));
		}
		else
		{
			return this->NodeChildrenList[name];
		}
	}

	std::shared_ptr<GuardianNode> GuardianNode::GetChildNode(const GuardianUUID& id)
	{
		for (auto& child : this->NodeChildrenList)
		{
			if (child.second->GetNodeId() == id)
			{
				return child.second;
			}
		}

		throw GUARDIAN_ERROR_EXCEPTION(std::format("No node found with id : '{}' !", (unsigned int)id));
	}

	std::map<GString, std::shared_ptr<GuardianNode>> GuardianNode::GetChildrenNodeList()
	{
		return this->NodeChildrenList;
	}
}
