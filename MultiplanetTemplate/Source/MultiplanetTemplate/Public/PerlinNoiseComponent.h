// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <libnoise.h>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PerlinNoiseComponent.generated.h"

UENUM(BlueprintType)
enum class E_PerlinNoiseQuality : uint8
{
	QUALITY_FAST UMETA(DisplayName = "Fast Quality"),
	QUALITY_STD UMETA(DisplayName = "Standard Quality"),
	QUALITY_BEST UMETA(DisplayName = "Best Quality")
};

UCLASS( ClassGroup=(Custom), BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class MULTIPLANETTEMPLATE_API UPerlinNoiseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/*
		Sets default values for this component's properties
	*/
	UPerlinNoiseComponent();

protected:

	/*
		Called when the game starts
	*/
	virtual void BeginPlay() override;

	/*
		Called every frame
	*/
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	noise::module::Perlin _PerlinNoise;
	noise::NoiseQuality convertBPNoiseQuality(E_PerlinNoiseQuality Quality) const;
	E_PerlinNoiseQuality convertBPNoiseQuality(noise::NoiseQuality Quality) const;
	void UpdateOptions();

	// Private because they shouldn't be modified at runtime, but can be edited in the editor before running the game
	UPROPERTY(EditAnywhere, Category = "Perlin")
		float _fFrequency = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Perlin")
		float _fLacunarity = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Perlin")
		TEnumAsByte<E_PerlinNoiseQuality> _eNoiseQuality = E_PerlinNoiseQuality::QUALITY_STD;

	UPROPERTY(EditAnywhere, Category = "Perlin")
		int _iOctaveCount = 6;

	UPROPERTY(EditAnywhere, Category = "Perlin")
		float _fPersistence = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Perlin")
		int _iSeed = 0;

public:	

	/*
		Primary method to call for noise output
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "noise perlin"), Category = "Perlin")
		float GetValue(float X, float Y, float Z) const { return _PerlinNoise.GetValue(X, Y, Z); }

	/*
		Setup method for all categories
	*/
	UFUNCTION(BlueprintCallable, Category = "Perlin")
		void SetupOptions(float Frequency, float Lacunarity, E_PerlinNoiseQuality NoiseQuality, int OctaveCount, float Persistence, int Seed);

	// Setters
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetFrequency(float Frequency);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetLacunarity(float Lacunarity);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetNoiseQuality(E_PerlinNoiseQuality NoiseQuality);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetOctaveCount(int OctaveCount);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetPersistence(float Persistence);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetSeed(int Seed);
	UFUNCTION(BlueprintCallable, Category = "Perlin") void SetRandomSeed();

	// Getters
	UFUNCTION(BlueprintPure, Category = "Perlin") float GetFrequency() const { return _PerlinNoise.GetFrequency(); }
	UFUNCTION(BlueprintPure, Category = "Perlin") float GetLacunarity() const { return _PerlinNoise.GetLacunarity(); }
	UFUNCTION(BlueprintPure, Category = "Perlin") E_PerlinNoiseQuality GetNoiseQuality() const { return convertBPNoiseQuality(_PerlinNoise.GetNoiseQuality()); }
	UFUNCTION(BlueprintPure, Category = "Perlin") int GetOctaveCount() const { return _PerlinNoise.GetOctaveCount(); }
	UFUNCTION(BlueprintPure, Category = "Perlin") float GetPersistence() const { return _PerlinNoise.GetPersistence(); }
	UFUNCTION(BlueprintPure, Category = "Perlin") int GetSeed() const { return _PerlinNoise.GetSeed(); }
	UFUNCTION(BlueprintPure, Category = "Perlin") float GetSourceModuleCount() const { return _PerlinNoise.GetSourceModuleCount(); }

};
