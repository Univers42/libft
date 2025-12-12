# Engine execution

This is a runtime execution for a repl algorithm like the shell would use..
it implements process/job management
- I/O redirections, pipelines and builtins. 

On OS level programming this kind of program play an essential par on process maangemer / job controller 

# Purpose
orchestate AST -> processes (fork/exec), wiring stdio, collecting exit status,  and optionally runnign builtinsin-process

This orchestratsotr is responsible for separating :
- AST -> executable model translation
- process launching / forking / exec (ProcessLauncher)
- REdirection and file-descriptor management (RedirectionManager)
- Pipeline
- builtin registry and in-process execution
- job/tracking and wait notify (ExeResult)
- Error & result types (ExeREsult)
- Optional callbacks/events (on_start, on_exit) and async va sync execution

# Design
keep low-level fork/xec and FD handling behind an interface so tests can inject a fake runner.
avoid global state (g_should_unwind). Make state explicit via an executor handle. 
Expose both synchronous (blocking) and asynchronous (pid/result callback) APIs
- return structured results (status, pid, signalled flag, c_canceled)rather than raw units.
- provide small testable units: run_commands(), run_pipeline(), run_subshell(), register_builtin(), wait_job()
