# ShieldUE Multiplayer Integrity

ShieldUE's multiplayer layer is designed around Unreal Engine's server-authoritative model. It does not attempt to make a client trustworthy. Instead, the server validates requests and records suspicious behavior.

## Components

### `UShieldUEMultiplayerSubsystem`

A `UGameInstanceSubsystem` that registers anti-tamper components and records server-side violation events.

### `UShieldUEAntiTamperComponent`

A Blueprint-spawnable component that can be attached to a player, inventory, weapon, or other gameplay actor. It provides reusable checks for:

- Int32 mismatches
- Int64 mismatches
- Float mismatches
- Rate-limit violations
- Suspicion scoring
- Restriction thresholds

It also exposes Blueprint events for violations, recovery, and validation failures.

## Example flow

```text
Client request
    ↓
Server receives request
    ↓
Server checks authority and gameplay rules
    ↓
Valid request → update server state and replicate
Invalid request → reject, record violation, increase suspicion
```

## Important limitations

- Validation functions should be called on the server.
- A client can modify its own local copy of any client-visible state.
- A listen-server host has authority over its session; dedicated servers are preferred for competitive games.
- ShieldUE does not automatically know a project's fire rate, inventory rules, movement limits, or economy rules.
- Projects must supply the server-side values and call the appropriate validation functions.

## Recommended usage

Use ShieldUE as one layer in a broader server-security design:

1. Keep authoritative gameplay state on the server.
2. Validate every important client request.
3. Rate-limit actions and reject impossible transitions.
4. Record violations instead of immediately banning on one signal.
5. Combine multiple signals before applying restrictions.
