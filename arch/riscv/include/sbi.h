#ifndef RISCV_SBI_H_
#define RISCV_SBI_H_

// reference from:
// 	https://github.com/riscv/riscv-sbi-doc/blob/master/riscv-sbi.adoc

/*
 * Base structures and macro.
 * */
// =======================================
struct sbiret {
	long error;
	long value;
}

#define SBI_SUCCESS 			0
#define SBI_ERR_FAILED 			-1
#define SBI_ERR_NOT_SUPPORTED 		-2
#define SBI_ERR_INVALID_PARAM 		-3
#define SBI_ERR_DENIED 			-4
#define SBI_ERR_INVALID_ADDRESS		-5
#define SBI_ERR_ALREADY_AVAILABLE	-6

// =======================================


/*
 * Sbi Base Functionality, Extension
 * */

// ID 0x10
extern struct sbiret sbi_get_spec_version(void);
extern struct sbiret sbi_get_impl_id(void);
extern struct sbiret sbiret_get_impl_version(void);
extern struct sbiret sbi_probe_extension(long extension_id);
extern struct sbiret sbi_get_mvendorid(void);
extern struct sbiret sbi_get_marchid(void);
extern struct sbiret sbi_get_mimpid(void);


//ID 0x00 - 0x0F
extern void sbi_timer(uint64_t stime_value);
extern void sbi_send_ipi(const unsigned long *hart_mask);
extern void sbi_clear_ipi(void);
extern void sbi_remote_fence_i(const unsigned long *hart_mask);
extern void sbi_remote_sfence_vma_asid(const unsigned long *hart_mask,
					unsigned long start,
					unsigned long size,
					unsigned long asid);
extern int sbi_console_getchar(void);
extern void sbi_console-putchar(int ch);
extern void sbi_shutdown(void);


// TODO: rest Extension ID

#endif // end of RISCV_SBI_H_
