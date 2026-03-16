# CHAI: Construction Harness of Artificial Intelligence

This document summarizes the core principles, concepts, and architectural decisions made for the CHAI project, developed through Chai Driven Development (CDD).

## 1. What CHAI Is (and Isn't)

CHAI is a **development structure** designed to make AI agents **useful collaborators for humans** by aligning AI strengths (generation, adaptation) with software engineering realities (long-term context, architectural discipline, verified execution).

-   **Not** an operating system, an AI memory system, or an autonomous development framework.
-   **Is** a discipline enforcer and a constraint engine around agent behavior.

## 2. Chai Driven Development (CDD) Philosophy

CDD is a methodology for AI collaboration and evaluation, designed **around AI limitations** (context, navigation, tool hallucination, over-editing). It prioritizes **restrictions** over requirements.

-   **Focus:** How code is *allowed to be constructed*, not just what it does.
-   **Core Principle:** AI agents participate by generating small, verifiable artifacts within a controlled environment, learning through observation rather than assumption.

### Why CDD Differs from BDD

-   BDD measures **success of execution** (step completion).
-   CDD measures **truth of observation** (empirical results).
-   CDD removes self-reporting mechanisms for correctness; agents must produce **evidence instead of approval**.

## 3. Green Syntax

A structured format designed for machine reasoning first, replacing natural language ambiguity with explicit blocks.

-   **Elements:** `TOOLS`, `PARAMETERS`, `RESULTS`.
-   **Purpose:** Minimizes ambiguity, easier for AI to parse and interact with.

## 4. Key CDD Primitives

### a. Situation
-   **Definition:** A specific environmental state described in a `.facts` file.
-   **Syntax:** Defined by `Situation: Name` headers.
-   **Purpose:** Allows cards to be evaluated against different system states (e.g., `LowResources`, `Production`).

### b. Class / Card
-   **Class:** A C/C++ source file (e.g., `SystemClass.cpp`) representing a domain of cards.
-   **Card:** An atomic unit of work defined via `// @Card: name` inside a class file.
-   **Structure:**
    ```cpp
    // @Card: check_os
    // @Is platform == linux
    // @Results os_type == linux
    ```

### c. Fact
-   **Definition:** A validated, immutable piece of knowledge about the environment or system, derived from empirical observation.
-   **Structure:** Prefixed by Level (`Is`, `Needs`, `Results`).
-   **Syntax:** `Is key = value`
-   **Location:** `cdd/facts/*.facts`
-   **Purpose:** Serves as the ground truth for agents, preventing assumptions and hallucinations.

## 5. The Sip Principle

A single, minimal, verifiable, and disposable incremental action performed by an AI agent. It is a **development boundary**.

-   **Rules:**
    1.  Implement one capability.
    2.  Write minimal code.
    3.  Provide compile instructions.
    4.  Stop after increment.
-   **Metaphor:** Taking small sips of hot chai to prevent burning (speculative code).
-   **Operational Loop:** `define increment -> agent writes minimal artifact -> system executes -> environment responds -> agent learns fact -> next sip`.

## 6. Three Agent Modes in a Sip

1.  **Understanding Mode:** Read situation, review facts, cards, tools, restrictions.
2.  **Planning Mode:** Determine next minimal increment (which card, tool, expected output).
3.  **Coding Mode:** Perform the actual increment (write test, modify function), then **stop**.

## 7. CHAI Restrictions Engine (CRE)

The structural enforcement layer of CDD, preventing AI agents from generating unverified code through incremental units.

-   **Core Problem Solved:** AI's linear token prediction leading to large blocks of unverified code.
-   **Rules Enforced:** Structural limits, tool restrictions, incremental growth, execution validation.
-   **Disallowed Patterns:** No empty catch blocks, no speculative code, no unused modules.

## 8. CHAI Reasoning Gate System (RGS)

A structured checkpoint mechanism ensuring AI agents validate reasoning before committing to actions.

-   **Purpose:** Prevents directional errors, over-engineering, tool misuse, constraint drift.
-   **Gate Types:** Interpretation, Constraint, Tool Selection, Strategy, Scope, Navigation.
-   **Key:** Evaluates decision quality *during* reasoning, not just outcomes.

## 9. CHAI Environment Rule

Agents must **discover environment context** instead of assuming it.

-   **Disallowed:** Hardcoded file paths, repository structure, output assumptions.
-   **Allowed:** Discover -> Adapt -> Operate (dynamic location of `cdd` directory, `facts` directory).
-   **Philosophy:** Generated code describes an idea; organic code survives in an environment.

## 10. The CHAI Discovery Rule

-   **Formal Definition:** Agents must not assume capabilities; they must discover capabilities through cards. Only observations may produce facts.
-   **Purpose:** Enforces an empirical approach, building knowledge from verified observations.

## 11. Minimal CDD Repository Structure (within `tests/chai/`)

```
tests/chai/
├── README.md              (This document)
├── CDD_Cheat_Sheet.md     (Quick technical reference)
└── cdd/
    ├── card_runner.cpp    (Core CHAI runner)
    ├── chai_checkins.md   (Sip tracking)
    ├── chai_checkouts.md  (Completed work history)
    ├── facts/
    │   ├── environment.facts (Default environment facts)
    │   └── validation.facts  (Test situations)
    └── cards/
        └── SystemClass.cpp (Class file with multiple logical cards)
```

## 12. CHAI CLI Capabilities (Implemented Sips)

-   `chai`: Prints "CHAI CLI".
-   `chai sip`: Prints "SIP RULES".
-   `chai discover cdd`: Dynamically locates and reports `cdd_directory_path`.
-   `chai discover facts`: Dynamically locates and reports `facts_directory_path`.

## 13. Three Critical Recommendations for the Siping Agent

1.  **Always Prefer Reality Over Structure:** Focus on demonstrable, real capabilities.
2.  **Never Simulate the Environment:** Avoid mocks, placeholders, and fake results; observe empirically.
3.  **Protect the Sip Discipline:** Prevent scope creep; one capability, one observable result, zero abstractions per sip.

## 14. Final Thought

CHAI is not about making AI more powerful, but **more disciplined**. It transforms AI from a high-confidence generator into a disciplined engineer operating under strict supervision, producing **organic code that survives in an environment.**

## 15. Six Critical Recommendations for Agents Wearing Gloves

1.  **Treat Every Action as Irreversible:** Assume no git, no rollback, no undo. Reduce the sip if it risks damage. "Measure twice, cut once."
2.  **Prefer Observation Before Mutation:** Always look first (`read -> inspect -> report`) before changing anything (`write -> modify -> assume success`).
3.  **Minimize Write Surface:** Prefer creating new files over modifying existing ones. Touch as little as possible.
4.  **Require Human Verification Points:** Stop at checkpoints (`implement sip`, `show compile command`, `show run command`, `wait`). Never assume execution.
5.  **Prefer Small Executables Over Big Edits:** New, small programs are safer than large integrated code edits. Small programs fail safely.
6.  **Fear Silent Failure:** Every program should produce explicit observations. Silence hides mistakes.

## 16. Monetization and Product Vision

CDD's profitability lies in solving existing business problems by **verifying reality**. The product is not CDD as a philosophy, but the tangible value it provides.

### 1. Environment Verification Service (`chai doctor`)
- **Problem Solved:** Broken CI environments, inconsistent developer machines, dependency breakage.
- **Product:** A tool that runs CDD cards as environment probes (`compiler_operational`, `network_dns_resolution`, etc.) and reports a machine-readable list of facts. It answers "why did this build fail?" by showing what is *actually true* about the environment.
- **Audience:** Developers, DevOps, Platform Engineering.

### 2. AI Agent Safety Layer (`Agent Execution Firewall`)
- **Problem Solved:** Companies want to use AI agents but fear they will break systems.
- **Product:** A "gloves layer" that requires agents to pass verification cards (`filesystem_safe`, `database_backup_verified`) before being allowed to perform mutations.
- **Audience:** Companies deploying code-generating or infrastructure-modifying AI agents.

### 3. Verified Infrastructure Knowledge (`Developer Observability`)
- **Problem Solved:** Infrastructure knowledge is often tribal, undocumented, or outdated.
- **Product:** A system that uses CDD to generate a **live, factual system map** (`postgres_version = 15.3`, `docker_available = true`). It monitors development reality, not just runtime metrics.
- **Audience:** Engineering teams, SREs, architects.

**Core Strategy:** Do not sell CDD. Sell solutions to problems developers already complain about: broken environments, mysterious CI failures, and AI agent safety. CDD is the engine behind the product.
