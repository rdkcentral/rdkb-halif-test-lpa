# Unit Testing Suite For LPA HAL

The following documents should be referenced to provide specific information on the tests required for this module.

## Table of Contents

- [Version History](#version-history)
- [Acronyms, Terms and Abbreviations](#acronyms-terms-and-abbreviations)
- [Description](#description)
- [Reference Documents](#reference-documents)

## Version History

| Date | Comment | Version |
| --- | --- | --- |
| 03/06/24 | Initial Release | 1.0.0 |

## Acronyms, Terms and Abbreviations

- `L1` - Unit Tests
- `HAL`- Hardware Abstraction Layer

## Description

This repository contains the Unit Test Suites L1 for LPA HAL.

## Reference Documents

|SNo|Document Name|Document Description|Document Link|
|---|-------------|--------------------|-------------|
|1|`HAL` Specification Document|This document provides specific information on the APIs for which tests are written in this module|[LpaHalSpec.md](../../../../../rdkcentral/rdkb-halif-lpa/blob/main/docs/pages/LpaHalSpec.md "LpaHalSpec.md")|
|2|`L1` Tests | `L1` Test Case File for this module |[test_l1_lpa_hal.c](src/test_l1_lpa_hal.c "test_l1_lpa_hal.c")|

## Populate Configuration File

The users of lpa hal 3PE test suite can populate appropriate platform specific values for the below parameters in the configuration file "lpa_config" before executing the test binary.

1. For iccid, fill with available iccid values as a list of strings. Refer the example given below :

    {
        "iccid": ["12345678901234567890","12121212121212121212"]
    }

   if we have only one iccid, Refer the example given below :

    {
        "iccid": ["12345678901234567890"]
    }

