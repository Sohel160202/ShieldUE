FSecureTransformRules Rules;
Rules.ComponentRules.bUseRange = false;
Rules.ComponentRules.bEnableAutoRekey = true;
Rules.ComponentRules.RekeyIntervalSeconds = 5.0f;
Rules.DefaultValue = FTransform::Identity;

FSecureTransform ProtectedTransform;
ProtectedTransform.Initialize(GetActorTransform(), Rules);

// Update the component values through the protected API.
ProtectedTransform.Set(GetActorTransform());
ProtectedTransform.UpdateProtection(DeltaSeconds);
