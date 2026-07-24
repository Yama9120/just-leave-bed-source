// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

UENUM(BlueprintType)
enum class ERotationMode : uint8
{
	Orient UMETA(DisplayName = "Orient"),
	Strafe UMETA(DisplayName = "Strafe"),
};

UCLASS()
class JUST_LEAVE_BED_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	void ToggleCrouch();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void SetRotationMode(ERotationMode NewRotationMode);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DodgeAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	TObjectPtr<USkeletalMeshComponent> VisualMesh;

	// ----------------------------------------------------------------
	// 回転モードの設定（Orient: 移動方向に向く、Strafe: 視点方向に向く）
	// ----------------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Movement")
	ERotationMode RotationMode = ERotationMode::Orient;

	ERotationMode PreviousRotationMode = ERotationMode::Orient;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (ClampMin = "0.0"))
	float RotationInterpSpeed = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (ClampMin = "0.1"))
	float DodgeDistanceScale = 2.f;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dodge();

	bool CanDodge() const;
	bool IsOccupied() const;
	void OnDodgeMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void FinishDodge();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DodgeMontage;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
};
