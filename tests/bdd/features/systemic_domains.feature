Feature: Systemic Domain Simulation Parity

  As an epidemiologist
  I want Environmental and Inflammation domains to model systemic load
  So that external and immune factors are correctly integrated

  Scenario: Environmental Stressor Exposure
    Given an integrated brain model with the Environment domain
    When external pressure is simulated
    Then an overlay with ID "env_stressor_exposure" should indicate load level

  Scenario: Neuroinflammatory Cytokine Tone
    Given an integrated brain model with the Inflammation domain
    When the simulation progress continues
    Then an overlay with ID "inflammation_cytokine_tone" should be active
    And it should describe the current inflammatory equilibrium
