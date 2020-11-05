// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoiseComponent.h"
#include "Math.h"

/*
	Sets default values for this component's properties
*/
UPerlinNoiseComponent::UPerlinNoiseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UpdateOptions();
}

/*
	Called when the game starts
*/
void UPerlinNoiseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...


}

/*
	Called every frame
*/
void UPerlinNoiseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

E_PerlinNoiseQuality UPerlinNoiseComponent::convertBPNoiseQuality(noise::NoiseQuality Quality) const
{
	switch (Quality)
	{
	case noise::QUALITY_FAST:
		return E_PerlinNoiseQuality::QUALITY_FAST;
		break;

	case noise::QUALITY_BEST:
		return E_PerlinNoiseQuality::QUALITY_BEST;
		break;

	case noise::QUALITY_STD:
		return E_PerlinNoiseQuality::QUALITY_STD;
		break;

	default: 
		return E_PerlinNoiseQuality::QUALITY_STD;
		break;
	}
}

noise::NoiseQuality UPerlinNoiseComponent::convertBPNoiseQuality(E_PerlinNoiseQuality Quality) const
{
	switch (Quality)
	{
	case E_PerlinNoiseQuality::QUALITY_BEST:
		return noise::QUALITY_BEST;
		break;
		
	case E_PerlinNoiseQuality::QUALITY_FAST:
		return noise::QUALITY_FAST;
		break;

	case E_PerlinNoiseQuality::QUALITY_STD:
		return noise::QUALITY_STD;
		break;

	default:
		return noise::QUALITY_STD;
		break;
	}
}

void UPerlinNoiseComponent::UpdateOptions()
{
	SetupOptions(_fFrequency, _fLacunarity, _eNoiseQuality, _iOctaveCount, _fPersistence, _iSeed);
}

void UPerlinNoiseComponent::SetupOptions(float Frequency, float Lacunarity, E_PerlinNoiseQuality NoiseQuality, int OctaveCount, float Persistence, int Seed)
{
	SetFrequency(Frequency);
	SetLacunarity(Lacunarity);
	SetNoiseQuality(NoiseQuality);
	SetOctaveCount(OctaveCount);
	SetPersistence(Persistence);
	SetSeed(Seed);
}

void UPerlinNoiseComponent::SetFrequency(float Frequency)
{
	_PerlinNoise.SetFrequency(Frequency);
}

void UPerlinNoiseComponent::SetLacunarity(float Lacunarity)
{
	_PerlinNoise.SetLacunarity(Lacunarity);
}

void UPerlinNoiseComponent::SetNoiseQuality(E_PerlinNoiseQuality NoiseQuality)
{
	_PerlinNoise.SetNoiseQuality(convertBPNoiseQuality(NoiseQuality));
}

void UPerlinNoiseComponent::SetOctaveCount(int OctaveCount)
{
	_PerlinNoise.SetOctaveCount(OctaveCount);
}

void UPerlinNoiseComponent::SetPersistence(float Persistence)
{
	_PerlinNoise.SetPersistence(Persistence);
}

void UPerlinNoiseComponent::SetSeed(int Seed)
{
	_PerlinNoise.SetSeed(Seed);
}

void UPerlinNoiseComponent::SetRandomSeed()
{
	SetSeed(FMath::RandRange(0, 1000000));
}

