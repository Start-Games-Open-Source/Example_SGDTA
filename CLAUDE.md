# SGDynamicTextAssets Ticket Workflow — HARD STOP RULE

> This rule applies whenever working on SGDynamicTextAssets tickets from `SGDynamicTextAssets_Tickets.json`.

**Complete exactly ONE task per response. After setting a task to `verify`, your response MUST end with the checkpoint summary. Do not write code, read files, or begin the next task in the same response — even if a prior user message said "proceed", "continue", or "ok". Those instructions apply one task at a time.**

Full workflow rules: @SGDynamicTextAssets_SystemPrompt.md

---

# Do not Act before Instructions
Do not jump into implementation or change any files unless clearly instructed to make changes. When the user's intent is ambiguous,
default to providing information, doing research, and providing recommendations and/or an implementation plan/proposal rather than taking action immediately.
Only proceed with edits, modifications, or implementations when the user explicitly requests them.

# Use Parallel Tool Calls 
If you intend to call multiple and there are no dependencies between the tool calls, make all the independent tool calls in parallel.
Maximize tool calls where possible to increase speed and effeciency without a loss of quality. 

> For example; when reading 3 files, run 3 tool calls in parallel to read all 3 files into context at the same time.

However, if some tool calls depend on previous calls to inform dependent values like the parameters, DO NOT call these tools in parallel
and instead call them sequentially. Never use placeholders or guess missing parameters in tool calls, 
if its required then query the user for those parameters so the information isn't missing.

# Investigate before Answering
Never speculate about code you have not opened. If the user references a specific file, you MUST read the file before answering.
Make sure to investigate and read relevant files BEFORE answering questions about the codebase. Never make any claims about code before
investigating unless you are certain of the correct answer. Give grounded and hallucination free answers.

# Documentation and Comment Style

**Never use em dashes** ( — or -- that render as em dashes) in code comments, documentation files, commit messages, or any written output. Em dashes are a hallmark of AI-generated text. Instead:
- Use a hyphen (-) where a dash is appropriate
- Reword the sentence to not need a dash at all

This applies to all output: C++ comments, Javadoc descriptions, Markdown documentation, JSON description fields, and chat responses about code.

---

# Project Guidelines

This document contains project-level instructions and coding standards for Unreal Engine projects.

---

## Project-Specific Configuration

> **Update this section for each project. Everything below this section is reusable across UE projects.**

### Project Description

**EmptyTestCPP** is a basic C++ Unreal Engine project used for testing and development.

### Engine Information

- **Engine Version:** 5.6
- **Engine Type:** Epic Games Launcher version (source code included for debugging)
- **Local Engine Location:** `%UE5ROOT%\UE_5.6`
- **Engine Naming Convention:** Engines are stored as `UE_[Version]` (e.g., `UE_5.6`, `UE_5.5`)

### Project Structure

```
Source/
└── EmptyTestCPP/
    ├── EmptyTestCPP.Build.cs
    ├── EmptyTestCPP.cpp
    └── EmptyTestCPP.h
```

### Enabled Plugins

`ModelingToolsEditorMode`

---

## Code Style Guidelines

### Comment Style

Use Javadoc-style comments (`/** */`) for all public API documentation. Unreal Header Tool (UHT) parses these to generate editor tooltips and documentation.

```cpp
/** Brief description of the function's purpose. */
void SimpleFunction();

/**
 * Detailed description for complex functions.
 * Explains the algorithm, side effects, or important behavior.
 *
 * @param DamageAmount The amount of damage to apply (must be >= 0)
 * @param DamageType The type of damage for resistance calculations
 * @param Instigator The controller responsible for the damage (can be nullptr)
 * @return True if the target was killed, false otherwise
 */
bool ApplyDamage(float DamageAmount, EDamageType DamageType, AController* Instigator);
```

**When to use `//` comments:**
- Inline implementation details within function bodies
- Macro guards in headers (`// MYCLASS_H`)
- Temporarily disabling code during debugging

**Never use `//` for:** Class, struct, enum, function, or property documentation.

#### Virtual Override Comment Style

Group overridden functions by their parent class using comment blocks. This improves readability when a class overrides many functions from different parents.

```cpp
// AActor overrides
virtual void BeginPlay() override;
virtual void Tick(float DeltaSeconds) override;
virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
// ~AActor overrides

// IMyInterface interface
virtual void OnInteract_Implementation(AActor* Interactor) override;
// ~IMyInterface interface
```

Use `override` suffix for base class overrides, `interface` suffix for interface implementations.

### Naming Conventions

| Element | Convention | Example | Rationale |
|---------|------------|---------|-----------|
| Local Variables | camelCase | `float deltaTime;` | Distinguishes from member variables at a glance |
| Function Parameters | PascalCase | `void SetHealth(float NewHealth)` | Matches Unreal convention, consistent with Blueprint |
| Member Variables | PascalCase | `float CurrentHealth;` | Unreal standard, works with UPROPERTY reflection |
| Static/Const Variables | SCREAMING_SNAKE_CASE | `static const int32 MAX_PLAYERS = 4;` | Immediately identifiable as constants |
| Functions | PascalCase | `void CalculateDamage()` | Unreal standard, matches Blueprint node names |
| Classes/Structs | PascalCase + Prefix | `class UHealthComponent` | Prefix indicates type: U=UObject, A=Actor, F=Struct |

### Boolean Variables

Prefer `uint8 : 1` bitfields over `bool` for member variables. This saves memory (multiple booleans pack into a single byte) and follows Unreal Engine conventions.

```cpp
// PREFERRED: Bitfield booleans - pack efficiently, default to false
uint8 bIsActive : 1;
uint8 bHasBeenInitialized : 1;
uint8 bCanTakeDamage : 1;

// In constructor - set non-false defaults explicitly
UMyComponent::UMyComponent()
{
    bCanTakeDamage = true;  // Override default false
}
```

Bitfield booleans are fully supported by Unreal's reflection system:
```cpp
// Bitfields work with UPROPERTY - use them for Blueprint-exposed bools too
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
uint8 bIsEnabled : 1;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
uint8 bHasBeenActivated : 1;
```

**When to use `bool` instead:**
```cpp
// Use bool for local variables (no memory benefit from bitfield)
void MyFunction()
{
    bool isValid = CheckValidity();  // Local variable, use bool
}

// Use bool when atomic operations are needed
std::atomic<bool> bThreadSafeFlag;  // Atomics don't work with bitfields

// Use bool when interfacing with external APIs that require it
void ExternalCallback(bool* OutResult);  // Can't take address of bitfield
```

**Naming:** Always prefix boolean variables with `b` (e.g., `bIsEnabled`, `bHasTarget`).

### Function Name Prefixes

Prefixes communicate function behavior and usage context. Always prefer prefix over suffix.

| Prefix | Usage | Example |
|--------|-------|---------|
| `Server_` | RPC executed only on the server | `Server_TakeDamage()` |
| `Client_` | RPC executed only on the owning client | `Client_PlayHitReaction()` |
| `Multicast_` | RPC executed on server and all clients | `Multicast_PlayDeathEffect()` |
| `Internal_` | Private implementation detail, not for external use | `Internal_ProcessDamage()` |
| `Native_` | C++ implementation called by Blueprint | `Native_CalculateTrajectory()` |
| `BP_` | Blueprint event/function, requires `DisplayName` UMETA | `BP_OnHealthChanged()` |

`BP_` prefixed functions **must** include a `DisplayName` specifier with a user-friendly name for Blueprint graphs:
```cpp
UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Health Changed"))
void BP_OnHealthChanged(float NewHealth, float OldHealth);
```

### Header File Organization

Organize class members in this order for consistency and cache-friendly memory layout:

```cpp
UCLASS()
class UMyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // 1. Constructors and initialization
    UMyComponent();

    // 2. Public functions (API)
    void PublicFunction();

    // 3. Public variables (ordered by size: largest first for alignment)
    UPROPERTY(EditAnywhere)
    float PublicFloat;

protected:
    // 4. Protected functions
    virtual void BeginPlay() override;

    // 5. Protected variables
    UPROPERTY()
    int32 ProtectedInt;

private:
    // 6. Private functions
    void PrivateHelper();

    // 7. Private variables (bitfield booleans grouped together)
    uint8 bIsActive : 1;
    uint8 bHasBeenDamaged : 1;

    // 8. Static members (at end of each section)
    static const int32 MAX_COUNT = 100;
};
```

**Key rules:**
- One `public:`, one `protected:`, one `private:` per class (no repeated access specifiers)
- Functions before variables in each section
- Order variables by size (largest first) to minimize padding
- Static members at the end of each visibility section

### Unreal Engine Best Practices

**Reflection Macros:** Every class, struct, enum, property, and function that needs Blueprint access or serialization must use the appropriate macro:
- `UCLASS()` - Classes deriving from UObject
- `USTRUCT()` - Value types that need reflection
- `UENUM()` - Enumerations for Blueprint/serialization
- `UPROPERTY()` - Member variables (required for GC, serialization, or Blueprint)
- `UFUNCTION()` - Functions callable from Blueprint or RPCs, CANNOT BE USED WITH STRUCTS

**Class Prefixes:**
| Prefix | Base Class | Example |
|--------|------------|---------|
| `U` | UObject | `UHealthComponent` |
| `A` | AActor | `AProjectile` |
| `F` | Struct (non-UObject) | `FDamageEvent` |
| `E` | Enum | `ETeamAffiliation` |
| `I` | Interface | `IDamageable` |
| `T` | Template | `TArray<>` |

**Struct Categories**
By default Unreal Engine does not support the `Categories` UPROPERTY keyword with structs.

**String Handling:** Always use `TEXT()` macro for string literals to ensure proper Unicode handling:
```cpp
UE_LOG(LogTemp, Log, TEXT("Player %s took %f damage"), *PlayerName, DamageAmount);
```

**Containers:** Prefer Unreal containers over STL for consistency and engine integration:
- `TArray<T>` instead of `std::vector<T>`
- `TMap<K,V>` instead of `std::unordered_map<K,V>`
- `TSet<T>` instead of `std::unordered_set<T>`

**Assertions:** Use the appropriate assertion for the situation:
- `check(Condition)` - Fatal in all builds, use for programming errors
- `ensure(Condition)` - Logs callstack but continues, use for recoverable errors
- `verify(Condition)` - Like check, but expression is always evaluated

**Other conventions:**
- Mark functions `const` when they don't modify object state
- Use `FORCEINLINE` only after profiling proves it helps
- Use `nullptr` instead of `NULL` or `0`
- Forward declare in headers, include in source files
- Do not use UIMin/UIMax UPROPERTY keywords, use ClampMin/ClampMax instead.

### Enums

Define enums as bitfields when values can be combined. Always include a zero value and use `DisplayName` for Blueprint readability.

```cpp
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDamageType : uint8
{
    None       = 0      UMETA(Hidden),                         // Required zero value
    Physical   = 1 << 0 UMETA(DisplayName = "Physical"),       // Bit 0
    Fire       = 1 << 1 UMETA(DisplayName = "Fire"),           // Bit 1
    Electric   = 1 << 2 UMETA(DisplayName = "Electric"),       // Bit 2
    Poison     = 1 << 3 UMETA(DisplayName = "Poison"),         // Bit 3
    All        = 0xFF   UMETA(Hidden)                          // All bits set
};
ENUM_CLASS_FLAGS(EDamageType);  // Enables bitwise operators
```

**Requirements:**
- Zero value (`None = 0`) for default initialization
- Bit-shifted values (`1 << N`) for combinable flags
- `DisplayName` on all user-facing values (prevents breakage if enum is renamed)
- `Hidden` on internal values (`None`, `All`)
- `ENUM_CLASS_FLAGS()` macro after enum for bitwise operator support

#### Enum Switch Statement Usage

Switch statements with enums should not be one line statements, they should follow proper spacing and use brakets.
There must ALWAYS be a default case to avoid undefined behavior.
Default case should have a comment(s) above it specifying which option(s) it is covering for readability reasons.

```cpp
EDamageType myEnum = EDamageType::Poison;

switch (myEnum)
{
    case EDamageType::None:
    {
        // Do stuff...
        
        break;
    }
    case EDamageType::Physical:
    case EDamageType::Fire:
    {
        // Do stuff...
        
        break;
    }    
    case EDamageType::Poison:    
    {
        // Do stuff...
        
        break;
    }
    // EDamageType::Electric
    // EDamageType::All
    default:
    {
        break;
    }
}
```

### Blueprint Exposure

Control how C++ exposes functionality to Blueprint:

```cpp
// Callable function - appears in Blueprint action menu
UFUNCTION(BlueprintCallable, Category = "Combat|Damage")
void ApplyDamage(float Amount);

// Pure function - no side effects, no exec pins in Blueprint
UFUNCTION(BlueprintPure, Category = "Combat|Health")
float GetHealthPercent() const;

// Editable property - visible in Details panel and Blueprint defaults
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Health")
float MaxHealth = 100.0f;

// Read-only property - Blueprint can read but not modify
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Health")
float CurrentHealth;
```

**Category naming:** Use `|` to create subcategories: `"Combat|Damage|Types"`

### Blueprint Native Events vs Implementable Events

Choose the right event type based on whether C++ provides default behavior:

**BlueprintImplementableEvent** - C++ calls it, Blueprint implements it. Use when there's no C++ default behavior:
```cpp
// C++ declares the event
UFUNCTION(BlueprintImplementableEvent, Category = "Events")
void BP_OnLevelComplete(int32 Score);

// C++ calls it
void AMyGameMode::CompleteLevel()
{
    CalculateFinalScore();
    BP_OnLevelComplete(FinalScore);  // Blueprint handles the response
}
```

**BlueprintNativeEvent** - C++ provides default behavior that Blueprint can override:
```cpp
// C++ declares with _Implementation
UFUNCTION(BlueprintNativeEvent, Category = "Events")
void OnTakeDamage(float Amount);

void AMyCharacter::OnTakeDamage_Implementation(float Amount)
{
    // Default C++ behavior
    PlayHitSound();
    FlashDamageIndicator();
}
```

**Recommended pattern** for safety: Use Internal + Implementable to ensure C++ logic always runs:
```cpp
// Internal function handles C++ logic
void AMyCharacter::TakeDamage(float Amount)
{
    CurrentHealth -= Amount;           // C++ logic ALWAYS runs
    BP_OnDamageTaken(Amount);          // Then notify Blueprint
}

// Blueprint event for optional responses
UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Damage Taken"))
void BP_OnDamageTaken(float Amount);
```

---

## Module Templates

Modules are the primary code organization unit in Unreal. Each module compiles to a separate library.

### Module Header (.h)

```cpp
// [Copyright information]

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// Log category for this module - use in UE_LOG calls
DECLARE_LOG_CATEGORY_EXTERN(Log[MODULE NAME]Module, All, All);

/**
 * Module interface for [MODULE NAME].
 * Provides static access to module functionality.
 */
class I[MODULE NAME]Module : public IModuleInterface
{
public:
    /**
     * Gets the module instance, loading it if necessary.
     * @warning Do not call during shutdown - module may be unloaded.
     */
    static inline I[MODULE NAME]Module& Get()
    {
        return FModuleManager::LoadModuleChecked<I[MODULE NAME]Module>("[MODULE NAME]");
    }

    /** Returns true if the module is loaded and ready to use. */
    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded("[MODULE NAME]");
    }
};
```

### Module Source (.cpp)

```cpp
// [Copyright information]

#include "[MODULE NAME]Module.h"

DEFINE_LOG_CATEGORY(Log[MODULE NAME]Module);

#define LOCTEXT_NAMESPACE "F[MODULE NAME]Module"

/**
 * Implementation of the [MODULE NAME] module.
 */
class F[MODULE NAME]Module : public I[MODULE NAME]Module
{
public:
    /** Called when the module is loaded into memory. */
    virtual void StartupModule() override
    {
        UE_LOG(Log[MODULE NAME]Module, Log, TEXT("[MODULE NAME] module initialized"));
    }

    /** Called when the module is unloaded from memory. */
    virtual void ShutdownModule() override
    {
        UE_LOG(Log[MODULE NAME]Module, Log, TEXT("[MODULE NAME] module shutdown"));
    }

    /**
     * Returns whether this is a gameplay module (true) or editor-only module (false).
     * Gameplay modules are loaded in packaged builds.
     */
    virtual bool IsGameModule() const override
    {
        return true;  // Set to false for editor-only modules
    }
};

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(F[MODULE NAME]Module, [MODULE NAME])
```

---

## Plugin Organization

Plugins encapsulate reusable functionality that can be shared across projects.

```
Plugins/[PluginName]/
├── [PluginName].uplugin          # Plugin descriptor (dependencies, modules, version)
├── Source/
│   └── [PluginName]/
│       ├── [PluginName].Build.cs # Build rules (dependencies, include paths)
│       ├── Public/               # Headers exposed to other modules
│       │   └── [PluginName]Module.h
│       └── Private/              # Implementation files (not visible externally)
│           └── [PluginName]Module.cpp
├── Content/                      # Optional: Blueprint assets, materials, etc.
└── README.md                     # Documentation for the plugin
```

---

## Component Template

Components are modular pieces of functionality that attach to Actors.

```cpp
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyComponent.generated.h"

/**
 * Brief description of what this component does.
 *
 * Detailed explanation of behavior, dependencies, and usage.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYMODULE_API UMyComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMyComponent();

    // UActorComponent overrides
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                              FActorComponentTickFunction* ThisTickFunction) override;
    // ~UActorComponent overrides

protected:
    // AActor overrides
    virtual void BeginPlay() override;
    // ~AActor overrides

private:
    /** Cached reference to owner's movement component. */
    UPROPERTY()
    TObjectPtr<UMovementComponent> CachedMovement;
};
```

---

## Interface Template

Interfaces define contracts that multiple unrelated classes can implement.

```cpp
// -------------------- Header --------------------
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

// UInterface class - required boilerplate for Unreal reflection
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
    GENERATED_BODY()
};

/**
 * Interface for actors that can receive damage.
 * Implement this on any actor that should respond to damage events.
 */
class IDamageable
{
    GENERATED_BODY()

public:
    /**
     * Apply damage to this actor.
     * @param Amount Damage amount (before resistances)
     * @param DamageType Type of damage for resistance calculations
     * @param Instigator Controller responsible for the damage
     * @return Actual damage dealt after resistances
     */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    float TakeDamage(float Amount, EDamageType DamageType, AController* Instigator);

    /** Returns true if this actor is still alive. */
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
    bool IsAlive() const;
};

// -------------------- Source --------------------
#include "Damageable.h"

float IDamageable::TakeDamage_Implementation(float Amount, EDamageType DamageType, AController* Instigator)
{
    // Default: apply full damage with no resistance
    return Amount;
}

bool IDamageable::IsAlive_Implementation() const
{
    // Default: always alive (implementers should override)
    return true;
}
```

---

## Network Replication

Unreal's replication system synchronizes state between server and clients.

### Property Replication

```cpp
// Header: Declare replicated property
UPROPERTY(ReplicatedUsing = OnRep_Health)
float Health;

/** Called on clients when Health replicates. */
UFUNCTION()
void OnRep_Health(float OldHealth);

// Source: Register property for replication
void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Basic replication - syncs to all clients
    DOREPLIFETIME(AMyCharacter, Health);

    // Conditional replication - only to owner
    DOREPLIFETIME_CONDITION(AMyCharacter, Ammo, COND_OwnerOnly);
}

void AMyCharacter::OnRep_Health(float OldHealth)
{
    // React to health change on client
    if (Health < OldHealth)
    {
        PlayDamageEffect();
    }
}
```

### Remote Procedure Calls (RPCs)

```cpp
// Server RPC - Client calls, Server executes
UFUNCTION(Server, Reliable, WithValidation)
void Server_RequestFire(FVector TargetLocation);

bool AMyCharacter::Server_RequestFire_Validate(FVector TargetLocation)
{
    // Return false to disconnect cheating clients
    return TargetLocation.Size() < MaxFireRange;
}

void AMyCharacter::Server_RequestFire_Implementation(FVector TargetLocation)
{
    // Execute on server
    PerformFire(TargetLocation);
}

// Client RPC - Server calls, specific Client executes
UFUNCTION(Client, Reliable)
void Client_NotifyHit(FHitResult HitInfo);

void AMyCharacter::Client_NotifyHit_Implementation(FHitResult HitInfo)
{
    // Execute on owning client only
    PlayHitMarkerUI();
}

// Multicast RPC - Server calls, Server + all Clients execute
UFUNCTION(NetMulticast, Unreliable)
void Multicast_PlayExplosion(FVector Location);

void AMyCharacter::Multicast_PlayExplosion_Implementation(FVector Location)
{
    // Execute everywhere for cosmetic effects
    SpawnExplosionVFX(Location);
}
```

---

## Memory Management

Unreal uses garbage collection for UObjects. Understanding ownership prevents leaks and crashes.

### Object Creation

```cpp
// NewObject - Create UObjects at runtime
// Outer determines lifecycle: object is destroyed when Outer is destroyed
UMyObject* Obj = NewObject<UMyObject>(this, TEXT("MyObject"));

// CreateDefaultSubobject - ONLY in constructors
// Creates components that appear in the CDO (Class Default Object)
MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

// SpawnActor - Create Actors in the world
FActorSpawnParameters Params;
Params.Owner = this;
AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, Params);
```

### Deferred Actor Spawning

Use deferred spawning when you need to configure an actor before it enters the world (before `BeginPlay` is called):

```cpp
// Deferred spawn - actor is created but NOT yet in the world
FActorSpawnParameters Params;
Params.bDeferConstruction = true;  // Key flag for deferred spawning
Params.Owner = this;

AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnTransform, Params);

// Actor exists but BeginPlay has NOT been called yet
// Safe to configure properties that affect initialization
Proj->SetDamage(CalculatedDamage);
Proj->SetInstigator(GetInstigator());
Proj->bShouldExplodeOnImpact = true;

// NOW finish spawning - this calls BeginPlay and fully activates the actor
Proj->FinishSpawning(SpawnTransform);
```

**When to use deferred spawning:**
- Setting properties that `BeginPlay` depends on
- Configuring components before they initialize
- Setting up references that the actor needs during initialization
- Avoiding multiple initialization passes

**Important notes:**
- The actor exists in memory but is not in the world until `FinishSpawning`
- `BeginPlay` is called inside `FinishSpawning`
- You must call `FinishSpawning` or the actor will never fully initialize
- Pass the same transform or an updated one to `FinishSpawning`

```cpp
// Common pattern: Factory function with deferred spawning
AEnemy* UEnemySpawner::SpawnEnemy(TSubclassOf<AEnemy> EnemyClass, const FEnemyConfig& Config)
{
    FActorSpawnParameters Params;
    Params.bDeferConstruction = true;
    Params.Owner = this;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    AEnemy* Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, Config.SpawnTransform, Params);
    if (!Enemy) return nullptr;

    // Configure before BeginPlay
    Enemy->SetTeam(Config.Team);
    Enemy->SetDifficulty(Config.Difficulty);
    Enemy->SetPatrolPath(Config.PatrolPath);

    // Finish spawning - BeginPlay will now have access to configured values
    Enemy->FinishSpawning(Config.SpawnTransform);

    return Enemy;
}
```

### Runtime Component Creation

When creating components at runtime (outside constructors), you must properly register them with the owning actor:

```cpp
// Creating a component at runtime - full setup required
UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(this, TEXT("DynamicMesh"));

// CRITICAL: Add to owner's component list BEFORE registering
AddOwnedComponent(NewMesh);

// Setup attachment (optional, depends on component type)
NewMesh->SetupAttachment(RootComponent);

// Register the component - this activates it in the world
NewMesh->RegisterComponent();

// Now safe to configure
NewMesh->SetStaticMesh(SomeMesh);
NewMesh->SetWorldLocation(SpawnLocation);
```

**Why `AddOwnedComponent` matters:**
- Ensures the component is tracked by the actor's `OwnedComponents` array
- Required for proper replication of dynamically created components
- Ensures cleanup when the actor is destroyed
- Must be called **before** `RegisterComponent()`

```cpp
// Helper function pattern for runtime component creation
template<typename T>
T* CreateRuntimeComponent(AActor* Owner, FName Name, USceneComponent* Parent = nullptr)
{
    T* NewComponent = NewObject<T>(Owner, Name);
    Owner->AddOwnedComponent(NewComponent);

    if (Parent && NewComponent->IsA<USceneComponent>())
    {
        Cast<USceneComponent>(NewComponent)->SetupAttachment(Parent);
    }

    NewComponent->RegisterComponent();
    return NewComponent;
}
```

### Reference Types

| Type | Use Case | GC Behavior |
|------|----------|-------------|
| `UPROPERTY() TObjectPtr<T>` | Standard UObject reference | Prevents GC of referenced object |
| `TWeakObjectPtr<T>` | Non-owning reference | Does NOT prevent GC, check `IsValid()` before use |
| `TStrongObjectPtr<T>` | Non-UPROPERTY member | Prevents GC, use when UPROPERTY not possible |
| `TSoftObjectPtr<T>` | Asset reference (lazy load) | Does not load asset until accessed |

```cpp
// Weak reference example - safe for caching objects you don't own
TWeakObjectPtr<AActor> CachedTarget;

void UpdateTarget(AActor* NewTarget)
{
    CachedTarget = NewTarget;
}

void TickAI()
{
    if (CachedTarget.IsValid())  // Always check before use!
    {
        MoveToward(CachedTarget.Get());
    }
}
```

---

## Build Configuration

### Agent Build Restrictions
> [!IMPORTANT]
> **The LLM is NOT permitted to build the code.**
> The LLM *must* ask the user to build and verify that it compiles.
> If there are any errors, the user must provide the error output.
> This applies to all build commands.

### Build Targets

| Configuration | Use Case |
|--------------|----------|
| `Development Editor` | Primary development - debugging enabled, editor tools available |
| `DebugGame Editor` | Deep debugging - slower but more debug info |
| `Development` | Packaged test builds |
| `Shipping` | Final release - no debug code, optimized |

**Critical:** Always build the **project target**, never the engine. Building the engine can take hours and cause unexpected issues.

### Compilation

```
Build target: [ProjectName] (e.g., RoombaPrototype)
Platform: Win64
Configuration: Development Editor
```

---

## Testing

Unreal's Automation Framework provides unit and functional testing.

### Test Structure

```
Plugins/[PluginName]/Source/[PluginName]Tests/
├── [PluginName]Tests.Build.cs
├── Public/
│   └── [PluginName]TestsModule.h
└── Private/
    ├── [Feature]Tests.cpp
    └── [OtherFeature]Tests.cpp
```

### Writing Tests

```cpp
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FHealthComponent_TakeDamage_ReducesHealth,           // Test class name
    "ModuleName.Components.HealthComponent.TakeDamage",     // Test path in UI
    EAutomationTestFlags::EditorContext |                 // Runs in editor
    EAutomationTestFlags::ProductFilter |                 // Included in automated runs
    EAutomationTestFlags::MediumPriority                  // Execution priority
)

bool FHealthComponent_TakeDamage_ReducesHealth::RunTest(const FString& Parameters)
{
    // Arrange
    UHealthComponent* Health = NewObject<UHealthComponent>();
    Health->SetMaxHealth(100.0f);
    Health->SetHealth(100.0f);

    // Act
    Health->TakeDamage(30.0f);

    // Assert
    TestEqual(TEXT("Health should be reduced by damage amount"), Health->GetHealth(), 70.0f);

    return true;
}
```

### Running Tests

- **Editor:** Window → Developer Tools → Session Frontend → Automation tab
- **Command line:** `RunUAT.bat BuildCookRun -project=[Project] -platform=Win64 -test`

### Debug Logging

```cpp
// Module-specific log category (declare in module header)
DECLARE_LOG_CATEGORY_EXTERN(LogCombat, Log, All);

// In source file
DEFINE_LOG_CATEGORY(LogCombat);

// Usage with verbosity levels
UE_LOG(LogCombat, Log, TEXT("Player dealt %f damage to %s"), Damage, *Target->GetName());
UE_LOG(LogCombat, Warning, TEXT("Damage amount negative: %f"), Damage);
UE_LOG(LogCombat, Error, TEXT("Failed to apply damage - target is null"));

// Temporary debug logging (easy to find and remove later)
UE_LOG(LogTemp, Log, TEXT("[DEBUG] Value: %d"), SomeValue);
```

---

## Performance Guidelines

### General Principles

1. **Profile before optimizing** - Use Unreal Insights, stat commands, or external profilers to identify actual bottlenecks
2. **Measure after changes** - Verify optimizations actually improved performance
3. **Readability matters** - Don't sacrifice code clarity for micro-optimizations without profiler data

### Memory Access & Cache Efficiency

Modern CPUs are fast; memory access is the bottleneck. Optimize for cache hits:

**Data layout:**
```cpp
// GOOD: Contiguous data, cache-friendly iteration
TArray<FTransform> Transforms;  // All transforms in one memory block
for (const FTransform& T : Transforms)
{
    ProcessTransform(T);  // Sequential memory access
}

// BAD: Scattered memory, cache misses
TArray<AActor*> Actors;
for (AActor* Actor : Actors)
{
    ProcessTransform(Actor->GetActorTransform());  // Pointer chase = cache miss
}
```

**Struct organization:**
```cpp
// GOOD: Hot data together, ordered by size
struct FProjectileData
{
    // Hot data (accessed every frame) - grouped together
    FVector Position;           // 24 bytes
    FVector Velocity;           // 24 bytes
    float Lifetime;             // 4 bytes
    float Damage;               // 4 bytes

    // Cold data (accessed rarely) - at the end
    FName ProjectileType;       // 8 bytes
    TWeakObjectPtr<AActor> Owner; // 8 bytes
};

// BAD: Mixed hot/cold, poor alignment
struct FProjectileDataBad
{
    bool bIsActive;             // 1 byte + 7 padding
    FVector Position;           // 24 bytes
    FName ProjectileType;       // 8 bytes (rarely used, splits hot data)
    FVector Velocity;           // 24 bytes
};
```

**Array of Structs vs Struct of Arrays:**
```cpp
// AoS - Good for accessing all properties of one entity
struct FEnemy { FVector Pos; float Health; };
TArray<FEnemy> Enemies;

// SoA - Good for processing one property across all entities
struct FEnemyArrays
{
    TArray<FVector> Positions;   // Process all positions together
    TArray<float> Healths;       // Process all health values together
};
```

### Tick Optimization

```cpp
// BAD: Ticking when nothing to do
virtual void Tick(float DeltaTime) override
{
    if (!bShouldUpdate) return;  // Tick still called!
    DoExpensiveWork();
}

// GOOD: Disable tick when not needed
void SetUpdating(bool bShouldUpdate)
{
    SetComponentTickEnabled(bShouldUpdate);
}

// BETTER: Use timers for infrequent updates
void BeginPlay()
{
    // Check every 0.5 seconds instead of every frame
    GetWorld()->GetTimerManager().SetTimer(
        CheckTimerHandle,
        this, &UMyComponent::PeriodicCheck,
        0.5f,  // Interval
        true   // Looping
    );
}
```

### Object Pooling

Avoid spawn/destroy overhead for frequently created objects:

```cpp
// Object pool for projectiles
TArray<AProjectile*> ProjectilePool;

AProjectile* GetProjectile()
{
    for (AProjectile* Proj : ProjectilePool)
    {
        if (!Proj->IsActive())
        {
            Proj->Activate();
            return Proj;
        }
    }
    // Pool exhausted - spawn new (consider logging this)
    return SpawnNewProjectile();
}

void ReturnProjectile(AProjectile* Proj)
{
    Proj->Deactivate();
    Proj->SetActorLocation(FVector::ZeroVector);  // Move out of gameplay area
}
```

### Caching

```cpp
// BAD: Repeated lookups every frame
void Tick(float DeltaTime)
{
    UMovementComponent* Movement = GetOwner()->FindComponentByClass<UMovementComponent>();
    Movement->SetVelocity(CalculateVelocity());
}

// GOOD: Cache in BeginPlay or initialization
UPROPERTY()
TObjectPtr<UMovementComponent> CachedMovement;

void BeginPlay()
{
    CachedMovement = GetOwner()->FindComponentByClass<UMovementComponent>();
    check(CachedMovement);  // Fail fast if required component missing
}

void Tick(float DeltaTime)
{
    CachedMovement->SetVelocity(CalculateVelocity());
}
```

### Rendering

- **Instanced Static Meshes:** Use for repeated geometry (foliage, debris, crowds)
- **LODs:** Set up Level of Detail for complex meshes
- **Occlusion:** Let Unreal's occlusion culling work by keeping levels reasonably segmented
- **Draw calls:** Merge materials where possible, use texture atlases

---

## Additional Resources

### Documentation

- [Epic C++ Coding Standard](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)
- [Unreal Engine API Reference](https://dev.epicgames.com/documentation/en-us/unreal-engine/API)
- [Gameplay Framework Overview](https://dev.epicgames.com/documentation/en-us/unreal-engine/gameplay-framework)

### Testing Checklist

- [ ] Test in Editor (PIE - Play In Editor)
- [ ] Test in Standalone Game
- [ ] Test in packaged Development build
- [ ] Test edge cases: zero, negative, max values, null pointers
- [ ] Test network scenarios: host, client, high latency
