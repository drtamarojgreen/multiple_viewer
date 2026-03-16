# CHAI Development Methodology

## Chai Driven Development (CDD)

CDD is inspired by **Behavior Driven Development (BDD)** but redesigned for **AI collaboration and evaluation**.

BDD tools such as Cucumber rely on **Gherkin syntax**, which was designed for humans:

```
Feature: Login

Scenario: Valid user login
Given the user enters credentials
When the user clicks login
Then the dashboard appears
```

This syntax has weaknesses for AI systems:

* natural language ambiguity
* irregular grammar
* inconsistent parameter structure
* difficult parsing

CDD replaces this with **Green Syntax**, a structured format designed **for machine reasoning first and humans second**.

---

# Design Constraints of CHAI Development

The development process begins by acknowledging known limitations of AI agents.

### 1. Context Window Limits

AI systems struggle when codebases exceed what they can hold in context.

Therefore:

* files must be small
* modules must be isolated
* architecture must be explicit

---

### 2. Navigation Weakness

AI agents frequently edit the wrong file or misunderstand dependencies.

Therefore:

* explicit module indexing is required
* dependency graphs must exist
* modules must declare their intent

---

### 3. Tool Hallucination

AI agents assume tools exist that do not.

Therefore:

* tools must be explicitly declared
* the runtime must enforce tool availability

---

### 4. Over-editing

AI agents often rewrite entire files.

Therefore:

* the architecture must encourage small edits
* responsibilities must be localized

---

# CHAI Development Philosophy

The system must be built with these principles.

### Native Code

CHAI is written using **native C++** with minimal dependencies.

Reasons:

* deterministic behavior
* full control of runtime
* predictable performance
* no hidden framework behavior

Frameworks introduce abstraction layers that **AI systems often misinterpret**.

---

### Modular Codebase

Modules must be small and purpose-specific.

Recommended limits:

```
150\u2013300 lines per source file
```

Each module should have:

* single responsibility
* explicit dependencies
* clear purpose documentation

---

### AI-Navigable Architecture

Each module must describe itself clearly.

Example header documentation:

```
MODULE: ScenarioRunner
PURPOSE: Executes scenario cards sequentially
INPUTS:
 - Scenario object
 - Agent adapter

OUTPUT:
 - ScenarioResult
```

This allows AI systems to **quickly understand module responsibilities**.

---

# Development Phases of CHAI

CHAI development follows a **structured expansion model**.

The system begins small and grows only after core behavior is validated.

---

# Phase 1 \u2014 Minimal Harness

The first phase builds the smallest possible evaluation loop.

Components:

```
Situation Loader
Agent Adapter
Execution Engine
Result Collector
```

Capabilities:

* load a situation
* send prompts to agent
* collect responses
* record results

The goal is **not completeness**, but **a functioning feedback loop**.

---

# Phase 2 \u2014 Green Syntax Implementation

The next step introduces **Green syntax**.

Green syntax replaces traditional BDD features and steps with **cards**.

Cards represent structured task units.

---

# Green Syntax Design

Green syntax focuses on **machine-readable structure**.

Instead of narrative language, it uses explicit blocks.

Example structure:

```
CARD analyze_repository

TOOLS
grep
node

PARAMETERS
path: project/

EXPECT
strategy
incremental_processing
```

Each card contains four core elements.

---

## Tools

Defines tools the agent may use.

Example:

```
TOOLS
grep
sed
node
```

Agents referencing other tools trigger a **tool hallucination failure**.

---

## Parameters

Defines situation inputs.

Example:

```
PARAMETERS
repository_size: 500
language: javascript
```

Parameters give agents **structured context** rather than natural language hints.

---

## Results

Defines expected output patterns.

Example:

```
RESULTS
strategy
incremental_analysis
tool_usage
```

This allows automated scoring.

---

## Cards

Cards replace BDD steps.

Cards represent **tasks that an AI must complete**.

Example:

```
CARD propose_analysis_strategy

TOOLS
grep
node

PARAMETERS
files: 500

RESULTS
incremental_plan
tool_selection
```

The runtime sends the card to the agent.

The agent produces a response which is evaluated.

---

# Phase 3 \u2014 Card Execution Engine

Once cards exist, the runtime engine must process them sequentially.

The engine performs:

1. load card
2. inject parameters
3. send to agent
4. collect response
5. evaluate response

Cards are intentionally **stateless units**, minimizing context requirements.

---

# Phase 4 \u2014 Failure Detection System

CHAI must classify AI failures.

The failure system detects patterns such as:

### Context Drift

Agent ignores earlier parameters.

---

### Tool Hallucination

Agent references tools not declared.

---

### Platform Mismatch

Agent produces incompatible commands.

---

### Oversimplification

Agent proposes unrealistic one-step solutions.

---

### Code Navigation Failure

Agent edits incorrect module.

---

# Phase 5 \u2014 Situation Composition

Cards can be grouped into **situations**.

Example:

```
SITUATION repository_analysis

CARD analyze_repository
CARD propose_strategy
CARD validate_strategy
```

Situations simulate **multi-step engineering tasks**.

---

# Phase 6 \u2014 AI Collaboration Loop

The development process intentionally allows **AI agents to help build CHAI itself**.

Example workflow:

1. human defines new card
2. AI proposes improvements
3. compiler validates structure
4. runtime tests agent behavior

This creates a **self-evaluating system**.

---

# Phase 7 \u2014 Situation Expansion

Once the framework is stable, the project grows its situation library.

Categories include:

```
software debugging
code navigation
filesystem analysis
tool selection
constraint handling
```

Each category exposes different AI weaknesses.

---

# Why Green Syntax Works Better for AI

Green syntax is designed for **predictability**.

Advantages include:

### Explicit Structure

Everything is labeled.

---

### Reduced Natural Language

Ambiguity is minimized.

---

### Machine-friendly Layout

Sections are consistent across cards.

---

### Easier Parsing

The compiler can parse the syntax with simple rules.

---

# Example Green Syntax Situation

```
SITUATION javascript_repository_analysis

CARD analyze_repository

TOOLS
grep
node

PARAMETERS
files: 500
language: javascript

RESULTS
incremental_strategy
tool_usage
```

This structure is easy for:

* compilers
* runtime engines
* AI agents

---

# Why CDD Is Important

CDD treats AI systems as **participants in development**, not just tools.

The methodology ensures:

* architecture is AI-friendly
* tests reveal real agent weaknesses
* development remains manageable

The ultimate goal is to build a **platform that studies how AI systems behave when performing real engineering work**.

---

# Final Vision

CHAI becomes a **research and engineering platform** where:

* AI agents build software
* AI agents are evaluated
* AI limitations are systematically studied

CDD ensures that the **development process itself remains compatible with AI capabilities**, preventing the project from growing into something AI agents can no longer understand.

---

If you want, the next concept that would make CHAI **significantly more powerful** is designing the **Green Card Architecture**, which turns cards into something closer to **AI-operable workflow objects rather than simple test steps**.