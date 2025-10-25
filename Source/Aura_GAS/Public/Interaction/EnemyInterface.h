

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};


class AURA_GAS_API IEnemyInterface
{
	GENERATED_BODY()


public:
	//接口为抽象类，谁继承了该接口谁就实现该接口
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;
};
