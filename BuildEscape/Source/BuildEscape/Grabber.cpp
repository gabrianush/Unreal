// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	FindPhysicsHandlerComponent();
	SetupInputComponent();
}

// Look for attached Physics Handle 
void UGrabber::FindPhysicsHandlerComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Missing physics handle component for: %s! "), *GetOwner()->GetName())
	}
}

//Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr) {
		//Bind input axis
		//InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		//InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Missing input component for: %s! "), *GetOwner()->GetName())
	}
}

void UGrabber::Grab() {
	//LINE TRACE and see if we hit any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();//get the mesh
	auto ActorHit = HitResult.GetActor();
	
	//if we hit something the attach a physics handler
	if (ActorHit != nullptr) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None, //no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation()
			//true //to allow rotation
		);
	}
}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	//if the physics handler is attached
	//if (PhysicsHandle->GrabbedComponent) {
	//	PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	//}
	//GetFirstPhysicsBodyInReach();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{	
	//Log out to test
	//UE_LOG(LogTemp, Warning, TEXT("Location %s, Rotation %s"), 
	//	*PlayerViewPointLocation.ToString(), 
	//	*PlayerViewPointRotator.ToString());


	//Draw a ray trace in the world to visualize
	//DrawDebugLine(
	//	GetWorld(),
	//	GetReachLineStart(),
	//	GetReachLineEnd(),
	//	FColor(255, 0, 0),
	//	false,
	//	0.f,
	//	0.f,
	//	10.f
	//);

	///Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Line trace(AKA ray-cast) out to reach distance
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	//See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
	}

	return HitResult;
}

FVector UGrabber::GetReachLineStart() {
	//Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() {
	//Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);
	//Calculate ray trace end
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;//+ FVector(0.f, 0.f, 100.f);
	return LineTraceEnd;
}