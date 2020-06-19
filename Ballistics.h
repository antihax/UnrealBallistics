
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/World.h"
#include "Ballistics.generated.h"

struct FBallisticInFlight
{
	FVector Position;
	FVector Vector;
	FVector Velocity;
	float BallisticCoefficient;
	float Lifetime;
	bool InPenetration;

	FVector CheckForExit(UWorld* World, FVector ImpactPoint, FVector Direction, TWeakObjectPtr<AActor> Ignore);
	FCollisionQueryParams TraceParameters();
};

UCLASS()
class SHOOTERGAME_API UBallisticsSubSystem : public UWorldSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;

	// Clear any ballistics still in play.
	void Clear();

	// CalculateDeceleration to G function tables with a ballistic coefficient and velocity in cm/s
	float CalculateDeceleration(int DragFunction, float BallisticCoefficient, float Velocity);

	// AddProjectile to be tracked
	void AddProjectile(APawn* character, FVector Start, FVector Direction);
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	bool ready;
	TMap<APawn*, TArray<FBallisticInFlight>> Projectiles;
};


