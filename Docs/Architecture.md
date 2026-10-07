# ShieldUE Architecture

## Current model

ShieldUE currently provides protected Unreal `USTRUCT` value types:

```text
FSecureFloat
FSecureInt32
FSecureBool
```

Each value stores an encoded representation, a shadow representation, a runtime key, an integrity value, the last valid value, and diagnostic state.

The protected value is checked when the game calls `Get()` or `Validate()`. If validation fails, the configured recovery policy is applied.

## Automatic re-keying

Automatic re-keying is opt-in. A caller configures the interval and calls:

```cpp
Value.UpdateProtection(DeltaSeconds);
```

ShieldUE intentionally does not create a hidden Tick for every value. This keeps update cost explicit and allows a project to choose an appropriate owner, component, or subsystem.

## Planned runtime architecture

The planned higher-level architecture is:

```text
GameInstance
    └── UShieldUERuntimeSubsystem
            └── registered protection owners/components
                    └── secure values and tamper events
```

The subsystem and event layer are not yet part of the plugin. They require Unreal Engine integration testing because protected values are structs whose memory addresses and lifetimes can change when their owners move, duplicate, or are destroyed.

## Design principles

- Keep protected values usable from both C++ and Blueprint.
- Make validation and recovery explicit.
- Keep testing-only operations visibly separate.
- Avoid promising protection beyond the stated threat model.
- Do not replace server authority with client-side obfuscation.
