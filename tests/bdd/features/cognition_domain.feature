Feature: Cognition Domain Simulation Parity

  As a researcher
  I want the Cognition domain to accurately reflect Greenhouse models
  So that the text-based integrated output is scientifically valid

  Scenario: Executive Control Pulse Trigger
    Given an integrated brain model with the Cognition domain
    When the simulation kernel is running
    And 100ms have elapsed
    Then an overlay with ID "cognition_exec_control" should be active
    And the overlay text should contain "Executive Control: Active Pulse"

  Scenario: PFC-Parietal Loop Activation
    Given an integrated brain model with the Cognition domain
    When the simulation reaches state where PFC and Parietal regions interact
    Then an overlay with ID "cognition_pfc_parietal" should be published
    And the role should be "Status"
