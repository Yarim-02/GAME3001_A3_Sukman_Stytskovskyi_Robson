#include "DecisionTree.h"
#include <iostream>

#include "MoveBehindCoverAction.h"
#include "WaitInCoverAction.h"
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
	m_LOSNode->setLOS(m_agent->hasLOS());
	m_FleeNode->setFleeing(m_agent->getFleeing());
	m_RadiusNode->setIsWithinRadius(m_agent->inDR());

	if (m_Ranged)
	{
		m_RangedCombatNode->setIsWithinCombatRange(static_cast<SkeletonRanged*>(m_agent)->getRangedCombatRange());
		
	}
	else if (!m_Ranged)
		m_CloseCombatNode->setIsWithinCombatRange(static_cast<SkeletonClose*>(m_agent)->getCloseCombatRange());
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
	m_FleeNode = new FleeCondition();  
	m_treeNodeList.push_back(m_FleeNode);

	TreeNode* fleeNode = AddNode(m_FleeNode, new FleeAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(fleeNode);

	if (isRanged == true)
	{
		m_MoveBehindCoverNode = new MoveBehindCoverCondition();
		AddNode(m_FleeNode, m_MoveBehindCoverNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_MoveBehindCoverNode);

		TreeNode* coverNode = AddNode(m_MoveBehindCoverNode, new MoveBehindCoverAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(coverNode);

		m_LOSNode = new LOSCondition();
		AddNode(m_MoveBehindCoverNode, m_LOSNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_LOSNode);
	}
	else if (isRanged == false)
	{
		m_LOSNode = new LOSCondition();
		AddNode(m_FleeNode, m_LOSNode, LEFT_TREE_NODE);
		m_treeNodeList.push_back(m_LOSNode);
	}

	m_RadiusNode = new RadiusCondition();
	AddNode(m_LOSNode, m_RadiusNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusNode); 

	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	m_treeNodeList.push_back(patrolNode); 

	TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(moveToLOSNode);

	if (isRanged == true)
	{
		TreeNode* moveToRangeNode = AddNode(m_LOSNode, new MoveToRangeAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(moveToRangeNode);

		m_RangedCombatNode = new RangedCombatCondition();
		AddNode(moveToRangeNode, m_RangedCombatNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_RangedCombatNode);

		TreeNode* attackNode = AddNode(m_RangedCombatNode, new AttackAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(attackNode);
	}
	else if (isRanged == false)
	{
		m_CloseCombatNode = new CloseCombatCondition();
		AddNode(m_LOSNode, m_CloseCombatNode, RIGHT_TREE_NODE);
		m_treeNodeList.push_back(m_CloseCombatNode);

		TreeNode* moveToPlayerNode = AddNode(m_CloseCombatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
		m_treeNodeList.push_back(moveToPlayerNode);

		TreeNode* attackNode = AddNode(m_CloseCombatNode, new AttackAction(), RIGHT_TREE_NODE);
		m_treeNodeList.push_back(attackNode);
	}
	
}
