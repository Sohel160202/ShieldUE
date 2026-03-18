#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SecureValueTypes.h"
#include "ShieldUEBlueprintLibrary.generated.h"

UCLASS()
class SHIELDUE_API UShieldUEBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/* FLOAT */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Initialize a Secure Float with a starting value and protection rules."))
	static void SecureFloat_Initialize(UPARAM(ref) FSecureFloat& Value, float InitialValue, const FSecureFloatRules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Set the current value of a Secure Float."))
	static void SecureFloat_Set(UPARAM(ref) FSecureFloat& Value, float NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Float", meta = (ToolTip = "Get the decoded runtime value from a Secure Float."))
	static float SecureFloat_Get(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Validate a Secure Float and return whether its protected state is still valid."))
	static bool SecureFloat_Validate(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Float."))
	static void SecureFloat_Rekey(UPARAM(ref) FSecureFloat& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Float", meta = (ToolTip = "Initialize a Secure Float using the rules already stored inside the variable."))
	static void SecureFloat_InitializeFromInternalRules(UPARAM(ref) FSecureFloat& Value, float InitialValue);


	/* INT */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Initialize a Secure Int with a starting value and protection rules."))
	static void SecureInt_Initialize(UPARAM(ref) FSecureInt32& Value, int32 InitialValue, const FSecureInt32Rules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Set the current value of a Secure Integer."))
	static void SecureInt_Set(UPARAM(ref) FSecureInt32& Value, int32 NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Int", meta = (ToolTip = "Get the decoded runtime value from a Secure Integer."))
	static int32 SecureInt_Get(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Validate a Secure Integer and return whether its protected state is still valid."))
	static bool SecureInt_Validate(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Integer."))
	static void SecureInt_Rekey(UPARAM(ref) FSecureInt32& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Int", meta = (ToolTip = "Initialize a Secure Int using the rules already stored inside the variable."))
	static void SecureInt_InitializeFromInternalRules(UPARAM(ref) FSecureInt32& Value, int32 InitialValue);


	/* BOOL */

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Initialize a Secure Bool with a starting value and protection rules."))
	static void SecureBool_Initialize(UPARAM(ref) FSecureBool& Value, bool InitialValue, const FSecureBoolRules& Rules);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Set the current value of a Secure Bool."))
	static void SecureBool_Set(UPARAM(ref) FSecureBool& Value, bool NewValue);

	UFUNCTION(BlueprintPure, Category = "ShieldUE|Bool", meta = (ToolTip = "Get the decoded runtime value from a Secure Bool."))
	static bool SecureBool_Get(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Validate a Secure Bool and return whether its protected state is still valid."))
	static bool SecureBool_Validate(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Generate a new runtime key and re-encode the Secure Bool."))
	static void SecureBool_Rekey(UPARAM(ref) FSecureBool& Value);

	UFUNCTION(BlueprintCallable, Category = "ShieldUE|Bool", meta = (ToolTip = "Initialize a Secure Bool using the rules already stored inside the variable."))
	static void SecureBool_InitializeFromInternalRules(UPARAM(ref) FSecureBool& Value, bool InitialValue);
};