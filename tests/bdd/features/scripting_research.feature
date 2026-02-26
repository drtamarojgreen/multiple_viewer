Feature: Scripting and research
  Scenarios covering enhancements 51-70.

  Scenario: Lua Script Execution
    Given the scripting runtime is initialized
    When I execute the Lua script "graph:addNode('LuaNode', 100)"
    Then a node named "LuaNode" with index 100 should exist

  Scenario: Plugin Lifecycle
    Given a valid plugin "custom_layout.so"
    When I load the plugin
    Then "onInit" should be called for the plugin
    And the plugin should be active in the manager

  Scenario: Temporal Analysis
    Given the graph state at timestamp 1000
    When I capture a snapshot
    And I modify the graph at timestamp 1100
    Then I should be able to retrieve the original state from timestamp 1000

  Scenario: Hypothesis Layers
    Given a hypothesis annotation "Connections are dense here"
    When I attach it to a cluster
    Then the cluster should store the annotation metadata
