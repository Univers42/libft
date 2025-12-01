- output capture tools
- temporary file descriptors
- RAII redirectors
- snapshot comparison
- golden test files
- tempalte helpers

| Goal                                     | Suggested Pattern                                 | Why                                                                                                                                              |
| ---------------------------------------- | ------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------ |
| Capture output from `stdout`/`fd`        | **RAII** (Resource Acquisition Is Initialization) | Automatically redirect output and restore it, cleanly handling resource lifetime.                                                                |
| Test multiple implementations / variants | **Strategy**                                      | Encapsulate different `printf`-like functions behind a common interface so you can swap them easily.                                             |
| Compare outputs / snapshots              | **Template Method**                               | Define a testing “skeleton”: capture output, compare to expected, report result; let specialized tests provide the function and expected output. |
| Multiple test cases / orchestration      | **Command / Test Case**                           | Each test is a self-contained object that can be executed, reported, logged, and composed.                                                       |
| Logging and assertion reporting          | **Observer / Event**                              | Optional: log results, send notifications to console or file without coupling to the test execution.                                             |
