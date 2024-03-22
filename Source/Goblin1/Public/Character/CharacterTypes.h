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
	ECS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_EquippingWeapon UMETA(DisplayName = "Equipping weapon"),
	ECS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum EDeathPose
{
	EDS_Death1 UMETA(DisplayName = "Death 1"),
	EDS_Death2 UMETA(DisplayName = "Death 2"),
	EDS_Death3 UMETA(DisplayName = "Death 3"),

	EDS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};
