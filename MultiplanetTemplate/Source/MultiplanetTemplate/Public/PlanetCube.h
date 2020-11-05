// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshActor.h"

#include "PlanetCube.generated.h"

class URuntimeMeshProviderStatic;
struct FRuntimeMeshTangent;

UCLASS()
class MULTIPLANETTEMPLATE_API APlanetCube : public ARuntimeMeshActor
{
	GENERATED_BODY()
	
public:

	/*
		Sets default values for this component's properties
	*/
	APlanetCube();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Properties")
		FVector _Size = FVector(100.0f, 100.0f, 100.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Properties")
		UMaterialInterface* Material;

	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable)
		void CreateMesh() { GenerateMesh(); }

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient)
		URuntimeMeshProviderStatic* _StaticProvider;

protected:

	/*
		Called when the game starts
	*/
	virtual void BeginPlay() override;

	/*
		Called every frame
	*/
	virtual void Tick(float DeltaTime) override;

private:

	void GenerateMesh();
	static void GenerateCube(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D> InTexCoords, FVector InSize);
	static void GenerateQuad(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector>& InNormals, TArray<FRuntimeMeshTangent>& InTangents, TArray<FVector2D> InTexCoords, const FVector BottomLeft, const FVector BottomRight, const FVector TopRight, const FVector TopLeft, int32& VertexOffset, int32& TriangleOffset, const FVector Normal, const FRuntimeMeshTangent Tangent);

	// Mesh buffers
	void SetupMeshBuffers();
	UPROPERTY(Transient) TArray<FVector> _Positions;
	UPROPERTY(Transient) TArray<int32> _Triangles;
	UPROPERTY(Transient) TArray<FVector> _Normals;
	UPROPERTY(Transient) TArray<FRuntimeMeshTangent> _Tangents;
	UPROPERTY(Transient) TArray<FVector2D> _TexCoords;

};