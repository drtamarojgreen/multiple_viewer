# SDD Cheat Sheet: **C**ards, **F**acts, **R**unner \u2014 Your AI Coding Guide

## **What is SDD?**

**Sorrel Driven Development**: An AI-aware coding methodology for **reliable, incremental, and empirically verified** software construction. It turns chaotic AI code generation into a disciplined workflow.

---

## **The Sip Principle \u2014 Your Core Loop**

**Act small, verify often.** Every action is a "sip" \u2014 a minimal, verifiable, disposable increment.

1.  **Define Sip:** What *one capability* are you proving?
2.  **Code Sip:** Write *minimal C++ code* (no placeholders, no frameworks).
3.  **Execute Sip:** Run the code. *(Human/System)*
4.  **Observe Results:** Note empirical outputs (e.g., `key = value`).
5.  **Record Fact:** Update `tests/sorrel/sdd/facts/environment.facts` (append-only).
6.  **Repeat:** Take the next sip.

---

## **Key SDD Artifacts**

*   **Class (C++ file in `tests/sorrel/sdd/cards/`):**
    *   A single source file grouped by domain (e.g., `SystemClass.cpp`).
    *   Contains one or more **Logical Cards**.
    *   Cards are defined by comments: `// @Card: name`.
    *   Cards are executed via `card_runner` passing the name as `argv[1]`.

*   **Logical Card (Unit of Work):**
    *   Defined within a Class file.
    *   Uses **Decorators** for prerequisites and validation.
    *   *Decorators:* `// @Is key == val`, `// @Needs key == val`, `// @Results key == val`, `// @Situation name`.

*   **Fact (Line in `tests/sorrel/sdd/facts/*.facts`):**
    *   Structured by **Situation**, **Level**, and **Fact**.
    *   *Syntax:* `[Level] [key] = [value]`.
    *   *Levels:* `Is` (System State), `Needs` (Prerequisite), `Results` (Observation).
    *   *Situations:* Grouped by `Situation: Name` headers.

*   **Runner (`tests/sorrel/sdd/card_runner.cpp`):**
    *   Discovers Classes and Logical Cards.
    *   Indexes Facts by Situation and Level.
    *   Evaluates decorators before/after execution.
    *   Reports truth based on empirical observation.

*   **`sorrel_checkins.md`:**
    *   Located in `tests/sorrel/sdd/`.
    *   Tracks incremental **Sips** (Done, Error, Pending).

---

## **SORREL CLI \u2014 Your Tools**

*   `sorrel`: Main CLI entrypoint (`SORREL CLI`).
*   `sorrel sip`: Displays the `SIP RULES`.
*   `sorrel discover sdd`: Finds the `sdd` directory.
*   `sorrel discover facts`: Finds the `facts` directory.

---

## **Guiding Principles \u2014 Wear Your Gloves!**

*   **Reality > Structure:** Prefer real capability over abstract architecture.
*   **Observe > Mutate:** Look first, then act.
*   **Minimize Write Surface:** Create new files; avoid modifying existing ones.
*   **Human Verify:** Stop for human checks (compile, run, observe).
*   **Small Executables:** Favor small, testable programs.
*   **Fear Silent Failure:** Explicitly observe everything.

---

## **Why SDD?**

Turns AI into a disciplined, real-world-aware collaborator. Prevents:
*   Over-engineering
*   Broken builds
*   Tool hallucination
*   Unverified code
*   Chaotic development

**SDD: The AI coding safety net for real-world environments.**
