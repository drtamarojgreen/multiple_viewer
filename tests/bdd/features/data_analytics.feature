Feature: Data I/O and Analytics
  Scenarios covering enhancements 31-50.

  Scenario: Multi-format loading
    Given a file "graph_data.json" in JSON format
    When I load the graph from "graph_data.json"
    Then the graph should contain "50" nodes and "120" edges

  Scenario: SVG Export
    Given a populated graph
    When I export the graph as SVG to "output.svg"
    Then the file "output.svg" should be created and contain "<svg" tags

  Scenario: Community Detection
    Given a graph with three distinct clusters
    When I run community detection
    Then it should identify "3" communities

  Scenario: Centrality calculation
    Given a star-topology graph
    When I compute betweenness centrality
    Then the center node should have the "HIGHEST" centrality score
