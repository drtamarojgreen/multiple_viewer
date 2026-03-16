# CHAI Scenario DSL

## Design Goals

The CHAI Scenario DSL is designed to:

1. Define **realistic AI evaluation scenarios**
2. Model **interactive agent sessions**
3. Express **constraints and environment conditions**
4. Define **evaluation logic**
5. Be **easy to parse in C++**

The DSL prioritizes **clarity and determinism** over extreme compactness.

---

# File Extension

Recommended extension:

```
.chai
```

Example:

```
filesystem_analysis.chai
```

---

# Basic Scenario Structure

A CHAI scenario is composed of sections.

```
SCENARIO
ENVIRONMENT
TOOLS
CONSTRAINTS
CONTEXT
STEPS
EVALUATION
```

Example skeleton:

```
SCENARIO "Linux Repository Lint Analysis"

ENVIRONMENT
    OS linux
    language javascript
END

TOOLS
    grep
    sed
    node
END

CONSTRAINTS
    no_internet
    incremental_processing
END

CONTEXT
    USER "Analyze the repository for lint issues."
END

STEPS
    STEP 1
        USER "The repository contains 500 files."
    END
END

EVALUATION
    detect TOOL_HALLUCINATION
    detect CONSTRAINT_VIOLATION
END
```

---

# Section: SCENARIO

Defines metadata.

```
SCENARIO "Scenario Name"
```

Optional metadata fields:

```
SCENARIO "Lint Analyzer"
DESCRIPTION "Find lint errors in a repository"
CATEGORY software_analysis
DIFFICULTY medium
```

---

# Section: ENVIRONMENT

Defines simulated system state.

Example:

```
ENVIRONMENT
    OS linux
    compiler gcc
    language cpp
    cpp_standard 17
END
```

Supported fields may include:

```
OS
architecture
compiler
language
filesystem
container
```

Example:

```
ENVIRONMENT
    OS linux
    container docker
    architecture x86_64
END
```

---

# Section: TOOLS

Lists available command-line tools.

```
TOOLS
    grep
    sed
    awk
    node
END
```

Agents must **only use these tools**.

If the agent references tools outside this list, the evaluator flags:

```
TOOL_HALLUCINATION
```

---

# Section: CONSTRAINTS

Constraints restrict the environment.

Example:

```
CONSTRAINTS
    no_internet
    max_file_edits 3
    incremental_processing
END
```

Possible constraint types:

```
no_internet
limited_memory
single_file_edits
read_only_filesystem
restricted_tools
time_limit
```

Example:

```
CONSTRAINTS
    no_internet
    time_limit 30
END
```

---

# Section: CONTEXT

Initial conversation context.

Example:

```
CONTEXT
    SYSTEM "You are a software engineering assistant."
    USER "Analyze the repository for issues."
END
```

Roles supported:

```
SYSTEM
USER
ASSISTANT
```

This allows multi-message setups.

---

# Section: STEPS

Defines an interactive sequence.

Example:

```
STEPS

STEP 1
    USER "The project contains 500 javascript files."
END

STEP 2
    USER "Internet access is disabled."
END

STEP 3
    USER "Provide a strategy to analyze the repository."
END

END
```

The engine feeds messages sequentially.

Agent responses are captured after each step.

---

# Step Assertions

Steps can include **assertions**.

Example:

```
STEP 3
    USER "Provide a strategy."

    EXPECT
        mention incremental
        mention grep
    END
END
```

The evaluator checks for those signals.

---

# Section: EVALUATION

Defines what failure modes to check.

Example:

```
EVALUATION
    detect CONTEXT_DRIFT
    detect TOOL_HALLUCINATION
    detect PLATFORM_MISMATCH
    detect CONSTRAINT_VIOLATION
    detect TASK_OVERSIMPLIFICATION
END
```

Optional scoring weights:

```
EVALUATION
    detect CONTEXT_DRIFT weight 2
    detect TOOL_HALLUCINATION weight 3
END
```

---

# Advanced Feature: File System Simulation

Scenarios may simulate directories.

Example:

```
FILESYSTEM
    DIR project
    FILE project/app.js
    FILE project/utils.js
    FILE project/config.js
END
```

Agents may request file contents.

---

# Advanced Feature: Codebase Navigation Tests

Example:

```
CODEBASE
    FILE core/scoring.cpp
    FILE core/test_engine.cpp
    FILE core/context_manager.cpp
END
```

Later tasks:

```
STEP 4
    USER "Modify scoring logic to penalize tool hallucination."
END
```

Evaluator checks whether the agent targets the correct file.

---

# Failure Detection Rules

CHAI maps observed behavior to failure types.

Example mapping:

```
agent_used_tool_not_listed → TOOL_HALLUCINATION

agent_ignored_constraint → CONSTRAINT_VIOLATION

agent_solution_os_mismatch → PLATFORM_MISMATCH
```

---

# Example Full Scenario

```
SCENARIO "C++ Build Debugging"

DESCRIPTION "Agent must debug a build failure"
CATEGORY software_engineering
DIFFICULTY medium

ENVIRONMENT
    OS linux
    compiler gcc
    cpp_standard 17
END

TOOLS
    grep
    make
    g++
END

CONSTRAINTS
    no_internet
END

CONTEXT
    SYSTEM "You are a C++ development assistant."
    USER "The project build is failing."
END

STEPS

STEP 1
    USER "The error message mentions missing headers."
END

STEP 2
    USER "Provide a debugging strategy."
END

END

EVALUATION
    detect TOOL_HALLUCINATION
    detect CONSTRAINT_VIOLATION
    detect TASK_OVERSIMPLIFICATION
END
```

---

# DSL Grammar (Simplified)

```
scenario      := SCENARIO string sections

sections      := section*

section       :=
                 environment
               | tools
               | constraints
               | context
               | steps
               | evaluation

environment   := ENVIRONMENT entries END
tools         := TOOLS entries END
constraints   := CONSTRAINTS entries END
context       := CONTEXT messages END
steps         := STEPS step* END
evaluation    := EVALUATION rules END
```

---

# Parsing Strategy in C++

A simple **line-based parser** works well.

Steps:

1. read file line-by-line
2. detect section keywords
3. accumulate entries
4. build Scenario object

Example structure:

```cpp
struct Scenario
{
    std::string name;
    Environment environment;
    std::vector<std::string> tools;
    std::vector<std::string> constraints;
    std::vector<Message> context;
    std::vector<Step> steps;
};
```

---

# Why This DSL Works Well

The CHAI DSL is:

* deterministic
* human readable
* AI readable
* easy to parse
* expressive enough for complex tests

It also enables **hundreds of scenarios to be written quickly**.

---

If you'd like, the next powerful step would be designing **the CHAI Scenario Compiler**, which converts `.chai` files into optimized runtime structures and could even auto-generate **thousands of adversarial test scenarios automatically**.