# MIPS Pipeline and Non-Pipeline Processor Project

## Project Overview
This project implements two MIPS-based processor models:

- **Pipeline Processor**: Executes instructions using a pipelined architecture, enhancing performance by overlapping stages of instruction execution.
- **Non-Pipeline Processor**: Executes instructions sequentially without pipelining, providing a simpler model to evaluate baseline performance.

Both processors accept binary input and provide the desired output, allowing for a comparative analysis of processing speed and efficiency between pipelined and non-pipelined architectures.

## Key Features
### Pipeline Processor:
- **Stages**: Implements the standard MIPS pipeline stages:
  - **Instruction Fetch (IF)**
  - **Instruction Decode (ID)**
  - **Execute (EX)**
  - **Memory Access (MEM)**
  - **Write Back (WB)**
- **Hazard Handling**: Includes handling for common pipeline hazards, ensuring efficient execution.

### Non-Pipeline Processor:
- **Sequential Execution**: Executes instructions one at a time from start to finish, without overlapping stages.
- **Baseline Model**: Provides a simpler execution model, useful for comparison with the pipelined processor.
