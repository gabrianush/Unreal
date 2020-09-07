// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h" 
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FHitResult GetFirstPhysicsBodyInReach();
private:
	UPROPERTY(EditAnywhere)
	float Reach = 200.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//Ray-cast and grab what's in reach
	void Grab();
	//Called when Grab is released
	void Release();
	//Find attached physics component
	void FindPhysicsHandlerComponent();
	//Setup (assumed) attached input component
	void SetupInputComponent();
	//Return current start of reach line
	FVector GetReachLineStart();
	//Return current end of reach line
	FVector GetReachLineEnd();

};
