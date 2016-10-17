#define CR0_PE          0x00000001      // 保护模式
#define CR0_WP          0x00010000      // 写保护
#define CR0_PG          0x80000000      // 分页

#define SEG_KCODE 1  // 内核代码段
#define SEG_KDATA 2  // 内核数据段
#define SEG_KCPU  3  // 内核cpu数据段(每个cpu都有)
#define SEG_UCODE 4  // 用户代码段
#define SEG_UDATA 5  // 用户数据段
#define SEG_TSS   6  // 进程tss段
