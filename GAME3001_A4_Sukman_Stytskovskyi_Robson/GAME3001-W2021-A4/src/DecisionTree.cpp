#include "DecisionTree.h"
#include <iostream>

#include "MoveBehindCoverAction.h"
#include "AttackAction.h"
#include "MoveToLOSAction.h"
#include "FleeAction.h"
#include "MoveToPlayerAction.h"
#include "PatrolAction.h"
#include "Skeletons.h"

DecisionTree::DecisionTree()
{
	m_buildTree();
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

void DecisionTree::m_buildTree()
{
	m_FleeNode = new FleeCondition();  // node 0
	m_treeNodeList.push_back(m_FleeNode);

	// add the root node
	m_LOSNode = new LOSCondition();
	AddNode(m_FleeNode, m_LOSNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_LOSNode); // node 1

	m_RadiusNode = new RadiusCondition();
	AddNode(m_LOSNode, m_RadiusNode, LEFT_TREE_NODE);
	m_treeNodeList.push_back(m_RadiusNode); // node 2

	m_CloseCombatNode = new CloseCombatCondition();
	AddNode(m_LOSNode, m_CloseCombatNode, RIGHT_TREE_NODE);
	m_treeNodeList.push_back(m_CloseCombatNode); // node 3

	TreeNode* patrolNode = AddNode(m_RadiusNode, new PatrolAction(), LEFT_TREE_NODE);
	m_treeNodeList.push_back(patrolNode); // node 4

	TreeNode* moveToLOSNode = AddNode(m_RadiusNode, new MoveToLOSAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(moveToLOSNode); // node 5

	TreeNode* moveToPlayerNode = AddNode(m_CloseCombatNode, new MoveToPlayerAction(), LEFT_TREE_NODE);
	m_treeNodeList.push_back(moveToPlayerNode); // node 6

	TreeNode* attackNode = AddNode(m_CloseCombatNode, new AttackAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(attackNode); // node 7

	TreeNode* fleeNode = AddNode(m_FleeNode, new FleeAction(), RIGHT_TREE_NODE);
	m_treeNodeList.push_back(fleeNode);
}
