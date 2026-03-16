# The Role of `sip` in CHAI

The `sip` command defines how an AI agent interacts with a **Situation**.

It enforces three core principles:

1. **Work only on the current increment**
2. **Stop after producing the increment**
3. **Wait for environmental feedback**

The agent is never allowed to continue autonomously beyond that boundary.

Conceptually:

```text
define situation
↓
agent sips situation
↓
environment responds
↓
agent reflects
↓
next sip
```

Each sip represents **one small constructive action**.

---

# Why the `sip` Concept Matters

AI agents naturally try to **complete the entire problem immediately**.

This happens because the model predicts tokens toward a perceived final solution.

Without guardrails, an agent will:

* design the system
* write multiple modules
* invent tools
* produce commit messages

—all in a single response.

CHAI prevents this by enforcing **incremental development behavior**.

The `sip` command explicitly tells the agent:

> produce only the next increment and stop.

---

# The Three Agent Modes

You identified a crucial requirement: the agent must know **what role it is currently performing**.

CHAI organizes this into three operational modes.

## 1. Understanding Mode

The agent reads and interprets the situation.

Tasks include:

* reviewing facts
* reading cards
* identifying available tools
* confirming restrictions

The agent **does not generate code** in this mode.

It simply builds situational awareness.

---

## 2. Planning Mode

The agent determines the next minimal increment.

Planning defines:

* which card to execute
* which tool will be used
* what output is expected

The plan must remain **small and bounded**.

Planning cannot expand into full system architecture.

---

## 3. Coding Mode

The agent performs the actual increment.

This might involve:

* writing a test class
* modifying a small function
* generating a minimal module

When the increment is complete, the agent **must stop**.

This is the essence of the `sip`.

---

# The Sip Cycle

A CHAI interaction typically follows a repeating cycle.

```text
sip situation
↓
understand
↓
plan increment
↓
produce increment
↓
stop
↓
environment responds
```

The response might include:

* compiler output
* runtime output
* error messages
* test results

These results become **new facts** for the situation.

---

# Why Stopping Is Essential

Stopping after each increment is critical because it forces the agent to incorporate **real-world feedback**.

Without stopping, the agent would:

* assume success
* continue building
* accumulate errors

By stopping, the agent is forced to wait for **actual evidence** from the system.

This aligns development with **observable reality instead of imagined outcomes**.

---

# The Human Role After a Sip

Once the agent completes a sip, the human or environment provides feedback.

This might include:

* compilation results
* runtime behavior
* system logs
* observations

At this moment the user might ask the agent:

> What happened in the environment?

The agent must then interpret the results before the next increment.

---

# Why the “Sip” Metaphor Works

The metaphor is useful because it describes the correct pacing for AI-assisted development.

You cannot drink hot chai quickly.

You must:

* take a small sip
* pause
* experience the effect
* then take another sip

Software development with AI requires the same rhythm.

If the agent tries to **chug the entire problem**, it will burn the project with speculative code.

---

# Preventing Deviation

Deviation is prevented through three mechanisms:

### Situation Constraints

The agent can only operate within defined facts, tools, and cards.

### CRE Restrictions

The Restrictions Engine blocks invalid actions.

### Sip Boundary

The agent must stop after completing the current increment.

Together these create a **controlled development cadence**.

---

# What the Agent Actually Learns

Each sip teaches the agent something real:

* what compiles
* what runs
* what fails
* what tools exist

The agent does not remember these internally.

Instead, the knowledge is captured in the **facts and cards within the repository**.

The repository becomes the **external memory of the development process**.

---

# The Core Philosophy of `sip`

`sip` transforms AI coding from:

```text
generate a full solution
```

into:

```
discover the solution incrementally
```

The agent does not try to solve the entire problem at once.

It simply takes the **next small sip of the situation**, observes what happens, and proceeds from there.

This rhythm is what allows CHAI to turn AI agents from speculative code generators into **careful engineering collaborators**.