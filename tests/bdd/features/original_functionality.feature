Feature: Original Viewer Functionality
  Scenarios covering the core legacy features of the CBT Graph Viewer.

  Scenario: Basic Graph Construction
    Given an empty graph
    When I add a node "Alpha" at index 10
    And I add a node "Beta" at index 20
    And I add an edge between 10 and 20
    Then node 10 should exist
    And node 20 should exist
    And node 10 should be connected to node 20
    And node 20 should be connected to node 10

  Scenario: Focus Management
    Given a graph with node 0
    When I add focus to node 0
    Then node 0 should be focused
    When I remove focus from node 0
    Then node 0 should not be focused
    When I cycle focus
    Then a node should be focused

  Scenario: Graph Persistence
    Given a populated graph with nodes and edges
    When I save the graph to "bdd_persist_test.csv"
    And I clear the current graph
    And I load the graph from "bdd_persist_test.csv"
    Then the graph should restore all previous nodes and edges

  Scenario: Graph Analytics
    Given a triangle graph (nodes 0, 1, 2 all connected)
    When I run full graph analysis
    Then the graph should be reported as "Connected"
    And the average degree should be "2.0"
    And the clustering coefficient should be greater than "0"

  Scenario: Navigation and View
    Given a view context with zoom level "Z1"
    When I zoom in
    Then the zoom level should be "Z2"
    When I pan the view by (5, 5)
    Then the pan offsets should reflect the change

  Scenario: Label Search
    Given a graph with nodes "SearchMe", "FindMe", "SkipMe"
    When I search for nodes containing "Me"
    Then "3" nodes should be identified
    And they should be added to the focus set
