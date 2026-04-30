Feature: Rendering Stability and Flicker Prevention
    As a developer
    I want to ensure the rendering loop is stable and free of flickering
    So that the user has a smooth experience

    Scenario: Single Pass Rendering
        Given a graph with multiple nodes
        When a frame is rendered
        Then the screen should be cleared exactly once
        And the graph should be rendered exactly once
        And the menu should be rendered exactly once
        And the status bar should be rendered exactly once
        And the final buffer should be presented exactly once

    Scenario: No Direct Console Output in Render Loop
        Given a graph viewer is active
        When a frame is rendered in "Full Layout" mode
        Then no text should be printed directly to stdout outside the FrameBuffer
        And the pan debug message should not be visible

    Scenario: Book View stability
        Given the graph viewer is in "Book View" mode
        And a graph with nodes from subject 1 and 2
        When a frame is rendered
        Then the text "-- Subject 1 (Depth 0) --" should be visible
        And the text "-- Subject 2 (Depth 0) --" should be visible
        And no flickering-inducing debug messages should be present

    Scenario: Page View stability
        Given a graph with node 0 labeled "DeepThought"
        And the graph viewer is in "Page View" mode for node 0
        When a frame is rendered
        Then the text "Label: DeepThought" should be visible
        And exactly one "CBT Node Page View" header should be present
