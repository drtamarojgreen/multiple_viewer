Feature: Brain Model API and Registry
  Scenario: Registering and retrieving a structural brain model
    Given a structural brain model manifest for "mni_v1"
    And a structural brain model with 10 regions
    When I register the model in the BrainModelRegistry
    Then I should be able to retrieve the model "mni_v1" from the registry
    And the model family should be "STRUCTURAL"

  Scenario: Temporal playback interpolation
    Given a temporal engine with frames at 0ms and 100ms
    And the data at 0ms is (0, 0, 0)
    And the data at 100ms is (10, 20, 30)
    When I set the timeline position to 50ms
    Then the interpolated data should be approximately (5, 10, 15)

  Scenario: Layer visibility management
    Given a render layer "cortex_surface" of type "SURFACE"
    When I add the layer to the RenderLayerManager
    And I set the visibility of "cortex_surface" to false
    Then the layer "cortex_surface" should not be visible
