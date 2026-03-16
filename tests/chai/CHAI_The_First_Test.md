# The First CHAI Test

## Purpose

The first CHAI test answers a few fundamental questions about the remote environment:

1. Can the program compile?
2. Can it execute?
3. Can it receive arguments?
4. Can it produce output?

These are the **first observable facts** an agent can learn.

Instead of writing a program, the agent writes a **probe**.

---

# CHAI Test Pseudocode

Your pseudocode captures the core idea:

```text
main(argument)
{
    out(argument)
}
```

Conceptually the test does only one thing:

* accept an argument
* output the argument

This verifies **input → output capability**.

---

# Example C++ Test Implementation

A minimal version might look like:

```cpp
#include <iostream>

int main(int argc, char** argv)
{
    if (argc > 1)
        std::cout << argv[1] << std::endl;

    return 0;
}
```

This program checks:

* command-line argument support
* standard output
* program execution

Nothing else.

---

# Why This Is a Good First Test

This test reveals several environmental facts.

After compilation and execution, the agent learns:

**Compilation facts**

* compiler exists
* C++ builds successfully
* standard library available

**Execution facts**

* program runs
* arguments are passed correctly
* stdout is functional

These become the **first entries in `cdd/facts/`**.

---

# Example Facts Produced

Example conceptual facts derived from the test:

```
compiler=c++
execution=enabled
stdout=available
argv=supported
```

These facts are not assumptions.

They are **observations from execution**.

---

# Corresponding CHAI Card

The first card might look conceptually like this.

```
CARD: environment-io-test

TOOLS
compiler
stdout

OBJECTIVE
verify program execution and argument output

RESULT
argument printed to stdout
```

If the test compiles and runs, the card succeeds.

---

# Why CHAI Starts This Small

Many AI coding failures happen because the agent begins with something like:

```
write a server
write a framework
write a parser
```

Those tasks assume dozens of environmental properties.

CHAI instead begins with something that assumes **almost nothing**.

```
receive input
print output
```

If that works, the system can grow.

---

# The Next Likely CHAI Tests

After this first probe, the next tests would typically explore:

### File system

```
create file
write file
read file
```

### Compiler capabilities

```
check language standard
test library headers
```

### Platform features

```
threads
network sockets
timers
```

Each test expands the **environment map** the agent operates within.

---

# Why This Matters

This first CHAI test is important because it establishes the rule:

**systems grow from verified capabilities, not imagined ones.**

Instead of generating large speculative programs, the agent learns the environment step by step and builds **only on things that have been proven to work**.

That small `main(argument) → out(argument)` probe is effectively the **first handshake between the AI and the real system**.