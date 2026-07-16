<h1 align="center">Configurable Matrix Multiplication Accelerator using Vitis HLS</h1>

<p align="center">
<b>Reusable Hardware Accelerator IP Core for Matrix Multiplication</b><br>
Developed using <b>Vitis HLS 2023.2</b><br>
Target FPGA: <b>Digilent Nexys A7-100T (XC7A100T-CSG324-1)</b>
</p>

---

# Overview

This project implements a **Configurable Matrix Multiplication Accelerator** using **Vitis HLS**. The accelerator is capable of multiplying matrices of different dimensions using the **same synthesized hardware**. Matrix dimensions are configured at runtime through **AXI4-Lite** registers while matrix data is transferred through **AXI4 Master** interfaces.

Unlike a fixed-size hardware implementation, this IP supports different matrix sizes (up to the predefined limits) without requiring hardware re-synthesis.

<br>

## Objectives

- Design a reusable Matrix Multiplication IP.
- Support runtime configurable matrix dimensions (**M**, **K**, **N**).
- Implement AXI4 Master and AXI4-Lite interfaces.
- Study HLS optimization techniques.
- Compare Floating Point, Fixed Point and Integer implementations.
- Analyze latency and FPGA resource utilization.

---

# Features

- Runtime configurable matrix dimensions
- AXI4 Master interfaces for Matrix A, Matrix B and Matrix C
- AXI4-Lite control interface
- Local BRAM buffering
- Loop Pipelining
- Array Partitioning
- Loop Unrolling experiments
- Floating Point implementation
- Fixed Point implementation
- Integer implementation
- Complete design-space exploration

---

# Development Environment

<table>
<tr><th>Item</th><th>Specification</th></tr>
<tr><td>Tool</td><td>Vitis HLS 2023.2</td></tr>
<tr><td>Language</td><td>C++</td></tr>
<tr><td>FPGA Board</td><td>Digilent Nexys A7-100T</td></tr>
<tr><td>FPGA Device</td><td>XC7A100T-CSG324-1</td></tr>
<tr><td>Interface</td><td>AXI4 Master + AXI4-Lite</td></tr>
</table>

---

# Hardware Architecture

```text
                 AXI4-Lite
                     │
                     ▼
          Configuration Registers
                     │
                     ▼
        ┌───────────────────────────┐
        │ Matrix Multiplication IP  │
        │                           │
AXI4 -->│ Local Buffer A (BRAM)     │
AXI4 -->│ Local Buffer B (BRAM)     │
        │                           │
        │ Multiply-Accumulate Unit  │
AXI4 <--│ Output Matrix C           │
        └───────────────────────────┘
```

> **Note:** Replace this diagram with your Microsoft Visio architecture image.

---

# Supported Matrix Dimensions

<table>
<tr><th>Parameter</th><th>Description</th></tr>
<tr><td>M</td><td>Rows of Matrix A</td></tr>
<tr><td>K</td><td>Columns of Matrix A / Rows of Matrix B</td></tr>
<tr><td>N</td><td>Columns of Matrix B</td></tr>
</table>

The hardware supports runtime configuration within the predefined limits:

```cpp
MAX_M = 32
MAX_K = 32
MAX_N = 32
```

---

# Optimization Methodology

The accelerator was optimized incrementally to study the effect of each optimization.

1. Baseline Design
2. Local Buffer A
3. Local Buffer B
4. Loop Pipelining
5. Array Partitioning
6. Loop Unrolling
7. Datatype Exploration

---

# Floating Point Optimization Results

<table>
<tr>
<th>Version</th>
<th>Clock (ns)</th>
<th>Latency (Cycles)</th>
<th>II</th>
<th>BRAM</th>
<th>DSP</th>
<th>FF</th>
<th>LUT</th>
</tr>

<tr><td>Baseline</td><td>7.300</td><td>253962</td><td>7</td><td>4</td><td>16</td><td>3497</td><td>3163</td></tr>
<tr><td>Local Buffer A</td><td>7.300</td><td>249879</td><td>7</td><td>6</td><td>12</td><td>3296</td><td>3504</td></tr>
<tr><td>Local Buffer A + B</td><td>7.300</td><td>243748</td><td>7</td><td>8</td><td>19</td><td>3686</td><td>3958</td></tr>
<tr><td><b>Pipeline</b></td><td>7.300</td><td><b>178212</b></td><td><b>5</b></td><td>8</td><td>17</td><td>3627</td><td>4129</td></tr>
<tr><td>Array Partition</td><td>7.300</td><td>178212</td><td>5</td><td>12</td><td>17</td><td>3663</td><td>4401</td></tr>
<tr><td>Unroll ×2</td><td>11.321</td><td>182308</td><td>10</td><td>8</td><td>17</td><td>3855</td><td>4358</td></tr>
<tr><td>Partition + Unroll</td><td>11.321</td><td>182308</td><td>10</td><td>12</td><td>17</td><td>3890</td><td>4682</td></tr>

</table>

---

# Datatype Comparison

<table>
<tr>
<th>Datatype</th>
<th>Clock (ns)</th>
<th>Latency (Cycles)</th>
<th>II</th>
<th>BRAM</th>
<th>DSP</th>
<th>FF</th>
<th>LUT</th>
</tr>

<tr><td>Float</td><td>7.300</td><td>178212</td><td>5</td><td>8</td><td>17</td><td>3627</td><td>4129</td></tr>
<tr><td><b>ap_fixed&lt;16,8&gt;</b></td><td>7.300</td><td><b>44068</b></td><td><b>1</b></td><td>4</td><td>15</td><td>3179</td><td>3674</td></tr>
<tr><td>ap_int&lt;8&gt;</td><td>7.300</td><td>44068</td><td>1</td><td>4</td><td>15</td><td>3172</td><td>3697</td></tr>
</table>

---

# Key Findings

- Local BRAM buffers reduced repeated AXI memory accesses and improved execution latency.
- Loop pipelining delivered the largest performance improvement (~30%) for the floating-point implementation.
- Array partitioning alone did not reduce latency because memory bandwidth was already sufficient for the access pattern.
- Loop unrolling increased the critical path due to the floating-point reduction dependency and therefore degraded performance.
- Fixed-point arithmetic achieved **II = 1** and reduced latency by more than **75%** while maintaining fractional precision.
- Integer arithmetic also achieved **II = 1**, but it cannot accurately represent fractional values.

---

# Repository Structure

```text
Configurable-Matrix-Multiplication-HLS
│
├── src/
│   ├── matrix_mul.cpp
│   ├── matrix_mul.h
│   └── matrix_mul_tb.cpp
│
├── reports/
│
├── images/
│
└── README.md
```

---

# Future Work

- Tiled Matrix Multiplication
- Parallel Reduction (Adder Tree)
- Multiple Processing Elements (PEs)
- Systolic Array Architecture
- AXI4-Stream Interface
- DMA-based Data Transfer
- Larger Matrix Support

---

# Conclusion

This project demonstrates the complete design flow of a configurable FPGA hardware accelerator using Vitis HLS, starting from a functional C model and progressing through systematic optimization and hardware synthesis.

A detailed design-space exploration was performed by evaluating memory optimizations, pipelining, array partitioning, loop unrolling, and datatype selection. The study showed that loop pipelining provides the highest improvement for the floating-point design, while fixed-point arithmetic offers the best balance between performance, resource utilization, and numerical capability.

The resulting accelerator is reusable, configurable, and serves as a strong foundation for more advanced architectures such as tiled matrix multiplication and systolic array accelerators.
