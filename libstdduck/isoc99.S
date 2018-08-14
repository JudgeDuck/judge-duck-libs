.text

.globl __isoc99_fscanf
.globl __isoc99_scanf
.globl __isoc99_sscanf
.globl __isoc99_vfscanf
.globl __isoc99_vscanf
.globl __isoc99_vsscanf

.globl fscanf
.globl scanf
.globl sscanf
.globl vfscanf
.globl vscanf
.globl vsscanf


__isoc99_fscanf:
jmp fscanf

__isoc99_scanf:
jmp scanf

__isoc99_sscanf:
jmp sscanf

__isoc99_vfscanf:
jmp vfscanf

__isoc99_vscanf:
jmp vscanf

__isoc99_vsscanf:
jmp vsscanf


