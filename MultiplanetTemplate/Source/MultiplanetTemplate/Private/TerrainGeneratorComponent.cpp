// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrainGeneratorComponent.h"
#include "Containers/Array.h"

/*
	Sets default values for this component's properties
*/
UTerrainGeneratorComponent::UTerrainGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

/*
	Called when the game starts
*/
void UTerrainGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	_pNoiseComponent = Cast<UPerlinNoiseComponent>(GetOwner()->GetComponentByClass(UPerlinNoiseComponent::StaticClass()));
	_pRuntimeMeshComponent = Cast<URuntimeMeshComponentStatic>(GetOwner()->GetComponentByClass(URuntimeMeshComponentStatic::StaticClass()));

	// Create quadtree
	_QuadTree = MakeShared<QuadTree>();
	_QuadTree->InitializeTree(10, FVector2D(0, 0), FVector2D(1000, 1000));
}

/*
	Called every frame
*/
void UTerrainGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (_QuadTree != NULL)
		_QuadTree->DebugDrawBoxes(GetWorld());
}

void UTerrainGeneratorComponent::GenerateVertices()
{
	_Vertices.Init(FVector(0.0f, 0.0f, 0.0f), _iVerticesArraySize);
	for (int y = 0; y < _iNoiseSamplesPerLine; y++)
	{
		for (int x = 0; x < _iNoiseSamplesPerLine; x++)
		{
			float noiseResult = GetNoiseValueForGridCoordinates(x, y);
			int index = GetIndexForGridCoordinates(x, y);
			FVector2D position = GetPositionForGridCoordinates(x, y);
			_Vertices[index] = FVector(position.X, position.Y, noiseResult);
			_UV[index] = FVector2D(x, y);
		}
	}
}

void UTerrainGeneratorComponent::GenerateTriangles()
{ 
	// The number of triangle indexes making up a quad
	int quadSize = 6;
	// We have one less quad per line than the amount of vertices, since each vertex is the start of a quad
	int numberOfQuadsPerLine = _iNoiseSamplesPerLine - 1;

	int trianglesArraySize = numberOfQuadsPerLine * numberOfQuadsPerLine * quadSize;
	_Triangles.Init(0, trianglesArraySize);

	for (int y = 0; y < numberOfQuadsPerLine; y++)
	{
		for (int x = 0; x < numberOfQuadsPerLine; x++)
		{
			int quadIndex = x + y * numberOfQuadsPerLine;
			int triangleIndex = quadIndex * quadSize;
		
			// Getting the indexes of the four vertices making up this quad
			int bottomLeftIndex = GetIndexForGridCoordinates(x, y);
			int topLeftIndex = GetIndexForGridCoordinates(x, y + 1);
			int topRightIndex = GetIndexForGridCoordinates(x + 1, y + 1);
			int bottomRightIndex = GetIndexForGridCoordinates(x + 1, y);

			// Assigning the 6 triangle points to the corresponding vertex indexes, by going counter-clockwise
			_Triangles[triangleIndex] = bottomLeftIndex;
			_Triangles[triangleIndex + 1] = topLeftIndex;
			_Triangles[triangleIndex + 2] = topRightIndex;
			_Triangles[triangleIndex + 3] = bottomLeftIndex;
			_Triangles[triangleIndex + 4] = topRightIndex;
			_Triangles[triangleIndex + 5] = bottomRightIndex;
		}
	}
}

void UTerrainGeneratorComponent::GenerateMesh()
{
	_pRuntimeMeshComponent->CreateSectionFromComponents(0, 0, 0, _Vertices, _Triangles, _Normals, _UV, _VertexColours, _Tangents, ERuntimeMeshUpdateFrequency::Average, true);
}

float UTerrainGeneratorComponent::GetNoiseValueForGridCoordinates(int X, int Y)
{
	return _pNoiseComponent->GetValue((X * _fNoiseInputScale) + 0.1, (Y * _fNoiseInputScale) + 0.1, 1.0) * _fNoiseOutputScale;
}

int UTerrainGeneratorComponent::GetIndexForGridCoordinates(int X, int Y)
{
	return X + Y * _iNoiseSamplesPerLine;
}

FVector2D UTerrainGeneratorComponent::GetPositionForGridCoordinates(int X, int Y)
{
	return FVector2D(X * _iNoiseResolution, Y * _iNoiseResolution);
}

void UTerrainGeneratorComponent::GenerateTerrain()
{
	if (_pNoiseComponent == NULL)
		_pNoiseComponent = Cast<UPerlinNoiseComponent>(GetOwner()->GetComponentByClass(UPerlinNoiseComponent::StaticClass()));

	if (_pRuntimeMeshComponent == NULL)
		_pRuntimeMeshComponent = Cast<URuntimeMeshComponentStatic>(GetOwner()->GetComponentByClass(URuntimeMeshComponentStatic::StaticClass()));

	if (_pNoiseComponent != NULL && _pRuntimeMeshComponent != NULL)
	{
		_Normals.Init(FVector(0, 0, 1), _iVerticesArraySize);
		_Tangents.Init(FRuntimeMeshTangent(0, -1, 0), _iVerticesArraySize);
		_UV.Init(FVector2D(0, 0), _iVerticesArraySize);
		_VertexColours.Init(FColor::White, _iVerticesArraySize);

		GenerateVertices();
		GenerateTriangles();
		GenerateMesh();
	} else
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("ERROR: _pNoiseComponent AND/OR _pRuntimeMeshComponent are NULL"));
}
