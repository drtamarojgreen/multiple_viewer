Feature: Stress Domain Simulation Parity

  As a clinician
  I want the Stress domain to model allostatic load and recovery
  So that stress-related health trajectories are accurately simulated

  Scenario: HPA-Axis Load Monitoring
    Given an integrated brain model with the Stress domain
    When the simulation progresses under moderate load
    Then an overlay with ID "stress_hpa_load" should be visible
    And the role should be "Clinical"
    And the text should contain "Allostatic Load Accumulation"

  Scenario: Resilience Reserve Recovery
    Given an integrated brain model with the Stress domain
    When a restorative sleep simulation step is executed
    Then an overlay with ID "stress_resilience_reserve" should be active
    And the text should describe replenishment of the resilience buffer
