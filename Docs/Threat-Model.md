# ShieldUE Threat Model

## Intended threat

ShieldUE is intended to detect or recover from simple runtime edits such as:

- Searching for a visible health, score, or currency value
- Replacing a stored value with a different value
- Corrupting one part of a protected value
- Accidentally loading an invalid or out-of-range value

The encoded value, shadow value, and integrity check make these edits more difficult and provide a way to detect inconsistency.

## Out of scope

ShieldUE does not guarantee protection against:

- A determined attacker reverse-engineering the plugin
- An attacker modifying the executable or plugin code
- An attacker changing all related fields consistently
- Debuggers, injected code, or kernel-level tools
- Values that the game exposes plainly while actively using them
- A malicious or compromised server

## Multiplayer boundary

ShieldUE must not be used as the final authority for competitive state. The server should validate important requests and own the authoritative copy of currency, inventory, score, progression, rankings, and purchases.

## Terminology

- **Obfuscation:** Makes direct memory searches less convenient.
- **Integrity validation:** Detects inconsistent or modified state.
- **Recovery:** Restores a configured value after a validation failure.
- **Authority:** Determines which system is trusted to decide the correct value.

ShieldUE provides the first three. It does not create authority on the client.
