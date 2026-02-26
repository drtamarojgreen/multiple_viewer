Feature: Neurochemical Domain Simulation Parity

  As a neurochemist
  I want Dopamine and Serotonin systems to model receptor kinetics
  So that motivational and emotional stability are correctly simulated

  Scenario: Dopamine Receptor Binding Kinetics
    Given an integrated brain model with the Dopamine domain
    When the simulation kernel is running
    Then a "Clinical" overlay with ID "dopamine_receptor_binding" should be active
    And it should describe dynamic flux equilibrium

  Scenario: Serotonin Emotional Stability Index
    Given an integrated brain model with the Serotonin domain
    When the simulation progresses
    Then an overlay with ID "serotonin_emotional_stability" should be active
    And its text should contain a resilience index value
