Feature: Layout Stability and Persistence
  Verify that switching between different visualization modes maintains state and correctly updates coordinates.

  Scenario: Switching from Perspective to Nexus Flow
    Given a graph is displayed
    When I switch to "Perspective" mode
    And I switch to "Nexus Flow" mode
    Then the viewport title should contain "Nexus Flow"
    And all nodes should have valid layout positions

  Scenario: Switching to Book View
    Given a graph is displayed
    When I switch to "Book View" mode
    Then the viewport title should contain "Book View"
    And the UI output should contain "-- Subject"

  Scenario: Layout Reset on Node Addition
    Given a graph is displayed
    When I add a node "StabilityNode" at index 500
    Then the graph should indicate layout reset is needed
    And I switch to "Perspective" mode
    Then all nodes should have valid layout positions
    And the graph should indicate layout reset is complete
