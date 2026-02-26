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
