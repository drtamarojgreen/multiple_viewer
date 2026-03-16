# **Chai Agent Handbook**

**Purpose:** To instruct AI collaborators on how to safely, incrementally, and empirically test external repositories using Chai.

---

## **1. Core Philosophy**

*   **Chai is a testing framework, not a sandbox.**
*   Agents work in **incremental steps (\u201csips\u201d)**: compile \u2192 run \u2192 observe \u2192 stop.
*   Success is measured by **real empirical results**, never assumptions or simulated outcomes.
*   **No placeholders or TODOs** in code; unfinished work is tracked in `chai_checkins.md`.
*   Always act as if the environment is **fragile and irrecoverable**\u2014gloves-on mindset.

---

## **2. Key Concepts**

| Concept       | Description                                     | Agent Expectation                                               |
| ------------- | ----------------------------------------------- | --------------------------------------------------------------- |
| **Sip**       | Smallest incremental task.                      | Compile, run, and report results before next action.            |
| **Fact**      | Defines environment conditions or constraints.  | Cards read facts to determine what is executable.               |
| **Card**      | Executable C++ unit implementing a fact.        | Must compile and produce measurable outputs.                    |
| **Runner**    | Executes cards against facts.                   | Prints results, enforces decorators, prevents unsafe execution. |
| **Checkins**  | Records unfinished work.                        | Only `chai_checkins.md`, never in code.                         |
| **Decorator** | Guard that blocks execution if conditions fail. | Ensure cards do not run if environment/facts unmet.             |

---

## **3. File Structure**

All work occurs in **`tests/chai/`**:

```text
tests/chai/
\u2514\u2500\u2500 cdd/
    \u251c\u2500\u2500 cards/             # Implemented cards
    \u251c\u2500\u2500 facts/             # Environment facts
    \u2514\u2500\u2500 chai_checkins.md   # Track unimplemented work
```

**Rules:**

1.  Never write outside `tests/chai/` unless explicitly authorized.
2.  Cards must follow **minimal code principle**.
3.  Runner and CLI commands (`chai discover`, `chai sip`) operate only in scoped directories.

---

## **4. Agent Workflow**

1.  **Discover Facts**

    *   Command: `chai discover facts`
    *   Output: validated list of environmental facts.

2.  **Sip a Card**

    *   Command: `chai sip <card_name>`
    *   Compile \u2192 run \u2192 observe \u2192 stop.
    *   Produce **empirical outputs**, e.g., `filesystem_write_permission = true`.

3.  **Record Checkins**

    *   Track unimplemented or deferred functionality in `chai_checkins.md`.

4.  **Run Card Runner**

    *   Executes card code against facts.
    *   Enforces decorators and outputs results.

5.  **Increment and Repeat**

    *   Each subsequent sip builds **incrementally**.
    *   Never jump ahead or assume previous sips are complete.

---

## **5. Execution Rules for Agents**

1.  **Compile Before Run** \u2013 Cards must compile successfully before any observation.
2.  **Observe Results** \u2013 Only recorded empirical results are valid.
3.  **No Hardcoding** \u2013 Paths, environment values, or config must reference facts or relative paths.
4.  **No Placeholders** \u2013 Track unfinished items in `chai_checkins.md`.
5.  **Respect Directory Scope** \u2013 Work only in allowed folders.
6.  **Fail Fast, Learn Fast** \u2013 Any deviation stops the current sip; analyze before retrying.

---

## **6. Common Pitfalls and How Chai Prevents Them**

| Pitfall               | Before Chai                                   | After Chai                                                   |
| --------------------- | --------------------------------------------- | ------------------------------------------------------------ |
| Overengineering       | Agents generate hundreds of unnecessary lines | Sips force incremental coding, only one capability at a time |
| Silent Failures       | Agents assert `true==true`                    | Cards return measurable empirical results                    |
| Hardcoded paths       | `/home/user/...`                              | Facts + relative paths enforce environment flexibility       |
| Placeholder abuse     | `TODO` comments left in code                  | `chai_checkins.md` tracks deferred work                      |
| Ignoring environment  | Agents assume compiler, libraries exist       | Facts define environment; runner validates before execution  |
| Skipping verification | Agents move on after compilation              | Runner executes cards, prints results, enforces decorators   |

---

## **7. Example Card and Fact**

**Fact (`tests/chai/cdd/facts/filesystem.facts`):**

```text
filesystem_write_permission = true
working_directory_exists = true
```

**Card (`tests/chai/cdd/cards/filesystem_create_file.cpp`):**

```cpp
#include <fstream>
#include <iostream>

int main() {
    std::ofstream file("chai_created_file.tmp");
    if(file) std::cout << "filesystem_create_file_operational = true
";
    return 0;
}
```

**Execution Output via Runner:**

```text
Loading facts from tests/chai/cdd/facts...
Loading card filesystem_create_file.cpp...
Decorators allow execution.
Executing card...
filesystem_create_file_operational = true
Card execution completed.
```

---

## **8. Best Practices**

1.  **Incremental Development:** Never write large blocks of code at once.
2.  **Empirical Reporting:** Every output must be observable and verifiable.
3.  **Environment Awareness:** Facts define real-world conditions.
4.  **No Shortcuts:** Avoid placeholders, TODOs, or assumptions.
5.  **Track Deferred Work:** Use `chai_checkins.md` for items not implemented yet.
6.  **Respect Sips:** Stop after each increment; do not continue until next instruction.

---

## **9. Mission Statement for Agents**

> **\u201cEvery card executed is a lesson in reality. Follow the facts, respect the sips, and never assume success.\u201d**

Chai transforms AI from **code-generating text machines** into **empirical, responsible collaborators** capable of reliably testing external repositories.
