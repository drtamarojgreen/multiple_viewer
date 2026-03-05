Feature: Core Rendering and UI/UX
  Scenarios covering enhancements 1-30.

  Scenario: Smooth panning and zooming
    Given a graph is displayed
    When I pan the view by (10, -5)
    And I zoom in to "Z5"
    Then the viewport center should shift by (10, -5)
    And the node size should increase to zoom level 5

  Scenario: Minimap orientation
    Given a large graph is loaded
    When the graph is rendered
    Then a minimap should be visible in the corner
    And the minimap should show the current viewport "FOCUS" area

  Scenario: Customizable shortcuts
    Given a shortcut manager is active
    When I register 'Ctrl+S' to "Save Graph"
    And I press 'Ctrl+S'
    Then the "Save Graph" command should be executed

  Scenario: Undo/Redo support
    Given I have added a node "UndoNode"
    When I undo the last command
    Then "UndoNode" should be removed from the graph
    When I redo the command
    Then "UndoNode" should be restored

  Scenario: Attribute-based styling
    Given a node with weight 15
    When the graph is rendered
    Then the node color should be "RED" according to VisualMapper

  Scenario: Viewport metadata in UIPrinter
    Given a graph is displayed
    When I pan the view by (20, 20)
    Then the UI output should contain "Pan: (20, 20)"

  Scenario: Searching for a node
    Given a graph with nodes "Brain, Neuron, Synapse"
    When I search for "Neuron"
    Then "Neuron" should be the focused node
    And the UI output should contain "Focuses: 1"

  Scenario: Switching view modes
    Given a graph is displayed
    When I switch to "Book View" mode
    Then the UI output should contain "Mode: 3"
    When I switch to "Nexus Flow" mode
    Then the UI output should contain "Mode: 5"

  Scenario: Multi-focus interaction
    Given a graph with nodes "A, B, C"
    And multi-focus is enabled
    When I add "A" to focus
    And I add "B" to focus
    Then both "A" and "B" should be focused
    And the UI output should contain "Focuses: 0 1"

  Scenario: Cycling through focus nodes
    Given a graph with nodes "X, Y, Z"
    When I cycle focus
    Then "X" should be focused
    When I cycle focus
    Then "Y" should be focused
