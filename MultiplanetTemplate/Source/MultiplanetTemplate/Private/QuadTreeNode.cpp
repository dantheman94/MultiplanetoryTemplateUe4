#include "QuadtreeNode.h"
#include "DrawDebugHelpers.h"
#include "GenericPlatformMath.h"

/** Shallow copies a QuadtreeNode. */
QuadTreeNode::QuadTreeNode(const QuadTreeNode& copy)
{
	SetParentNode(copy._pParentNode);
	_pBoundingBox = copy._pBoundingBox;
	_pElements = copy._pElements;
	_pChildNodes = copy._pChildNodes;
}

/** Add an element to this node. */
void QuadTreeNode::AddElement(AActor* Actor)
{
	// Explore each child belonging to this node
	for (auto childNode : _pChildNodes)
	{
		FVector2D position = FVector2D(Actor->GetActorLocation().X, Actor->GetActorLocation().Z);

		// If the position lies within the bounding box of this child
		if (childNode->GetBoundingBox()->IsInside(position))
		{
			// If the child has children of its own
			if (childNode->HasChildrenNodes())
			{
				// Explore their children nodes to get the deepest position in the tree
				childNode->AddElement(Actor);
				return;
			}

			// If this node has no children, we can't be any more accurate
			else
			{
				childNode->_pElements.Add(Actor);
				return;
			}
		}
	}

	// Wasn't inside a child node, probably on a boundary, must be in this node...
	_pElements.Add(Actor);
	return;
}

void QuadTreeNode::DrawBoxAroundNode(UWorld* World, FColor Colour)
{
	FVector center = FVector(_pBoundingBox->GetCenter(), 0);
	FVector extent = FVector(_pBoundingBox->GetExtent(), 0);

	DrawDebugBox(World, center, extent, Colour);

	for (auto node : _pChildNodes)	
		node->DrawBoxAroundNode(World, Colour);	
}

/** Get the distance from the root node of the tree. */
int QuadTreeNode::GetDistance()
{
	TWeakPtr<QuadTreeNode> parent = GetParentNode();
	int i = 0;
	while (parent.IsValid() && parent != nullptr)
	{
		i++;
		TSharedPtr<QuadTreeNode> pinnedObserver(parent.Pin());
		parent = pinnedObserver->GetParentNode();
	}

	return i;
}

/** Find if a given position is inside this node. */
bool QuadTreeNode::IsPositionInsideNode(FVector Position)
{
	FVector2D pos2d = { Position.X, Position.Y };
	for (auto childNode : _pChildNodes)
	{
		if (childNode->GetBoundingBox()->IsInside(pos2d))		
			return true;		
	}

	return false;
}

/** Get the node which contains a given position. */
TSharedPtr<QuadTreeNode> QuadTreeNode::GetNode(FVector Position)
{
	for (auto childNode : _pChildNodes)
	{
		if (childNode->IsPositionInsideNode(Position))		
			return childNode;		
	}

	return nullptr;
}

/** Get all elements belong to this node & elements which are contained within its children. */
TArray<AActor*> QuadTreeNode::GetAllElements()
{
	TArray<AActor*> result;

	for (auto childNode : GetChildNodes())
	{
		result.Append(childNode->GetElements());
	}

	result.Append(_pElements);
	return result;
}

/** Determines whether or not a node in this element. */
void QuadTreeNode::GenerateElementInNode(UWorld* World, const FVector2D MaximumDistanceBetweenActors)
{

}

/** Get the root node of this QuadTree. */
TSharedPtr<QuadTreeNode> QuadTreeNode::GetRootNode()
{
	TSharedPtr<QuadTreeNode> parentNode = TSharedPtr<QuadTreeNode>(GetParentNode().Pin());

	// If the parent node isn't nullptr
	if (parentNode != NULL)
	{
		// Grab an instance of the grand parent
		TSharedPtr<QuadTreeNode> parentOfParent = TSharedPtr<QuadTreeNode>(parentNode->GetParentNode().Pin());

		// If grandparent is valid
		if (parentOfParent != nullptr)
		{
			// Recursively search the generation above
			return parentNode->GetRootNode();
		}
		// Found the root node, return the immediate parent to this node
		else
			return parentNode;		
	}

	// Couldn't find a valid parent for whatever reason. Maybe only 2 layers?
	return nullptr;
}

/** Get the node which contains a given position. */
TSharedPtr<QuadTreeNode> QuadTreeNode::GetNodeContainingPosition(FVector2D Position)
{
	// Explore each child that belongs to this node
	for (auto childNode : _pChildNodes)
	{
		// If the position lies within the bounds of this child
		if (childNode->_pBoundingBox->IsInside(Position))
		{
			// If the child has children of its own
			if (childNode->HasChildrenNodes())
			{
				// Explore the children nodes to get the deepest position in the tree
				TSharedPtr<QuadTreeNode> result = childNode->GetNodeContainingPosition(Position);
				if (!result.IsValid())
				{
					// Hit a nullptr, return the current child node that we know contains the position. (possibly on the boundary?)
					return childNode;
				} else
					return result;
			}

			// If this node has no children, then we can't be any more accurate to the position
			else
				return childNode;			
		}
	}
	return nullptr;
}