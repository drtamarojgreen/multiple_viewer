Feature: Menu Display
    As a user of the graph viewer
    I want to see the complete user menu
    So that I know which actions I can perform

    Scenario: User menu is completely displayed in Full Layout mode
        Given a graph with 0 nodes
        And the graph viewer is in "Full Layout" mode
        When the screen is rendered
        Then the full menu text should be visible on the screen

    Scenario: User menu is completely displayed in Book View mode
        Given a graph with 0 nodes
        And the graph viewer is in "Book View" mode
        When the screen is rendered
        Then the full menu text should be visible on the screen

    Scenario: User menu is completely displayed in Nexus Flow mode
        Given a graph with 0 nodes
        And the graph viewer is in "Nexus Flow" mode
        When the screen is rendered
        Then the full menu text should be visible on the screen

    Scenario: User menu is completely displayed in Page View mode
        Given a graph with 1 node (index 0, label "TestNode")
        And the graph viewer is in "Page View" mode for node 0
        When the screen is rendered
        Then the full menu text should be visible on the screen
