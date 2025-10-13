# Issue Tracking Log

This log tracks requirements from the project requirements document that are not currently operating to standard, are incomplete, or lack attention. Each entry includes the requirement number, a description of the issue, current status, possible fixes, and possible mitigations.

---

## 1. Requirement 15: Multi-focus support

**Issue:**  
Multi-focus is partially implemented; user experience is incomplete and not fully intuitive.

**Current Status:**  
Basic multi-focus logic exists, but UI feedback, batch operations, and analytics for multiple foci are limited.

**Possible Fixes:**  
- Improve UI to clearly display all focused nodes.
- Allow batch add/remove of foci.
- Enhance analytics to operate on all focused nodes.

**Possible Mitigations:**  
- Document current limitations in the user guide.
- Provide a warning when multi-focus is toggled.

---

## 2. Requirement 32: Highlight connected components

**Issue:**  
No clear visual or analytic feedback for connected components.

**Current Status:**  
Functionality may be missing or only available via code, not UI.

**Possible Fixes:**  
- Implement a function to find and highlight connected components.
- Add a menu option to trigger this analysis.

**Possible Mitigations:**  
- Document as a future feature.
- Suggest manual analysis using node/edge lists.

---

## 3. Requirement 33: Visualize shortest path

**Issue:**  
No UI or rendering for shortest path between nodes.

**Current Status:**  
Algorithm may exist, but not exposed to user or visualized.

**Possible Fixes:**  
- Implement Dijkstra/BFS for shortest path.
- Highlight path in the viewer.

**Possible Mitigations:**  
- Document as a planned feature.
- Provide CLI output as a temporary solution.

---

## 4. Requirement 34: Show proximity depth

**Issue:**  
Proximity depth not visualized or easily accessible.

**Current Status:**  
May be calculated internally but not shown to user.

**Possible Fixes:**  
- Display proximity depth in node details or overlays.
- Color nodes by proximity.

**Possible Mitigations:**  
- Add to analytics panel as a text output.

---

## 5. Requirement 35: Support multiple layouts (e.g., Book View)

**Issue:**  
Only one layout (default) is available.

**Current Status:**  
Book View and other layouts not implemented.

**Possible Fixes:**  
- Implement alternative layout algorithms.
- Add menu options to switch layouts.

**Possible Mitigations:**  
- Document as a future enhancement.

---

## 6. Requirement 36: Switch between layouts

**Issue:**  
No UI or command to change layouts.

**Current Status:**  
Layout switching logic missing.

**Possible Fixes:**  
- Add a layout selection menu or key binding.
- Store current layout in settings.

**Possible Mitigations:**  
- Document as a planned feature.

---

## 7. Requirement 37: Display overlays (analytics, weights)

**Issue:**  
Overlays for analytics and edge weights are not available.

**Current Status:**  
No overlay rendering logic.

**Possible Fixes:**  
- Implement overlay rendering in the viewer.
- Add toggle keys for overlays.

**Possible Mitigations:**  
- Provide analytics in a separate panel or CLI output.

---

## 8. Requirement 45: Batch add/remove nodes/edges

**Issue:**  
No batch operations for nodes/edges.

**Current Status:**  
Nodes and edges must be added/removed individually.

**Possible Fixes:**  
- Implement batch add/remove functions.
- Allow CSV or list input for batch operations.

**Possible Mitigations:**  
- Document as a limitation.
- Suggest scripting as a workaround.

---

## 9. Requirement 50: Plugin system for analytics

**Issue:**  
No plugin architecture for analytics.

**Current Status:**  
Analytics are hardcoded.

**Possible Fixes:**  
- Design and implement a plugin API.
- Allow dynamic loading of analytics modules.

**Possible Mitigations:**  
- Document as a future goal.

---

## 10. Requirement 51: Scripting support

**Issue:**  
No scripting interface for automating tasks.

**Current Status:**  
All actions are manual.

**Possible Fixes:**  
- Integrate a scripting language (e.g., Lua, Python).
- Expose core functions to scripts.

**Possible Mitigations:**  
- Allow command batching via CLI as a temporary measure.

---

## 11. Requirement 52: API for automation

**Issue:**  
No external API for automation.

**Current Status:**  
No way to control the app programmatically.

**Possible Fixes:**  
- Implement a REST or CLI API.
- Document API endpoints and usage.

**Possible Mitigations:**  
- Provide a roadmap for API development.

---

## 12. Requirement 57: Unit tests for all functions

**Issue:**  
Unit test coverage is incomplete.

**Current Status:**  
Some functions may lack tests.

**Possible Fixes:**  
- Write unit tests for all public functions.
- Use a test framework (e.g., Google Test).

**Possible Mitigations:**  
- Prioritize testing for critical functions.

---

## 13. Requirement 58: Integration tests for workflows

**Issue:**  
Integration tests are missing or incomplete.

**Current Status:**  
No automated workflow testing.

**Possible Fixes:**  
- Develop integration tests simulating user actions.
- Automate test execution.

**Possible Mitigations:**  
- Manually test workflows until automation is ready.

---

## 14. Requirement 59: Performance benchmarks

**Issue:**  
No benchmarking for rendering or large graphs.

**Current Status:**  
Performance unmeasured.

**Possible Fixes:**  
- Implement benchmark tests for rendering and data loading.
- Profile and optimize slow code paths.

**Possible Mitigations:**  
- Document known performance bottlenecks.

---

## 15. Requirement 60: Code documentation

**Issue:**  
Documentation for public APIs is incomplete.

**Current Status:**  
Some functions lack docstrings or comments.

**Possible Fixes:**  
- Add docstrings to all public functions and classes.
- Generate API docs automatically.

**Possible Mitigations:**  
- Provide high-level documentation as a stopgap.

---

## 16. Requirement 61: Support color output

**Issue:**  
Color output is not fully supported or portable.

**Current Status:**  
Limited or no color in console.

**Possible Fixes:**  
- Use cross-platform libraries for color (e.g., ncurses, termcolor).
- Detect terminal capabilities.

**Possible Mitigations:**  
- Use symbols or text cues as fallback.

---

## 17. Requirement 62: Localization

**Issue:**  
No support for multiple languages.

**Current Status:**  
All UI text is hardcoded in English.

**Possible Fixes:**  
- Externalize strings to resource files.
- Implement a translation system.

**Possible Mitigations:**  
- Document as a future feature.

---

## 18. Requirement 63: Customizable key bindings

**Issue:**  
Key bindings are hardcoded.

**Current Status:**  
Users cannot remap keys.

**Possible Fixes:**  
- Implement a key binding configuration file.
- Add UI for key remapping.

**Possible Mitigations:**  
- Document current key map.

---



## 24. Requirement 69: Graph integrity check

**Issue:**  
No automated integrity checks.

**Current Status:**  
Corrupt or inconsistent graphs may go undetected.

**Possible Fixes:**  
- Implement integrity checks on load/save.
- Alert user to issues.

**Possible Mitigations:**  
- Provide manual check tools.

---



## 26. Requirement 71: 3D graph rendering

**Issue:**  
No support for 3D rendering.

**Current Status:**  
2D only.

**Possible Fixes:**  
- Implement 3D rendering (optional, advanced).
- Use ASCII or external tools for 3D.

**Possible Mitigations:**  
- Document as a future enhancement.

---

## 28. Requirement 73: Graph merging

**Issue:**  
No merging of graphs.

**Current Status:**  
Graphs are isolated.

**Possible Fixes:**  
- Implement merge logic with conflict resolution.
- Allow user to select merge options.

**Possible Mitigations:**  
- Document as a future feature.

---

## 29. Requirement 74: Graph splitting

**Issue:**  
No support for splitting graphs.

**Current Status:**  
Cannot partition graphs.

**Possible Fixes:**  
- Implement split function based on node selection.
- Allow user to save splits as new graphs.

**Possible Mitigations:**  
- Suggest manual extraction as workaround.

---

## 30. Requirement 75: Graph comparison

**Issue:**  
No comparison tools.

**Current Status:**  
Cannot compare two graphs.

**Possible Fixes:**  
- Implement comparison logic.
- Highlight differences in UI.

**Possible Mitigations:**  
- Provide CLI comparison as a temporary solution.

---

## 31. Requirement 76: Graph statistics summary

**Issue:**  
No summary analytics.

**Current Status:**  
Only basic stats shown.

**Possible Fixes:**  
- Implement a summary panel for key statistics.
- Display on demand.

**Possible Mitigations:**  
- Document as a future feature.

---


## 36. Requirement 81: Node/edge grouping

**Issue:**  
No grouping functionality.

**Current Status:**  
Nodes and edges are managed individually.

**Possible Fixes:**  
- Implement group creation and management.
- Allow group operations.

**Possible Mitigations:**  
- Suggest using labels or attributes for grouping.

---


## 40. Requirement 85: Accessibility features

**Issue:**  
Limited accessibility.

**Current Status:**  
No screen reader or high-contrast support.

**Possible Fixes:**  
- Add screen reader compatibility.
- Provide high-contrast mode.

**Possible Mitigations:**  
- Document accessibility limitations.

---

## 41. Requirement 86: Export/import settings

**Issue:**  
No settings export/import.

**Current Status:**  
Settings are local only.

**Possible Fixes:**  
- Implement settings serialization.
- Allow import/export via file.

**Possible Mitigations:**  
- Document manual settings backup.

---

## 42. Requirement 87: Graph clustering

**Issue:**  
No clustering algorithms.

**Current Status:**  
No group detection.

**Possible Fixes:**  
- Implement clustering (e.g., community detection).
- Visualize clusters.

**Possible Mitigations:**  
- Document as a future feature.

---

## 43. Requirement 88: Graph layout optimization

**Issue:**  
No optimization for layout aesthetics.

**Current Status:**  
Basic layout only.

**Possible Fixes:**  
- Implement force-directed or other optimization algorithms.
- Minimize edge crossings.

**Possible Mitigations:**  
- Allow manual node positioning.

---

## 44. Requirement 89: Node/edge locking

**Issue:**  
No locking to prevent edits.

**Current Status:**  
All nodes/edges are editable.

**Possible Fixes:**  
- Add lock flags to nodes/edges.
- Prevent modification when locked.

**Possible Mitigations:**  
- Warn users before editing critical nodes.

---

## 45. Requirement 90: Custom analytics scripts

**Issue:**  
No support for user analytics scripts.

**Current Status:**  
Analytics are built-in only.

**Possible Fixes:**  
- Allow users to write and run scripts.
- Sandbox script execution.

**Possible Mitigations:**  
- Document as a future feature.

---

## 46. Requirement 91: Graph export to other formats

**Issue:**  
Limited export options.

**Current Status:**  
CSV only.

**Possible Fixes:**  
- Support export to GraphML, JSON, etc.
- Add format selection to UI.

**Possible Mitigations:**  
- Provide conversion scripts.

---

## 47. Requirement 92: Import from external sources

**Issue:**  
No import from APIs or other tools.

**Current Status:**  
Manual import only.

**Possible Fixes:**  
- Implement importers for common formats/APIs.
- Allow user to map fields.

**Possible Mitigations:**  
- Document manual import steps.

---

## 48. Requirement 93: Graph notifications

**Issue:**  
No notification system.

**Current Status:**  
No feedback for background events.

**Possible Fixes:**  
- Implement notification queue.
- Display messages in status bar.

**Possible Mitigations:**  
- Use console output as fallback.

---

