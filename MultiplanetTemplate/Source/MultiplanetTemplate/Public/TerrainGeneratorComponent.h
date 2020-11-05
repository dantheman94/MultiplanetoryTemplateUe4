// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "PerlinNoiseComponent.h"
#include "QuadTree.h"
#include "RuntimeMeshComponentStatic.h"

#include "TerrainGeneratorComponent.generated.h"


UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class MULTIPLANETTEMPLATE_API UTerrainGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	/*
		Sets default values for this component's properties
	*/
	UTerrainGeneratorComponent();

private:

	UPerlinNoiseComponent* _pNoiseComponent = NULL;
	URuntimeMeshComponentStatic* _pRuntimeMeshComponent = NULL;
	TArray<FVector> _Vertices;
	TArray<int> _Triangles;
	TArray<FVector> _Normals;
	TArray<FRuntimeMeshTangent> _Tangents;
	TArray<FVector2D> _UV;
	TArray<FColor> _VertexColours;

	UPROPERTY(VisibleAnywhere, Category = "Noise", meta = (DisplayName = "Generation Size"))
		int _iTotalSizeToGenerate = 12000;

	UPROPERTY(VisibleAnywhere, Category = "Noise", meta = (DisplayName = "Noise Resolution"))
		int _iNoiseResolution = 300;

	UPROPERTY()
		int _iNoiseSamplesPerLine = _iTotalSizeToGenerate / _iNoiseResolution;

	UPROPERTY()
		int _iVerticesArraySize = _iNoiseSamplesPerLine * _iNoiseSamplesPerLine;

	/*
		Smaller values will "stretch" the perlin noise terrain
	*/
	UPROPERTY(EditAnywhere, Category = "Noise", meta = (DisplayName = "Noise Input Scale"))
		float _fNoiseInputScale = 0.01f;

	/*
		Larger values will scale the terrain's height.
	*/
	UPROPERTY(EditAnywhere, Category = "Noise", meta = (DisplayName = "Noise Output Scale"))
		float _fNoiseOutputScale = 2000.0f;

	TSharedPtr<QuadTree> _QuadTree;

public:

	UFUNCTION(BlueprintCallable)
		void GenerateTerrain();

	// Setters
	UFUNCTION(BlueprintSetter) void SetGenerationSize(int GenerationSize) { _iTotalSizeToGenerate = GenerationSize; }
	UFUNCTION(BlueprintSetter) void SetNoiseResolution(int Resolution) { _iNoiseResolution = Resolution; }
	UFUNCTION(BlueprintSetter) void SetNoiseInputScale(int Scale) { _fNoiseInputScale = Scale; }
	UFUNCTION(BlueprintSetter) void SetNoiseOutputScale(int Scale) { _fNoiseOutputScale = Scale; }

	// Getters
	UFUNCTION(BlueprintPure) int GetGenerationSize() { return _iTotalSizeToGenerate; }
	UFUNCTION(BlueprintPure) int GetNoiseResolution() { return _iNoiseResolution; }
	UFUNCTION(BlueprintPure) float GetNoiseInputScale() { return _fNoiseInputScale; }
	UFUNCTION(BlueprintPure) float GetNoiseOutputScale() { return _fNoiseOutputScale; }

protected:

	/*
		Called when the game starts
	*/
	virtual void BeginPlay() override;

	/*
		Called every frame
	*/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GenerateVertices();
	void GenerateTriangles();
	void GenerateMesh();

	float GetNoiseValueForGridCoordinates(int X, int Y);
	int GetIndexForGridCoordinates(int X, int Y);
	FVector2D GetPositionForGridCoordinates(int X, int Y);

};
