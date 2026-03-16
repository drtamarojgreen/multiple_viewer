# The CHAI Restrictions Engine (CRE)

## Overview

The **CHAI Restrictions Engine (CRE)** is the structural enforcement layer of **Chai Driven Development (CDD)**.

Its purpose is to prevent AI agents from generating large volumes of unverified code by forcing development to occur through **incremental, executable units**.

Instead of allowing an AI agent to produce hundreds or thousands of lines of speculative implementation, the CRE enforces a workflow where the agent must:

1. Construct a **minimal functional unit**
2. Execute or validate that unit
3. Expand the system only after validation

The CRE transforms AI development from **speculative generation** into **verified construction**.

---

# The Core Problem CRE Solves

AI agents operate through **linear token prediction**.

They generate code in sequences like:

```
token → token → token → token
```

This process encourages agents to produce **large blocks of code that appear complete**, even though they have never been executed or validated.

Because the agent has no runtime responsibility, it may generate:

* unused modules
* incorrect abstractions
* nonfunctional logic
* missing dependencies

The result is often **hundreds of lines of unusable code**.

The CHAI Restrictions Engine prevents this by restructuring the generation process.

---

# CDD’s Incremental Construction Model

Traditional agent workflows often look like this:

```
Prompt → AI writes 800 lines of code → debugging begins
```

CDD enforces a different model:

```
Prompt
↓
Create minimal module
↓
Validate execution
↓
Extend system
↓
Repeat
```

The CRE enforces this cycle automatically.

---

# The Responsibility Principle

The Restrictions Engine introduces a new concept: **AI responsibility boundaries**.

An agent is only allowed to generate code that it can reasonably verify.

This prevents the agent from constructing systems that exceed its reasoning capability.

For example:

**Disallowed generation**

```
server
database
authentication
caching layer
API router
```

Generated all at once.

**Allowed generation**

```
core server loop
↓
verify execution
↓
add request handler
↓
verify execution
↓
add routing
```

Each step must produce a working artifact before expansion occurs.

---

# The Core Module Rule

One of the most important restrictions enforced by CRE is the **Core Module Rule**.

The agent must begin by constructing the smallest meaningful executable unit.

Example:

```
class Application
{
    int run();
}
```

This core module must compile and execute successfully before additional components are introduced.

The system then grows **around the working center**.

This mirrors how experienced engineers build reliable systems.

---

# Circular Construction vs Linear Generation

AI agents naturally operate in **linear construction**.

```
A → B → C → D → E
```

But reliable software systems require **circular validation**.

```
build → run → verify → extend → repeat
```

The Restrictions Engine forces the agent into this circular development cycle.

This ensures each stage produces **functional infrastructure**.

---

# Restriction Categories

The CRE enforces several categories of development restrictions.

---

## Structural Restrictions

These define architectural limits.

Examples:

```
max_classes_per_module
max_dependencies
no_empty_catch_blocks
```

These rules prevent structural anti-patterns common in AI-generated code.

---

## Tool Restrictions

Agents may only use tools declared in the Situation.

Example:

```
TOOLS
filesystem
zlib
END
```

The agent cannot import unknown libraries or invent dependencies.

---

## Incremental Growth Restrictions

The system prevents large architectural jumps.

Example restrictions:

```
max_lines_per_iteration
max_modules_created
max_abstraction_layers
```

This forces the agent to build systems gradually.

---

## Execution Validation Restrictions

Before new modules can be created, existing modules must demonstrate basic functionality.

Examples:

```
core_function_executes
class_instantiation_valid
basic_io_operational
```

This ensures that expansion occurs only after validation.

---

# Example: Without CRE

Prompt:

```
Write a C++ HTTP server
```

Typical AI output:

* server class
* router
* middleware
* logging
* thread pool
* configuration system
* error handler

Hundreds of lines of speculative code appear instantly.

Many parts will be incorrect or unnecessary.

---

# Example: With CRE

Step 1 — Core module

```
class Server
{
public:
    void run();
};
```

Verify execution.

---

Step 2 — Minimal socket handling

```
open socket
accept connection
close socket
```

Verify execution.

---

Step 3 — Request parsing

Add HTTP parsing logic.

Verify execution.

---

Step 4 — Routing

Introduce routing once request handling is proven.

---

The system grows from **working infrastructure**, not speculative architecture.

---

# Token Efficiency

One of the major benefits of the Restrictions Engine is **token economy**.

Without restrictions, agents may produce thousands of tokens of faulty code.

With CRE:

* each step is small
* validation happens early
* failures appear quickly

This dramatically reduces wasted token usage.

---

# Aligning AI Behavior With Engineering Discipline

Human engineers naturally apply incremental validation because they understand the cost of large failures.

AI agents do not share this intuition.

The CRE effectively **teaches engineering discipline through enforced constraints**.

Instead of trusting the agent to behave responsibly, the system **guarantees responsible behavior structurally**.

---

# How CRE Integrates With CHAI

Within the CHAI architecture:

```
Situation
↓
Reasoning Gates
↓
Restrictions Engine
↓
Card Execution
↓
Evaluation
```

The Restrictions Engine ensures the agent cannot bypass the structural constraints defined in the Situation.

---

# The Key Idea

The CHAI Restrictions Engine converts AI development from:

```
language generation
```

into:

```
incremental system construction
```

The agent is no longer rewarded for producing large volumes of code.

Instead it is rewarded for producing **small, verifiable units that progressively form a complete system**.

---

# The Philosophy Behind CRE

AI agents do not think about responsibility, ownership, or long-term maintainability.

They think in **the next token**.

The CHAI Restrictions Engine bridges this gap by forcing the development process to revolve around **working code rather than generated language**.

In doing so, it aligns AI output with the fundamental principle of reliable engineering:

**a system should only grow from parts that already work.**