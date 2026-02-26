Feature: Neuro Domain Simulation Parity

  As a neuroscientist
  I want the Neuro domain to simulate network connectivity and plasticity
  So that brain-wide activity patterns are correctly represented

  Scenario: Network Connectivity Status
    Given an integrated brain model with the Neuro domain
    When the simulation kernel is active
    Then an overlay with ID "neuro_network_dynamics" should be published
    And the text should signify active distributed processing

  Scenario: Synaptic Plasticity Trigger
    Given an integrated brain model with the Neuro domain
    When high-frequency signaling is simulated
    Then a "Telemetry" overlay with ID "neuro_plasticity_event" should be active
    And the text should describe synaptic weighting adjustments
