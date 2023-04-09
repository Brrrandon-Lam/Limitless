// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Character/PlayerCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize our sphere and static mesh components.

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Adds callback function to delegate.
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called in response to overlap with the sphere component with the following parameters that give us information about our overlap event.

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// Debugging
	const FString OverlappedActor = FString("Item began overlap with actor: ") + OtherActor->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with actor: %s"), *OverlappedActor);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter) 
	{
		PlayerCharacter->SetOverlappingItem(this);
	}


}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		// Unset the overlapping item.
		PlayerCharacter->SetOverlappingItem(nullptr);
	}
	
}