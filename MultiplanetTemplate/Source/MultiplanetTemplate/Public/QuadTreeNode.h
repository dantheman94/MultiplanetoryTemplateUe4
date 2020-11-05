#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Containers/Array.h"
#include "SharedPointer.h"
#include "WeakObjectPtr.h"

enum ENodePosition
{
	BottomRight = 0,
	BottomLeft = 1,
	TopRight = 2,
	TopLeft = 3,
	Root = 4
};

class QuadTreeNode
{
public:

	QuadTreeNode() {}
	QuadTreeNode(const QuadTreeNode& copy); // Shallow copies a QuadtreeNode
	~QuadTreeNode() {}

protected:

	/** The bounding box for this node. */
	TSharedPtr<FBox2D> _pBoundingBox;

	/** The parent node to this node. */
	TWeakPtr<QuadTreeNode> _pParentNode;

	/** Elements held within this node. */
	TArray<AActor*> _pElements;

	/** Child nodes held by this node. */
	TArray<TSharedPtr<QuadTreeNode>> _pChildNodes;

	/** The position of this node, relevant to its parent. */
	ENodePosition _eNodePosition;

public:

	/** Set the bounding box for this node. */
	void SetBoundingBox(const FVector2D Minimum, const FVector2D Maximum) { _pBoundingBox = MakeShared<FBox2D>(Minimum, Maximum); }

	/** Get the bounding box for this node. */
	TSharedPtr<FBox2D> GetBoundingBox() { return _pBoundingBox; }

	/** Set the parent node for this node. */
	void SetParentNode(TWeakPtr<QuadTreeNode> ParentNode) { this->_pParentNode = ParentNode; }

	/** Get the parent node to this node. */
	TWeakPtr<QuadTreeNode> GetParentNode() { return _pParentNode; }

	/** Add a child node to this node. */
	void AddChildNode(TSharedPtr<QuadTreeNode> Node) { this->_pChildNodes.Add(Node); }

	/** Get child nodes belonging to this node. */
	TArray<TSharedPtr<QuadTreeNode>> GetChildNodes() { return _pChildNodes; }

	/** Add an element to this node. */
	void AddElement(AActor* Actor);

	/** Get elements belonging to this node. */
	TArray<AActor*> GetElements() { return _pElements; }

	/** Draw a debug box to outline where the box is. */
	void DrawBoxAroundNode(UWorld* World, FColor Colour);

	/** Get the distance from the root node of the tree. */
	int GetDistance();

	/** Find if a given position is inside this node. */
	bool IsPositionInsideNode(FVector Position);

	/** Get the node which contains a given position. */
	TSharedPtr<QuadTreeNode> GetNode(FVector Position);

	/** Get all elements belong to this node & elements which are contained within its children. */
	TArray<AActor*> GetAllElements();

	/** Set the value of a node position member variable. */
	void SetNodePosition(ENodePosition Position) { _eNodePosition = Position; }

	/** Get a copy of the node position member variable. */
	ENodePosition GetNodePosition() const { return _eNodePosition; }

	/** Check if this node has any children nodes. */
	bool HasChildrenNodes() const { return _pChildNodes.Num() > 0; }

	/** Check if this node has any elements. */
	bool HasElements() const { return _pElements.Num() > 0; }
	
	/** Determines whether or not a node in this element. */
	void GenerateElementInNode(UWorld* World, const FVector2D MaximumDistanceBetweenActors);

private:

	/** Get the root node of this QuadTree. */
	TSharedPtr<QuadTreeNode> GetRootNode();

	/** Get the node which contains a given position. */
	TSharedPtr<QuadTreeNode> GetNodeContainingPosition(FVector2D Position);

};