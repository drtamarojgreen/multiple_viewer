Feature: Advanced Brain Model Features
  Scenario: Hierarchical region traversal
    Given a brain model with region "H1" parent of "L1"
    And "L1" is parent of "A1"
    Then the hierarchy path for "A1" should be "H1, L1, A1"
    And the descendants of "H1" should include "L1" and "A1"

  Scenario: Probabilistic node membership
    Given a brain node "N1" with 70% membership in "R1" and 30% in "R2"
    Then node "N1" should have 2 memberships
    And the probability for "R1" should be 0.7

  Scenario: Model capability check
    Given a model "mni_v2" with temporal and weight capabilities
    Then model "mni_v2" should support "temporal"
    And model "mni_v2" should support "weights"
    But model "mni_v2" should not support "probabilistic"

  Scenario: ROI selection
    Given a brain model with regions at (0,0,0) and (10,0,0) with radius 1
    When I query regions in radius 5 at (0,0,0)
    Then 1 region should be returned
