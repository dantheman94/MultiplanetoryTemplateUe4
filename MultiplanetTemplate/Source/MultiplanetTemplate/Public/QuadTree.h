#pragma once

#include "CoreMinimal.h"

#include "QuadTreeNode.h"

class QuadTree
{
protected:

	// The top most node of our tree
	TSharedPtr<QuadTreeNode> _pRootNode;

	// The number of layers between top node & bottom
	int _iMaxDistance;

public:

	QuadTree() {}
	~QuadTree() {}

	void InitializeTree(const int MaxDistance, FVector2D Minimum, FVector2D Maximum);

	// Get a shared pointer to the top most node in the QuadTree
	TSharedPtr<QuadTreeNode> GetRootNode() { return _pRootNode; }

	// Add an element to the QuadTree
	void AddElement(AActor* Actor);

	// Draw debug boxes around each node in the QuadTree
	void DebugDrawBoxes(UWorld* World);

	// Get the deepest node which holds a position
	TSharedPtr<QuadTreeNode> GetNode(FVector Positon);

	// Get the distance from the top to the bottom of the tree
	int GetMaxDistance() const { return _iMaxDistance; }

private:

	// Create all nodes within the QuadTree
	void InitializeNodes(TSharedPtr<QuadTreeNode> ParentNode, FVector2D Minimum, FVector2D Maximum);

	// Create a node in the QuadTree
	TSharedPtr<QuadTreeNode> CreateNode(TSharedPtr<QuadTreeNode> ParentNode, FVector2D Minimum, FVector2D Maximum);

};