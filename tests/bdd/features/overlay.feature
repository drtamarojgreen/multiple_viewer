Feature: Overlay System
  Scenario: Toggling overlays and matching nodes
    Given a graph with node 0 and label "Alpha"
    And an external overlay graph with node 5 and label "Alpha"
    When I enable overlays
    Then node 0 should be marked as an overlay node
    And the console should show "1" overlay match
