#include <common.h>
#include <asm/nonsecure.h>
#include <libfdt.h>

#define PG_4K_ALING ~((1 << 12) - 1)
#define PG_4K (1 << 12)

int fdt_add_non_sec_mem_rsv(void *blob)
{
	unsigned long rsv_start = non_sec_start & PG_4K_ALING;
	unsigned long rsv_end = non_sec_end & PG_4K_ALING;
	unsigned long rsv_size;
	int off;

	if (non_sec_end == non_sec_start + 0x4)
		return 0;

	if (rsv_start != rsv_end)
		rsv_size = 2 * PG_4K;
	else
		rsv_size = PG_4K;

	off = fdt_add_mem_rsv(blob, rsv_start, (u64)rsv_size);
	if (off < 0)
		printf("Failed to reserve memory for spin table: %s\n",
		       fdt_strerror(off));

	return off;
}
