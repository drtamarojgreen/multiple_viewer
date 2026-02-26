Feature: Brain-Model Core Kernel Determinism

  As a simulation architect
  I want the kernel to exhibit perfectly deterministic behavior
  So that results are reproducible across all environments

  Scenario: Fixed Timestep Progression
    Given a simulation kernel initialized with seed 12345
    When I step the simulation by 100ms
    Then the current time should be exactly 100ms
    And I step the simulation by 50ms
    Then the current time should be exactly 150ms

  Scenario: Snapshot and Restore Integrity
    Given a simulation kernel initialized with seed 98765
    And I run the simulation for 500ms
    And I capture a simulation snapshot
    When I step the simulation for another 200ms
    And I restore the simulation to the captured snapshot
    Then the current time should be exactly 500ms
    And the next 100ms step should produce the same state as the original first step

  Scenario: Seed-Based Reproducibility
    Given two simulation kernels initialized with the same seed 42
    When I step both kernels by 1000ms
    Then both kernels should have identical state hashes
