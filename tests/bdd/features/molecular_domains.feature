Feature: Molecular Domain simulation Parity

  As a biologist
  I want DNA and RNA domains to simulate repair fidelity
  So that micro-level biological resilience is correctly modeled

  Scenario: DNA Base Excision Repair (BER)
    Given an integrated brain model with the DNA domain
    When the simulation kernel steps
    Then an overlay with ID "dna_repair_activity" should be active
    And the text should confirm BER maintenance integrity

  Scenario: RNA Transcript Fidelity via AlkB
    Given an integrated brain model with the RNA domain
    When transcription repair protocols are simulated
    Then an overlay with ID "rna_transcript_fidelity" should be active
    And the text should contain "AlkB Repair Protocol"
