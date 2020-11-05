#include "QuadTree.h"

void QuadTree::InitializeTree(const int MaxDistance, FVector2D Minimum, FVector2D Maximum)
{
	_iMaxDistance = MaxDistance;
	_pRootNode = CreateNode(nullptr, Minimum, Maximum);
	_pRootNode->SetNodePosition(ENodePosition::Root);
	InitializeNodes(_pRootNode, Minimum, Maximum);
}

// Add an element to the QuadTree
void QuadTree::AddElement(AActor* Actor)
{
	_pRootNode->AddElement(Actor);
}

void QuadTree::DebugDrawBoxes(UWorld* World)
{
	_pRootNode->DrawBoxAroundNode(World, FColor::Red);
}

TSharedPtr<QuadTreeNode> QuadTree::GetNode(FVector Positon)
{
	if (_pRootNode->IsPositionInsideNode(Positon))
	{
		return _pRootNode->GetNode(Positon);
	}
	return nullptr;
}

void QuadTree::InitializeNodes(TSharedPtr<QuadTreeNode> parentNode, FVector2D Minimum, FVector2D Maximum)
{
	// Bottom right node
	FVector2D brMin = Minimum;
	FVector2D brMax = { Maximum.X - ((Maximum.X - Minimum.X) / 2.0f), Maximum.Y - ((Maximum.Y - Minimum.Y) / 2.0f) };
	TSharedPtr<QuadTreeNode> br = CreateNode(parentNode, brMin, brMax);
	br->SetNodePosition(ENodePosition::BottomRight);

	// Bottom left node
	FVector2D blMin = { Maximum.X - ((Maximum.X - Minimum.X) / 2.0f), Minimum.Y };
	FVector2D blMax = { Maximum.X, Minimum.Y + ((Maximum.Y - Minimum.Y) / 2.0f) };
	TSharedPtr<QuadTreeNode> bl = CreateNode(parentNode, blMin, blMax);
	bl->SetNodePosition(ENodePosition::BottomLeft);

	// Top right node
	FVector2D trMin = { Minimum.X, Maximum.Y - ((Maximum.Y - Minimum.Y) / 2.0f) };
	FVector2D trMax = { Maximum.X, Minimum.Y + ((Maximum.Y - Minimum.Y) / 2.0f) };
	TSharedPtr<QuadTreeNode> tr = CreateNode(parentNode, trMin, trMax);
	tr->SetNodePosition(ENodePosition::TopRight);

	// Top left node
	FVector2D tlMin = { Maximum.X - ((Maximum.X - Minimum.X) / 2.0f), Maximum.Y - ((Maximum.Y - Minimum.Y) / 2.0f) };
	FVector2D tlMax = { Maximum.X, Maximum.Y };
	TSharedPtr<QuadTreeNode> tl = CreateNode(parentNode, tlMin, tlMax);
	tl->SetNodePosition(ENodePosition::TopLeft);

	if (bl->GetDistance() < _iMaxDistance &&
		br->GetDistance() < _iMaxDistance &&
		tl->GetDistance() < _iMaxDistance &&
		tr->GetDistance() < _iMaxDistance)
	{
		InitializeNodes(br, brMin, brMax);
		InitializeNodes(bl, blMin, blMax);
		InitializeNodes(tr, trMin, trMax);
		InitializeNodes(tl, tlMin, tlMax);
	}
}

TSharedPtr<QuadTreeNode> QuadTree::CreateNode(TSharedPtr<QuadTreeNode> parentNode, FVector2D Minimum, FVector2D Maximum)
{
	// Bottom left node
	TSharedPtr<QuadTreeNode> newNode = MakeShared<QuadTreeNode>();
	if (parentNode != nullptr)
	{
		parentNode->AddChildNode(newNode);
	}
	newNode->SetParentNode(TWeakPtr<QuadTreeNode>(parentNode));
	newNode->SetBoundingBox(Minimum, Maximum);
	return newNode;
}
