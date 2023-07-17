# Introduction

## The Repository Contains
- ACPI Table: The table creates a ACPI device and provides read or write data function in the operation region.

- Corresponding Linux ACPI Device Driver: One is for ACPI device register, another is for ACPI method access.

## Build Environment
- ACPI Table (Take [QEMU](https://vinxue.github.io/2022/03/01/ovmf_uefi_with_acpi.html) as an example): Copy AcpiTable\AcpiDemo.asl to OvmfPkg\AcpiTables folder, and change below ASL code:
```c
# OvmfPkg\AcpiTables\Dsdt.asl

DefinitionBlock ("Dsdt.aml", "DSDT", 1, "INTEL ", "OVMF    ", 4) {
  //
  // System Sleep States
  //
  // We build S3 and S4 with GetSuspendStates() in
  // "OvmfPkg/AcpiPlatformDxe/Qemu.c".
  //
  Name (\_S0, Package () {5, 0, 0, 0}) // Working
  Name (\_S5, Package () {0, 0, 0, 0}) // Soft Off

  Include ("AcpiDemo.asl")  // New added

```

- Device Driver: Copy device_driver/acpi_demo*.c to Kernel package (e.g. drivers/platform/x86/), add them to makefile.
```makefile
obj-y					+= acpi_demo.o
obj-y					+= acpi_demo_op.o
```

## Verfication
```bash
~ # echo 0xccddeeff > /proc/acpi/demo
[   49.817742] acpi_demo_op: Write data is: 0xCCDDEEFF
[   49.819418] acpi_demo_op: set successfully
~ # cat /proc/acpi/demo
[   58.278745] acpi_demo_op: return data: 0xCCDDEEFF
```
## Boot Command (Windows)
```
"c:\Program Files\qemu\qemu-system-x86_64.exe" -bios OVMF.fd -serial stdio -hda kernel.vhd
```
