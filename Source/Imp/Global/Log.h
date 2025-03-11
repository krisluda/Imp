#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogImp, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogImpError, Log, All);

#define IMP_LOG(Format, ...) UE_LOG(LogImp, Log, TEXT(Format), ##__VA_ARGS__)
#define IMP_LOG_ERROR(Format, ...) UE_LOG(LogImpError, Error, TEXT(Format), ##__VA_ARGS__)