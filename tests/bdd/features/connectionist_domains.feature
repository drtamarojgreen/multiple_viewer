Feature: Connectionist Domain Simulation Parity

  As a behavioral scientist
  I want Pathway and Synapse domains to model connection weights
  So that habit formation and signal transmission are correctly represented

  Scenario: Pathway Habit Reinforcement
    Given an integrated brain model with the Pathway domain
    When the simulation steps through repeated signaling
    Then an overlay with ID "pathway_habit_loop" should be active
    And the text should signify reinforcement strength

  Scenario: Synaptic Signal Weighting
    Given an integrated brain model with the Synapse domain
    When a synaptic transmission event occurs
    Then an overlay with ID "synapse_signal_weighting" should describe LTP activation
