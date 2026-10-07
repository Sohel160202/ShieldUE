# ShieldUE

ShieldUE is a runtime gameplay-integrity plugin for Unreal Engine. It helps developers detect and recover from naïve memory edits to gameplay-critical values such as health, currency, score, ammunition, and progression.

ShieldUE supports both C++ and Blueprint workflows.

## Current features

- Protected Float, Int32, and Bool values
- Encoded runtime storage
- Shadow-value verification
- Integrity validation
- Tamper counters and tamper reasons
- Configurable recovery modes
- Range limits for Float and Int32 values
- Manual runtime re-keying
- Optional automatic re-keying through `UpdateProtection`
- Blueprint and C++ APIs

## Important security scope

ShieldUE is a runtime integrity and tamper-detection layer. It is not a complete anti-cheat system, encryption library, or replacement for server-authoritative multiplayer design.

For competitive multiplayer games, the server must remain authoritative for currency, score, inventory, progression, and other important state. ShieldUE can help protect local state and detect suspicious client-side manipulation, but it should not be treated as the final authority.

## Installation

1. Download the repository or a release package.
2. Copy the `ShieldUE` folder into your Unreal project's `Plugins` directory.
3. Open the project and enable ShieldUE from **Edit → Plugins → Security**.
4. Rebuild the project if you are using C++.

ShieldUE currently targets Unreal Engine 5.7. Compatibility with other engine versions may require source adjustments.

## C++ example

```cpp
FSecureInt32Rules Rules;
Rules.bUseRange = true;
Rules.MinValue = 0;
Rules.MaxValue = 1000000;
Rules.DefaultValue = 100;
Rules.RecoveryMode = ESecureValueRecoveryMode::RestoreLastValid;
Rules.bEnableAutoRekey = true;
Rules.RekeyIntervalSeconds = 5.0f;

SecureCurrency.Initialize(100, Rules);
SecureCurrency.Set(250);

const int32 CurrentCurrency = SecureCurrency.Get();
if (!SecureCurrency.Validate())
{
    UE_LOG(LogTemp, Warning, TEXT("Currency validation failed"));
}
```

When automatic re-keying is enabled, update the value from an actor, component, or subsystem:

```cpp
SecureCurrency.UpdateProtection(DeltaSeconds);
```

## Blueprint workflow

For a protected value:

1. Create a `Secure Float`, `Secure Int`, or `Secure Bool` variable.
2. Initialize it with the appropriate ShieldUE node.
3. Use the ShieldUE `Get` and `Set` nodes instead of reading or writing the value directly.
4. Call `Update Protection` from Tick when automatic re-keying is enabled.
5. Use `Validate`, `Get Tamper Count`, and `Get Last Tamper Reason` for diagnostics or response logic.
6. Use `Reset To Default` when your game intentionally wants to restore the configured fallback value.

## Recovery modes

| Mode | Behavior |
| --- | --- |
| Restore Last Valid | Re-encodes the most recent valid value |
| Clamp To Range | Restores the last valid value and clamps it to the configured range |
| Reset To Default | Replaces the value with the configured default |

## Automatic re-keying

Automatic re-keying is opt-in. Configure `bEnableAutoRekey` and `RekeyIntervalSeconds`, then call `UpdateProtection(DeltaSeconds)` regularly. ShieldUE does not create a hidden global Tick for every protected value, which keeps the feature explicit and lets developers choose the appropriate update frequency.

## Testing tamper detection

The C++ structs include `CorruptForTesting()` for controlled development tests. A test can corrupt a value, call `Validate()` or `Get()`, and confirm that the configured detection and recovery behavior occurs.

Do not use testing corruption functions in shipping gameplay code.

## Roadmap

- Automation tests and performance benchmarks
- Sample Unreal project
- ShieldUE runtime inspection tools
- Secure Int64 and Double types
- Secure containers and save-game integrity helpers
- More detailed multiplayer integration guidance

## Links

- [ShieldUE project page](https://shieldue.iamsohel.xyz/)
- [Demonstration video](https://youtu.be/pkV4bPpjEOI)
- [Developer: Sheikh Sohel Moon](https://iamsohel.xyz)

## License

ShieldUE is distributed under the MIT License. See [LICENSE](LICENSE).
