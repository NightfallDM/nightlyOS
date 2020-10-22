#ifndef RISCV_SBI_H_
#define RISCV_SBI_H_

// reference from:
// 	https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc

struct sbiret {
	long error;
	long value;
}

#define SBI_SUCCESS 0
#define SBI_ERR_FAILED -1
#define SBI

#endif // end of RISCV_SBI_H_
