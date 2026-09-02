# Scrambling Compiler results

These files are derived side-channel attack results for the paper [*Scrambling Compiler: Automated and Unified Countermeasure for Profiled and Non-Profiled Side Channel Attacks*](https://re.public.polimi.it/retrieve/910a49bd-1ee4-499e-9d5a-e16488a62524/main.pdf).

The paper evaluates a compiler-applied Computation Interleaving countermeasure on AES-128 running on an STM32F4 ARM Cortex-M4, measured with a ChipWhisperer 1200 Pro at 7.37 MHz. The files below cover the non-profiled evaluation in Section 4 and Figure 3: Correlation Power Analysis (CPA) and non-specific t-tests.

## Files

| File | Data shape | First column | Other columns |
| --- | ---: | --- | --- |
| `unprot_CPA_incr_results.csv` | 400 × 257 | Trace count, 10–4,000 | 256 CPA correlation values |
| `protected_CPA_incr_results.csv` | 500 × 257 | Trace count, 100–50,000 | 256 CPA correlation values |
| `unprot_CPA_results.csv` | 3,000 × 257 | Sample index, 0–2,999 | 256 CPA correlation values |
| `protected_CPA_results.csv` | 6,000 × 257 | Sample index, 0–5,999 | 256 CPA correlation values |
| `unprot-500-ttest.csv` | 15,000 data rows | `time` | `tstat`, using 500 traces |
| `unprot-5k-ttest.csv` | 15,000 data rows | `time` | `tstat`, using 5,000 traces |
| `protected_ttest.csv` | 70,000 data rows | `time` | `tstat`, using 50,000 traces |

