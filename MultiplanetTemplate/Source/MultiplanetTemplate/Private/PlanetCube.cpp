// Fill out your copyright notice in the Description page of Project Settings.

#include "PlanetCube.h"
#include "Providers/RuntimeMeshProviderBox.h"
#include "Providers/RuntimeMeshProviderStatic.h"

/*
	Sets default values for this component's properties
*/
APlanetCube::APlanetCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create provider
	_StaticProvider = CreateDefaultSubobject<URuntimeMeshProviderStatic>(TEXT("RuntimeMeshProvider-Static"));
	///_BoxProvider->seri
}

void APlanetCube::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	GenerateMesh();
}

void APlanetCube::PostLoad()
{
	Super::PostLoad();
	GenerateMesh();
}

/*
	Called when the game starts
*/
void APlanetCube::BeginPlay()
{
	Super::BeginPlay();
	
}

/*
	Called every frame
*/
void APlanetCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlanetCube::GenerateMesh()
{
	GetRuntimeMeshComponent()->Initialize(_StaticProvider);
	_StaticProvider->ClearSection(0, 0);
	SetupMeshBuffers();
	GenerateCube(_Positions, _Triangles, _Normals, _Tangents, _TexCoords, _Size);

	const TArray<FColor> emptyColours {};
	_StaticProvider->CreateSectionFromComponents(0, 0, 0, _Positions, _Triangles, _Normals, _TexCoords, emptyColours, _Tangents, ERuntimeMeshUpdateFrequency::Average, false);
	_StaticProvider->SetupMaterialSlot(0, TEXT("CubeMaterial"), Material);
}

void APlanetCube::GenerateCube(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D> InTexCoords, FVector InSize)
{
	// NOTE: Unreal uses an upper-left origin UV
	// NOTE: This sample uses a simple UV mapping scheme where each face is the same
	// NOTE: For a normal facing towards us, be build the polygon CCW in the order 0-1-2 then 0-2-3 to complete the quad.
	// Remember in Unreal, X is forwards, Y is to your right and Z is up.

	// Calculate a half offset so we have the correct center of the object
	const float offsetX = InSize.X / 2.0f;
	const float offsetY = InSize.Y / 2.0f;
	const float offsetZ = InSize.Z / 2.0f;	

	// Define the 8 corners of the cube
	const FVector p0 = FVector(offsetX,  offsetY, -offsetZ);
	const FVector p1 = FVector(offsetX, -offsetY, -offsetZ);
	const FVector p2 = FVector(offsetX, -offsetY,  offsetZ);
	const FVector p3 = FVector(offsetX,  offsetY,  offsetZ);
	const FVector p4 = FVector(-offsetX, offsetY, -offsetZ);
	const FVector p5 = FVector(-offsetX, -offsetY, -offsetZ);
	const FVector p6 = FVector(-offsetX, -offsetY, offsetZ);
	const FVector p7 = FVector(-offsetX, offsetY,  offsetZ);

	// Create 6x faces, 4 verts each
	int32 vertexOffset = 0;
	int32 triangleOffset = 0;
	FVector normal;
	FRuntimeMeshTangent tangent;

	// Front (+X) face: 0-1-2-3
	normal = FVector(1, 0, 0);
	tangent = FVector(0, 1, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p0, p1, p2, p3, vertexOffset, triangleOffset, normal, tangent);

	// Back (-X) face: 5-4-7-6
	normal = FVector(-1, 0, 0);
	tangent = FVector(0, -1, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p5, p4, p7, p6, vertexOffset, triangleOffset, normal, tangent);

	// Left (-Y) face: 1-5-6-2
	normal = FVector(0, -1, 0);
	tangent = FVector(1, 0, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p1, p5, p6, p2, vertexOffset, triangleOffset, normal, tangent);

	// Right (+Y) face: 4-0-3-7
	normal = FVector(0, 1, 0);
	tangent = FVector(-1, 0, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p4, p0, p3, p7, vertexOffset, triangleOffset, normal, tangent);

	// Top (+Z) face: 6-7-3-2
	normal = FVector(0, 0, 1);
	tangent = FVector(0, 1, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p6, p7, p3, p2, vertexOffset, triangleOffset, normal, tangent);

	// Bottom (-Z) face: 1-0-4-5
	normal = FVector(0, 0, -1);
	tangent = FVector(0, -1, 0);
	GenerateQuad(InVertices, InTriangles, InNormals, InTangents, InTexCoords, p1, p0, p4, p5, vertexOffset, triangleOffset, normal, tangent);
}

void APlanetCube::GenerateQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D> InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FRuntimeMeshTangent Tangent)
{
	const int32 index1 = VertexOffset++;
	const int32 index2 = VertexOffset++;
	const int32 index3 = VertexOffset++;
	const int32 index4 = VertexOffset++;
	InVertices[index1] = BottomLeft;
	InVertices[index2] = BottomRight;
	InVertices[index3] = TopRight;
	InVertices[index4] = TopLeft;
	InTexCoords[index1] = FVector2D(0.0f, 1.0f);
	InTexCoords[index2] = FVector2D(1.0f, 1.0f);
	InTexCoords[index3] = FVector2D(1.0f, 0.0f);
	InTexCoords[index4] = FVector2D(0.0f, 0.0f);
	InTriangles[TriangleOffset++] = index1;
	InTriangles[TriangleOffset++] = index2;
	InTriangles[TriangleOffset++] = index3;
	InTriangles[TriangleOffset++] = index1;
	InTriangles[TriangleOffset++] = index3;
	InTriangles[TriangleOffset++] = index4;
	
	// On a cube side, all the vertex normals face the same way
	InNormals[index1] = InNormals[index2] = InNormals[index3] = InNormals[index4] = Normal;
	InTangents[index1] = InTangents[index2] = InTangents[index3] = InTangents[index4] = Tangent;
}

void APlanetCube::SetupMeshBuffers()
{
	const int32 vertexCount = 6 * 4; // 6 sides on a cube, 4 verts each
	const int32 triangleCount = 6 * 2 * 3; // 2x triangles per cube side, 3 verts each

	if (vertexCount != _Positions.Num())
	{
		_Positions.Empty();
		_Positions.AddUninitialized(vertexCount);
		_Normals.Empty();
		_Normals.AddUninitialized(vertexCount);
		_Tangents.Empty();
		_Tangents.AddUninitialized(vertexCount);
		_TexCoords.Empty();
		_TexCoords.AddUninitialized(vertexCount);
	}

	if (triangleCount != _Triangles.Num())
	{
		_Triangles.Empty();
		_Triangles.AddUninitialized(triangleCount);
	}
}