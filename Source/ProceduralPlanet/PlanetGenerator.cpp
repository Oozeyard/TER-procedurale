// Fill out your copyright notice in the Description page of Project Settings.
// Copyright 2021 Phyronnaz

#include "PlanetGenerator.h"
#include "FastNoise/VoxelFastNoise.inl"
#include "VoxelMaterialBuilder.h"

TVoxelSharedRef<FVoxelGeneratorInstance> UVoxelGeneratorExample::GetInstance()
{
	return MakeVoxelShared<FVoxelGeneratorExampleInstance>(*this);
}

///////////////////////////////////////////////////////////////////////////////

FVoxelGeneratorExampleInstance::FVoxelGeneratorExampleInstance(const UVoxelGeneratorExample& MyGenerator)
	: Super(&MyGenerator)
	, NoiseHeight(MyGenerator.NoiseHeight)
	, Seed(MyGenerator.Seed)
{
}

void FVoxelGeneratorExampleInstance::Init(const FVoxelGeneratorInit& InitStruct)
{
	Noise.SetSeed(Seed);
}

v_flt FVoxelGeneratorExampleInstance::GetValueImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	// Radius of the world
	const float Radius = 100.f;

	// Normalize the position to get the 3D noise sample position
	const FVector SamplePosition = FVector(X, Y, Z).GetSafeNormal();

	// Compute noise. Note that the noise has a much higher frequency since the sample position is normalized
	const float Height = Radius + Noise.GetPerlin_3D(SamplePosition.X, SamplePosition.Y, SamplePosition.Z, 1.f) * NoiseHeight;

	// Value = DistanceFromCenter - Height
	float Value = FVector(X, Y, Z).Size() - Height;

	// Smoother gradient
	Value /= 5;

	return Value;
}

FVoxelMaterial FVoxelGeneratorExampleInstance::GetMaterialImpl(v_flt X, v_flt Y, v_flt Z, int32 LOD, const FVoxelItemStack& Items) const
{
	FVoxelMaterialBuilder Builder;

	// RGB
	Builder.SetMaterialConfig(EVoxelMaterialConfig::RGB);
	Builder.SetColor(FColor::Red);

	// Single index
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::SingleIndex);
	//Builder.SetSingleIndex(0); 

	// Multi index
	//Builder.SetMaterialConfig(EVoxelMaterialConfig::MultiIndex);
	//Builder.AddMultiIndex(0, 0.5f);
	//Builder.AddMultiIndex(1, 0.5f);

	return Builder.Build();
}

TVoxelRange<v_flt> FVoxelGeneratorExampleInstance::GetValueRangeImpl(const FVoxelIntBox& Bounds, int32 LOD, const FVoxelItemStack& Items) const
{
	// Return the values that GetValueImpl can return in Bounds
	// Used to skip chunks where the value does not change
	// Be careful, if wrong your world will have holes!
	// By default return infinite range to be safe
	return TVoxelRange<v_flt>::Infinite();

	// Example for the GetValueImpl above

	// Noise is between -1 and 1
	const TVoxelRange<v_flt> Height = TVoxelRange<v_flt>(-1, 1) * NoiseHeight;

	// Z can go from min to max
	TVoxelRange<v_flt> Value = TVoxelRange<v_flt>(Bounds.Min.Z, Bounds.Max.Z) - Height;

	Value /= 5;

	return Value;
}

FVector FVoxelGeneratorExampleInstance::GetUpVector(v_flt X, v_flt Y, v_flt Z) const
{
	// Used by spawners
	return FVector::UpVector;
}