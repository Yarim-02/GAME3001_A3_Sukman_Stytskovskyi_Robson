#pragma once
#ifndef __DECISION_TREE__
#define __DECISION_TREE__
#include "Agent.h"
#include "CloseCombatCondition.h"
#include "RangedCombatCondition.h"
#include "MoveBehindCoverCondition.h"
#include "WaitInCoverCondition.h"
#include "IsBehindCoverCondition.h"
#include "LOSCondition.h"
#include "FleeCondition.h"
#include "RadiusCondition.h"
#include "TreeNode.h"
#include "TreeNodeType.h"

class DecisionTree
{
public:
	// constructor and destructor
	DecisionTree(bool isRanged);
	~DecisionTree();

	// getters and setters
	Agent* getAgent() const;
	void setAgent(Agent* agent);

	// convenience functions
	TreeNode* AddNode(TreeNode* parent, TreeNode* child_node, TreeNodeType type);
	void Display();
	void Update();
	
	std::string MakeDecision(); // in order traversal

	
private:
	Agent* m_agent;

	LOSCondition* m_LOSConditionNode;
	FleeCondition* m_FleeConditionNode;
	RadiusCondition* m_RadiusConditionNode;
	CloseCombatCondition* m_CloseCombatConditionNode;
	RangedCombatCondition* m_RangedCombatConditionNode;
	MoveBehindCoverCondition* m_MoveBehindCoverConditionNode;
	IsBehindCoverCondition* m_IsBehindCoverConditionNode;
	WaitInCoverCondition* m_WaitInCoverConditionNode;

	void m_buildTree(bool isRanged);

	bool m_Ranged;

	std::vector<TreeNode*> m_treeNodeList;
};


#endif /* defined (__DECISION_TREE__) */