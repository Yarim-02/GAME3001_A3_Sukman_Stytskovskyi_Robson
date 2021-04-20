#include "DecisionTree.h"
#include <iostream>

#include "MoveBehindCoverAction.h"
#include "WaitInCoverAction.h"
#include "IsBehindCoverCondition.h"
#include "WaitInCoverAction.h"
#include "LeaveCoverAction.h"
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "MoveToRangeAction.h"
#include "MoveToPlayerAction.h"
#include "FleeAction.h"
#include "PatrolAction.h"
#include "Skeletons.h"

DecisionTree::DecisionTree(bool isRanged)
{
	m_Ranged = isRanged;
	m_buildTree(isRanged);
}

DecisionTree::~DecisionTree()
= default;

Agent* DecisionTree::getAgent() const
{
	return m_agent;
}

void DecisionTree::setAgent(Agent* agent)
{
	m_agent = agent;
}

TreeNode* DecisionTree::AddNode(TreeNode* parent, TreeNode* child_node, const TreeNodeType type)
{
	switch(type)
	{
	case LEFT_TREE_NODE:
		parent->Left = child_node;
		break;
	case RIGHT_TREE_NODE:
		parent->Right = child_node;
		break;
	}

	child_node->Parent = parent; // for backwards traversal
	
	return child_node;
}

void DecisionTree::Display()
{
	for (auto* node : m_treeNodeList)
	{
		std::cout << node->name << std::endl;
	}
}

void DecisionTree::Update()
{
	m_LOSConditionNode->setLOS(m_agent->hasLOS());
	m_FleeConditionNode->setFleeing(m_agent->getFleeing());
	m_RadiusConditionNode->setIsWithinRadius(m_agent->inDR());

	if (m_Ranged)
	{
		m_RangedCombatConditionNode->setIsWithinCombatRange(static_cast<SkeletonRanged*>(m_agent)->getRangedCombatRange());
		m_MoveBehindCoverConditionNode->setGotHit(static_cast<SkeletonRanged*>(m_agent)->getTakingDamage());
		m_WaitInCoverConditionNode->setIsTimerOut(static_cast<SkeletonRanged*>(m_agent)->getIsTimerOut()); // still need to trigger
		m_IsBehindCoverConditionNode->setBehindCover(static_cast<SkeletonRanged*>(m_agent)->getIsBehindCover()); //still need to trigger
		
	}
	else if (!m_Ranged)
		m_CloseCombatConditionNode->setIsWithinCombatRange(static_cast<SkeletonClose*>(m_agent)->getCloseCombatRange());
}

// in-order traversal
std::string DecisionTree::MakeDecision()
{
	Update();
	
	auto currentNode = m_treeNodeList[0]; // start at root node

	while(!currentNode->isLeaf)
	{
		currentNode = (currentNode->data) ? (currentNode->Right) : (currentNode->Left);
	}

	return currentNode->name;
}

void DecisionTree::m_buildTree(bool isRanged)
{
	m_FleeConditionNode = new FleeCondition();  
	m_treeNodeList.push_back(m_FleeConditionNode);

	TreeNode* fleeNode = AddNode(m_FleeConditionNode, new FleeAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(fleeNode);

	if (isRanged == true)
	{
		m_MoveBehindCoverConditionNode = new MoveBehindCoverCondition();
		AddNode(m_FleeConditionNode, m_MoveBehindCoverConditionNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_MoveBehindCoverConditionNode);

		m_IsBehindCoverConditionNode = new IsBehindCoverCondition();
		AddNode(m_MoveBehindCoverConditionNode, m_IsBehindCoverConditionNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_IsBehindCoverConditionNode);

		TreeNode* coverNode = AddNode(m_IsBehindCoverConditionNode, new MoveBehindCoverAction(), LEFT_TREE_NODE);
		m_treeNodeList.push_back(coverNode);

		m_WaitInCoverConditionNode = new WaitInCoverCondition();
		AddNode(m_IsBehindCoverConditionNode, m_WaitInCoverConditionNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_WaitInCoverConditionNode);

		TreeNode* waitNode = AddNode(m_WaitInCoverConditionNode, new WaitInCoverAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(waitNode);

		TreeNode* leaveCoverNode = AddNode(m_WaitInCoverConditionNode, new LeaveCoverAction(), LEFT_TREE_NODE);
		m_treeNodeList.push_back(leaveCoverNode);
		
		m_LOSConditionNode = new LOSCondition();
		AddNode(m_MoveBehindCoverConditionNode, m_LOSConditionNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_LOSConditionNode);
	}
	else if (isRanged == false)
	{
		m_LOSConditionNode = new LOSCondition();
		AddNode(m_FleeConditionNode, m_LOSConditionNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_LOSConditionNode);
	}

	m_RadiusConditionNode = new RadiusCondition();
	AddNode(m_LOSConditionNode, m_RadiusConditionNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusConditionNode); 

	TreeNode* patrolNode = AddNode(m_RadiusConditionNode, new PatrolAction(), LEFT_TREE_NODE);
	m_treeNodeList.push_back(patrolNode); 

	TreeNode* moveToLOSNode = AddNode(m_RadiusConditionNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(moveToLOSNode);

	if (isRanged == true)
	{
		m_RangedCombatConditionNode = new RangedCombatCondition();
		AddNode(m_LOSConditionNode, m_RangedCombatConditionNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_RangedCombatConditionNode);

		TreeNode* moveToRangeNode = AddNode(m_RangedCombatConditionNode, new MoveToRangeAction(), LEFT_TREE_NODE);
		m_treeNodeList.push_back(moveToRangeNode);

		TreeNode* attackNode = AddNode(m_RangedCombatConditionNode, new AttackAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(attackNode);
		
	}
	else if (isRanged == false)
	{
		m_CloseCombatConditionNode = new CloseCombatCondition();
		AddNode(m_LOSConditionNode, m_CloseCombatConditionNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_CloseCombatConditionNode);

		TreeNode* moveToPlayerNode = AddNode(m_CloseCombatConditionNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
		m_treeNodeList.push_back(moveToPlayerNode);

		TreeNode* attackNode = AddNode(m_CloseCombatConditionNode, new AttackAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(attackNode);
	}
	
}
