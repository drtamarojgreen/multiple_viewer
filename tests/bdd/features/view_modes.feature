Feature: View Mode Management
    As a user of the graph viewer
    I want to switch between different display modes
    So that I can analyze the graph in various perspectives

    Scenario: Graph View does not display in Book View
        Given the graph viewer is in "Book View" mode
        And a graph with 3 nodes exists
        When the screen is rendered
        Then the "Full Layout" graph content should not be rendered
        And the "Book View" content should be rendered

    Scenario: Page View works correctly
        Given a graph with 1 node (index 0, label "TestNode")
        And the graph viewer is in "Page View" mode for node 0
        When the screen is rendered
        Then the text "=== CBT Node Page View ===" should be visible
        And the text "Label: TestNode" should be visible
        And the text "Index: 0" should be visible

    Scenario: Page View does not appear in Full Layout View
        Given a graph with 1 node
        And the graph viewer is in "Full Layout" mode
        When the screen is rendered
        Then the text "=== CBT Node Page View ===" should not be visible

    Scenario: Page View does not appear in Book View
        Given a graph with 1 node
        And the graph viewer is in "Book View" mode
        When the screen is rendered
        Then the text "=== CBT Node Page View ===" should not be visible

