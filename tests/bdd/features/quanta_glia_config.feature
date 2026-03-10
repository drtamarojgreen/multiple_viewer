Feature: QuantaGlia Multi-Repository Configuration
  As a researcher
  I want to manage multiple independent brain model repositories
  So that I can switch between different atlases and datasets without interference

  Scenario: Creating and retrieving multiple repositories
    Given a QuantaGlia manager
    When I create a repository named "Atlas_A"
    And I create a repository named "Atlas_B"
    Then I should have 2 active repositories
    And I should be able to retrieve "Atlas_A"
    And I should be able to retrieve "Atlas_B"

  Scenario: Repositories are isolated
    Given a repository named "Atlas_A"
    And a repository named "Atlas_B"
    When I load a region "R1" into "Atlas_A"
    Then "Atlas_A" should contain region "R1"
    And "Atlas_B" should NOT contain region "R1"
