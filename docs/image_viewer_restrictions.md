# SDD Restrictions: Image Viewer System

## 1. Architectural Guardrails
- **Manual Verification Logic:** Each requirement must be verified by a unique, manually written C++ card that performs a real action. Script-generated tests are prohibited.
- **Empirical Execution:** Results must be derived from the system's actual data processing. Mocks or proxies are allowed only for environmental factors (e.g., GPU absence).
- **Strict Decoupling:** Core logic (ImageManager) must be independent of visualization (ImageRenderer).

## 2. SDD Card Principles
- **Minimalism:** Each card must perform a single measurable action.
- **Traceability:** Completed work must be recorded in `chai_checkouts.md` with the corresponding card filename.
- **Binary Prohibition:** No binary assets (images, executables) are allowed in the repository.
