/** @file
  Contains ACPI demo device

  Copyright (c) 2023, Gavin Xue. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

//
//  ACPI Demo Device
//
Scope (\_SB) {
  Device (DEM0) {
    Name (_HID, "DEMO0001")

    Method (_STA, 0, NotSerialized) {
      Return (0x0F)
    }

    OperationRegion (TPMR, SystemMemory, 0x1000, 0x10)
    Field (TPMR, AnyAcc, NoLock, Preserve)
    {
        DATA, 32
    }

    //
    // Set data
    //
    Method (SETM, 1, Serialized)
    {
      Store (Arg0, DATA)
    }

    //
    // Return data
    //
    Method (RETM, 0, Serialized)
    {
      return (DATA)
    }
  }
}
