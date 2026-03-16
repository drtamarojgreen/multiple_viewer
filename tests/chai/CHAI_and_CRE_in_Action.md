# CHAI and CRE in Action

## Overview

CHAI operates on a simple but powerful principle:

**Agents must understand the environment before they are allowed to implement the system.**

Traditional workflows ask AI agents to immediately produce implementation code. This often leads to incorrect assumptions about:

* platform capabilities
* available libraries
* compiler behavior
* runtime environment

CHAI reverses this order.

Instead of starting with application code, the agent begins by generating **test classes that probe the environment**.

Through this process, the agent gradually learns the system it is building for.

---

# The CHAI Development Loop

In a CHAI-driven workflow, prompts form an iterative loop between the AI agent and the execution environment.

The cycle looks like this:

```
Agent writes test class
↓
Environment compiles test
↓
Compilation results returned
↓
Agent updates understanding
↓
Next test class generated
```

Over time the agent builds an internal model of the environment.

This discovery process becomes the **foundation for CDD testing**.

---

# Why Test Classes Come First

AI agents frequently fail when writing implementation code first because they do not know the constraints of the system.

Examples of common failures include:

* using unavailable libraries
* assuming compiler features that are not present
* writing code incompatible with the build system
* relying on unsupported OS behavior

By requiring test classes first, CHAI forces the agent to **validate assumptions before building systems**.

The agent effectively performs **environment reconnaissance**.

---

# Example Interaction

## Prompt 1

The agent is asked to write a test class.

```
Create a minimal test class to verify C++ compilation.
```

The agent produces something like:

```cpp
class CompileTest {
public:
    int run() {
        return 0;
    }
};
```

---

## Prompt 2

The compilation output is returned to the agent.

Example response:

```
Compilation successful
Compiler: GCC 13
Standard: C++17
```

Now the agent knows:

* the compiler
* the language standard
* that compilation works

---

## Prompt 3

The agent writes another test.

```
Verify filesystem access.
```

Example:

```cpp
#include <filesystem>
```

If compilation fails, the agent learns something about the environment.

Over multiple iterations, the agent develops **environment awareness**.

---

# From Environment Discovery to System Construction

Once the agent understands the environment, it can begin constructing the system.

But now construction occurs with **validated knowledge**.

Instead of guessing, the agent knows:

* what libraries are available
* what language features exist
* how the build system behaves

This drastically reduces implementation errors.

---

# The Role of the CHAI Restrictions Engine

The **CRE** ensures the agent cannot skip this discovery process.

Restrictions might enforce rules such as:

```
no_application_code_before_tests
test_class_required
compile_validation_required
```

This guarantees that the agent always begins with environment validation.

The CRE therefore prevents agents from generating large volumes of speculative code.

---

# The CDD Repository Structure

A CHAI project begins with a directory structure designed for AI collaboration.

```
cdd/
cdd/facts/
cdd/cards/
```

These directories represent the core artifacts of the CDD system.

---

# Facts

Facts represent **validated knowledge about the environment**.

They are derived from successful tests and environment discovery.

Examples might include:

* compiler version
* supported language standard
* available libraries
* operating system characteristics

Facts serve as the **ground truth** for the agent.

Unlike natural language descriptions, facts are **machine-verifiable observations**.

---

# Cards

Cards represent **atomic development tasks**.

They replace traditional BDD step definitions.

A card describes a structured action involving:

* tools
* parameters
* expected results

Cards are intentionally small so that AI agents can complete them reliably.

Each card becomes a **bounded reasoning task**.

---

# Situations

Facts and cards combine to form **Situations**.

A Situation represents the environment in which the AI agent operates.

It includes:

* available tools
* known facts
* development objectives
* restrictions

Situations replace traditional BDD scenarios.

Instead of describing behavior narratively, Situations describe **operational environments**.

---

# How This Improves AI Reliability

CHAI significantly improves reliability by aligning with how AI systems actually work.

AI models excel at:

* generating structured text
* adapting to feedback
* iterating on constrained tasks

They struggle with:

* understanding unseen environments
* predicting runtime behavior
* managing large architectural systems

CHAI addresses these limitations by making the development process **incremental and feedback-driven**.

---

# CHAI Builds CHAI

An important philosophical element of the project is that **CHAI itself was developed using CHAI principles**.

This demonstrates that the methodology is not theoretical.

The framework was built through the same iterative loop:

```
test class
compile
feedback
refine
```

The resulting artifacts now form the initial **CDD core tests**.

These tests define the environment and development rules that future agents will follow.

---

# Why Facts and Cards Replace Features and Steps

BDD terminology assumes human understanding of behavior.

Terms like *features* and *steps* require interpretation.

AI agents do better with **explicit structures**.

CDD replaces these concepts with:

Facts
Cards

Facts describe **what is known**.

Cards describe **what must be done**.

This separation eliminates ambiguity.

---

# The Fundamental Shift

Traditional development assumes developers know the system before building it.

CHAI assumes that **AI agents must learn the system first**.

The development process therefore begins with **environment discovery** rather than implementation.

This discovery phase becomes the foundation for the rest of the system.

---

# The Result

By combining:

* CHAI Situations
* the Restrictions Engine
* incremental test discovery
* Facts and Cards

CDD creates a development process where AI agents progressively acquire knowledge about their environment before constructing systems.

The result is a workflow where AI agents generate **far less broken code**, because they are forced to **learn before they build**.

---

If you'd like, the next very powerful step would be designing the **CHAI Facts Specification**, because facts become the **memory layer for AI agents working across multiple prompts and sessions**. That is where CHAI starts turning into a full **AI-native development operating system**.