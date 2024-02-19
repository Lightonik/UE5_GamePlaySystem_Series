#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8 
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapond"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two-Handed Weapond"),
};

UENUM(BlueprintType)
enum class EActionsState : uint8 
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_EquippingWeapon UMETA(DisplayName = "Equipping weapon")
};

UENUM(BlueprintType)
enum class EDeathPose : uint8
{
	EDS_Alive UMETA(DisplayName = "Alive"),
	EDS_Death1 UMETA(DisplayName = "Death 1"),
	EDS_Death2 UMETA(DisplayName = "Death 2"),
	EDS_Death3 UMETA(DisplayName = "Death 3")
};