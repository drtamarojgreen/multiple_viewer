Feature: Menu Keyboard Shortcuts
  Verify that all keyboard shortcuts correctly trigger their intended actions.

  Scenario: Zoom Shortcuts
    Given a view context with zoom level "Z3"
    When I press the "Z" key
    Then the zoom level should be "Z4"
    When I press the "X" key
    Then the zoom level should be "Z3"

  Scenario: Panning Shortcuts
    Given a view context with pan offsets (0, 0)
    When I press the "I" key
    Then the pan offsets should be (0, -1)
    When I press the "K" key
    Then the pan offsets should be (0, 0)
    When I press the "J" key
    Then the pan offsets should be (-1, 0)
    When I press the "L" key
    Then the pan offsets should be (0, 0)

  Scenario: Focus and Navigation Shortcuts
    Given a graph with node 0
    And a graph with node 1
    When I press the "F" key with input "1"
    Then node 1 should be focused
    When I press the "TAB" key
    Then node 0 should be focused
    When I press the "O" key with input "0"
    Then node 0 should not be focused

  Scenario: Configuration Toggles
    Given "viewerOverlayMode" is "false"
    When I press the "G" key
    Then "viewerOverlayMode" should be "true"
    When I press the "M" key
    Then "allowMultiFocus" should be "true"
    When I press the "Y" key
    Then the minimap visibility should be toggled
