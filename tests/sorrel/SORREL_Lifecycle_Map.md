# SORREL Lifecycle Map: Disciplined AI-Assisted Development

This map illustrates the interaction between Developers (Humans), AI Agents, and the SORREL System components throughout the development lifecycle, guided by the Sip Principle.

---

```mermaid
graph TD
    subgraph Human Developer
        HD[Define Vision & High-Level Requirements]
    end

    subgraph AI Agent
        AI_U[1. Understanding Mode: Read Situation, Facts, Rules]
        AI_P[2. Planning Mode: Propose Next Minimal Sip]
        AI_C[3. Coding Mode: Generate Minimal Artifact (Code)]
    end

    subgraph SORREL System
        SORREL_RGS{Reasoning Gate System: Validate AI Plan?}
        SORREL_CRE{Restrictions Engine: Enforce Sip Constraints?}
        SORREL_ENV(Environment: Compiler, Filesystem, OS)
        SORREL_RUN[Card Runner: Execute Card/Code]
        SORREL_OBS[Observe Results: Capture Empirical Data]
        SORREL_REC[Record Fact: Append to environment.facts]
        SORREL_CHECK[Update sorrel_checkins.md (Future Work)]
    end

    subgraph Artifacts
        SIT[Situation: Context, Tools, Objectives]
        FACTS[Facts: Verified Environment Knowledge]
        CARDS[Card: Executable C++ Unit]
        RESULTS[Card Results: Empirical Observations]
        CHECKINS[Checkins: Unimplemented Functions/Files]
    end

    %% Flow from Developer to AI
    HD --> SIT

    %% AI Loop
    SIT --> AI_U
    AI_U --> AI_P
    AI_P --> SORREL_RGS
    SORREL_RGS -- Plan Validated --> SORREL_CRE
    SORREL_RGS -- Plan Rejected --> AI_P
    SORREL_CRE -- Constraints Met --> AI_C
    SORREL_CRE -- Constraints Violated --> AI_P
    AI_C --> CARDS

    %% System Execution & Feedback Loop
    CARDS --> SORREL_RUN
    SORREL_RUN --> SORREL_ENV
    SORREL_ENV --> SORREL_OBS
    SORREL_OBS --> RESULTS
    RESULTS --> SORREL_REC
    SORREL_REC --> FACTS
    CARDS --> SORREL_CHECK
    SORREL_CHECK --> CHECKINS

    %% AI Learning and Next Sip
    RESULTS --> AI_U

    %% Human Oversight / Interaction Points
    HD -.-> SORREL_RGS
    HD -.-> SORREL_CRE
    HD -.-> SORREL_ENV
    HD -.-> SORREL_OBS
    HD -.-> SORREL_REC
    HD -.-> SORREL_CHECK

    %% Labels for clarity
    classDef Human fill:#b2e0e0,stroke:#333,stroke-width:2px;
    classDef AI fill:#c2e0b2,stroke:#333,stroke-width:2px;
    classDef System fill:#e0b2b2,stroke:#333,stroke-width:2px;
    classDef Artifact fill:#e0e0e0,stroke:#333,stroke-width:1px,border-radius:5px;

    class HD Human;
    class AI_U,AI_P,AI_C AI;
    class SORREL_RGS,SORREL_CRE,SORREL_ENV,SORREL_RUN,SORREL_OBS,SORREL_REC,SORREL_CHECK System;
    class SIT,FACTS,CARDS,RESULTS,CHECKINS Artifact;
```

---

**Explanation of Flow:**

1.  **Human Developer (HD):** Initiates by defining the high-level vision and requirements for the software. This is captured in a **Situation (SIT)** artifact.
2.  **AI Agent - Understanding Mode (AI_U):** The AI agent begins by reading the current **Situation (SIT)**, existing **Facts (FACTS)**, and all governing **Rules**. It uses this to build situational awareness.
3.  **AI Agent - Planning Mode (AI_P):** Based on its understanding, the AI proposes the next *minimal sip* \u2014 the smallest, most impactful increment of work.
4.  **SORREL Reasoning Gate System (SORREL_RGS):** This acts as a checkpoint. It validates the AI's proposed plan against the Situation's constraints and the SORREL Discovery Rule.
    *   If the plan is **rejected** (e.g., violates a constraint, too broad), the AI agent returns to **Planning Mode (AI_P)** to revise its approach.
    *   If the plan is **validated**, it proceeds to the Restrictions Engine.
    *   *(Human Developer can optionally intervene here for complex rejections)*
5.  **SORREL Restrictions Engine (SORREL_CRE):** Enforces strict sip constraints (e.g., one capability, no placeholders, no hardcoded paths).
    *   If constraints are **violated**, the AI agent returns to **Planning Mode (AI_P)**.
    *   If constraints are **met**, the AI proceeds to **Coding Mode**.
6.  **AI Agent - Coding Mode (AI_C):** The AI generates the minimal C++ artifact (e.g., a Card's source code) for the approved sip. This new code forms a **Card (CARDS)**.
7.  **SORREL Card Runner (SORREL_RUN):** The system's runner takes the generated **Card (CARDS)** and executes it against the **Environment (SORREL_ENV)**.
8.  **Observe Results (SORREL_OBS):** The SORREL system observes and captures the empirical output of the Card's execution, forming **Card Results (RESULTS)**.
9.  **Record Fact (SORREL_REC):** If the **Card Results (RESULTS)** verify a new piece of knowledge, the SORREL system records this as an immutable entry in **Facts (FACTS)**.
10. **Update Checkins (SORREL_CHECK):** For any conceptual or unimplemented parts of the work, the `sorrel_checkins.md` (**CHECKINS**) file is updated.
11. **AI Agent - Understanding Mode (AI_U):** The AI agent consumes the newly recorded **Facts (FACTS)** and **Card Results (RESULTS)**, updating its understanding of the environment and state. This completes one sip cycle, leading to the next iteration of planning.

**Human Developer Oversight (dashed lines):** The human continuously monitors and can intervene at various points, especially during Reasoning Gate and Restrictions Engine checks, or by reviewing Observations and Facts. This ensures the AI remains aligned with the overall vision without micromanagement.
