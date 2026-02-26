Feature: Emotion Domain Simulation Parity

  As a psychologist
  I want the Emotion domain to simulate limbic dynamics
  So that affect-based responses are correctly modeled

  Scenario: Papez Circuit Status Update
    Given an integrated brain model with the Emotion domain
    When the simulation kernel steps forward
    Then an overlay for the "limbic_system" anchor should be active
    And the text should describe "Papez Circuit"

  Scenario: Amygdala Threat Detection
    Given an integrated brain model with the Emotion domain
    When a threat stimulus event is published
    Then the Amygdala system should emit high-priority "Status" text
    And its priority should be greater than or equal to 12
