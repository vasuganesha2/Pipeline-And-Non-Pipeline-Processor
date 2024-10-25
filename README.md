MIPS Pipeline and Non-Pipeline Processor Project
Project Overview
This project implements two MIPS-based processor models:

Pipeline Processor: Executes instructions using a pipelined architecture, enhancing performance by overlapping stages of instruction execution.
Non-Pipeline Processor: Executes instructions sequentially without pipelining, providing a simpler model to evaluate baseline performance.
Both processors accept binary input and gives the desired output allowing for a comparative analysis of processing speed and efficiency between pipelined and non-pipelined architectures.

Key Features
Pipeline Processor:
Stages: Implements the standard MIPS pipeline stages: Instruction Fetch (IF), Instruction Decode (ID), Execute (EX), Memory Access (MEM), and Write Back (WB).
Hazard Handling: Includes handling for common pipeline hazards, ensuring efficient execution.
Non-Pipeline Processor:
Executes instructions sequentially, processing one instruction at a time from start to finish.
Provides a simpler execution model, serving as a comparison baseline to the pipelined processor.
