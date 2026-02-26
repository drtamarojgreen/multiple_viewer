Feature: Brain Model Mapping
  Scenario: Mapping a graph node to a brain region
    Given a graph with a node "Node1" at index 0
    And a brain model with region "R1" named "Thalamus" at (10, 20, 30) with radius 5.0
    When I apply a brain overlay mapping node 0 to region "R1"
    Then node 0 should be associated with region "R1"
