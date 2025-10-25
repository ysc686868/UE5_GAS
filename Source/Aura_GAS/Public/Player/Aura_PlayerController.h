
#pragma once
#include "GameFramework/PlayerController.h"

#include "Aura_PlayerController.generated.h"


//前向声明
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;


UCLASS()
class AURA_GAS_API AAura_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAura_PlayerController();
	virtual void PlayerTick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	//上下文
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;//默认置空
	//移动行为
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	//移动行为的回调函数
	void Move(const FInputActionValue& InputActionValue);

	//光标追踪
	void CursorTrace();
	//判断光标悬停时的前后两帧是否是同一个actor
	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
};
