# CHAI Lifecycle Map: Disciplined AI-Assisted Development

This map illustrates the interaction between Developers (Humans), AI Agents, and the CHAI System components throughout the development lifecycle, guided by the Sip Principle.

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

    subgraph CHAI System
        CHAI_RGS{Reasoning Gate System: Validate AI Plan?}
        CHAI_CRE{Restrictions Engine: Enforce Sip Constraints?}
        CHAI_ENV(Environment: Compiler, Filesystem, OS)
        CHAI_RUN[Card Runner: Execute Card/Code]
        CHAI_OBS[Observe Results: Capture Empirical Data]
        CHAI_REC[Record Fact: Append to environment.facts]
        CHAI_CHECK[Update chai_checkins.md (Future Work)]
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
    AI_P --> CHAI_RGS
    CHAI_RGS -- Plan Validated --> CHAI_CRE
    CHAI_RGS -- Plan Rejected --> AI_P
    CHAI_CRE -- Constraints Met --> AI_C
    CHAI_CRE -- Constraints Violated --> AI_P
    AI_C --> CARDS

    %% System Execution & Feedback Loop
    CARDS --> CHAI_RUN
    CHAI_RUN --> CHAI_ENV
    CHAI_ENV --> CHAI_OBS
    CHAI_OBS --> RESULTS
    RESULTS --> CHAI_REC
    CHAI_REC --> FACTS
    CARDS --> CHAI_CHECK
    CHAI_CHECK --> CHECKINS

    %% AI Learning and Next Sip
    RESULTS --> AI_U

    %% Human Oversight / Interaction Points
    HD -.-> CHAI_RGS
    HD -.-> CHAI_CRE
    HD -.-> CHAI_ENV
    HD -.-> CHAI_OBS
    HD -.-> CHAI_REC
    HD -.-> CHAI_CHECK

    %% Labels for clarity
    classDef Human fill:#b2e0e0,stroke:#333,stroke-width:2px;
    classDef AI fill:#c2e0b2,stroke:#333,stroke-width:2px;
    classDef System fill:#e0b2b2,stroke:#333,stroke-width:2px;
    classDef Artifact fill:#e0e0e0,stroke:#333,stroke-width:1px,border-radius:5px;

    class HD Human;
    class AI_U,AI_P,AI_C AI;
    class CHAI_RGS,CHAI_CRE,CHAI_ENV,CHAI_RUN,CHAI_OBS,CHAI_REC,CHAI_CHECK System;
    class SIT,FACTS,CARDS,RESULTS,CHECKINS Artifact;
```

---

**Explanation of Flow:**

1.  **Human Developer (HD):** Initiates by defining the high-level vision and requirements for the software. This is captured in a **Situation (SIT)** artifact.
2.  **AI Agent - Understanding Mode (AI_U):** The AI agent begins by reading the current **Situation (SIT)**, existing **Facts (FACTS)**, and all governing **Rules**. It uses this to build situational awareness.
3.  **AI Agent - Planning Mode (AI_P):** Based on its understanding, the AI proposes the next *minimal sip* \u2014 the smallest, most impactful increment of work.
4.  **CHAI Reasoning Gate System (CHAI_RGS):** This acts as a checkpoint. It validates the AI's proposed plan against the Situation's constraints and the CHAI Discovery Rule.
    *   If the plan is **rejected** (e.g., violates a constraint, too broad), the AI agent returns to **Planning Mode (AI_P)** to revise its approach.
    *   If the plan is **validated**, it proceeds to the Restrictions Engine.
    *   *(Human Developer can optionally intervene here for complex rejections)*
5.  **CHAI Restrictions Engine (CHAI_CRE):** Enforces strict sip constraints (e.g., one capability, no placeholders, no hardcoded paths).
    *   If constraints are **violated**, the AI agent returns to **Planning Mode (AI_P)**.
    *   If constraints are **met**, the AI proceeds to **Coding Mode**.
6.  **AI Agent - Coding Mode (AI_C):** The AI generates the minimal C++ artifact (e.g., a Card's source code) for the approved sip. This new code forms a **Card (CARDS)**.
7.  **CHAI Card Runner (CHAI_RUN):** The system's runner takes the generated **Card (CARDS)** and executes it against the **Environment (CHAI_ENV)**.
8.  **Observe Results (CHAI_OBS):** The CHAI system observes and captures the empirical output of the Card's execution, forming **Card Results (RESULTS)**.
9.  **Record Fact (CHAI_REC):** If the **Card Results (RESULTS)** verify a new piece of knowledge, the CHAI system records this as an immutable entry in **Facts (FACTS)**.
10. **Update Checkins (CHAI_CHECK):** For any conceptual or unimplemented parts of the work, the `chai_checkins.md` (**CHECKINS**) file is updated.
11. **AI Agent - Understanding Mode (AI_U):** The AI agent consumes the newly recorded **Facts (FACTS)** and **Card Results (RESULTS)**, updating its understanding of the environment and state. This completes one sip cycle, leading to the next iteration of planning.

**Human Developer Oversight (dashed lines):** The human continuously monitors and can intervene at various points, especially during Reasoning Gate and Restrictions Engine checks, or by reviewing Observations and Facts. This ensures the AI remains aligned with the overall vision without micromanagement.
