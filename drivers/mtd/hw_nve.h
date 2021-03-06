/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * FileName: kernel/drivers/mtd/hw_nve.h
 * Description: define some macros and declear some functions that will be
 * used in file hw_nve.c.
 * Author: Chengxiaoyi
 * Create: 2020-09-28
 */

#ifndef __HW_NVE_H
#define __HW_NVE_H

#include <linux/mtd/hw_nve_interface.h>
#include <stddef.h>

#define TEST_NV_IN_KERNEL 1
#define CONFIG_CRC_SUPPORT
#define NVE_PARTITION_SIZE (128 * 1024)
#define NVE_PARTITION_NUMBER 7
#define NVE_INVALID_NVM 0xFFFFFFFF
#define NVE_PARTITION_INVALID_AGE         0x0
#define NVE_PARTITION_COUNT 8
#define NVE_BASE_VERSION         0x1
#define NVE_CRC_SUPPORT_VERSION         0x2
#define NV_ITEMS_MAX_NUMBER     1023
#define NVE_HEADER_NAME "Hisi-NV-Partition" /* ReliableData area */
#define NVE_BLOCK_SIZE 512
#define NVE_NV_DATA_SIZE 104
#define NV_DEVICE_NAME "/dev/block/by-name/nvme"

#define NV_INFO_LEN 1024
#define NVE_ERROR (-1)

#define CONFIG_HW_DEBUG_FS 1
#define MIN_NUM 0
#define MAX_NUM 9
#define NUM_CHAR 10
#define SHIFT_COUNT 4
#define SLEEP_TIME 20
#define MIN_PARTITION 3


/*
 * In case accidently power-off happened when NV
 * is writing,we put the partition_header at the
 * position that locate the last 128 Bytes of every
 * partition,so even if power-off happend,current
 * partition's age will not increase which means current
 * partition is not updated and is invalid partiton.
 */

#define PARTITION_HEADER_SIZE 128
#define PARTITION_HEADER_OFFSET (NVE_PARTITION_SIZE - PARTITION_HEADER_SIZE)

#define NVE_ERROR_INIT 1
#define NVE_ERROR_PARAM 2
#define NVE_ERROR_PARTITION 3

#define PARTITION_NAME_LENTH 32
#define PARTITION_RESERVED_LENTH 68

#ifdef CONFIG_HW_NVE_WHITELIST
extern unsigned int get_userlock(void);
#endif

struct nve_partition_header_struct {
	char nve_partition_name[PARTITION_NAME_LENTH];
	/* should be built in image with const value */
	unsigned int nve_version;
	/* should be built in image with const value */
	unsigned int nve_block_id;
	/* should be built in image with const value */
	unsigned int nve_block_count;
	/* should be built in image with const value */
	unsigned int valid_items;
	unsigned int nv_checksum;
	unsigned int nve_crc_support;
	unsigned char reserved[PARTITION_RESERVED_LENTH];
	/* changed by run-time image */
	unsigned int nve_age;
};

/*
 * nv_items_struct and nv_partittion_struct struct
 * are used for get NV partition information,
 * only used for debug and test.
 */
struct nv_items_struct {
	unsigned int nv_number;
	char nv_name[NV_NAME_LENGTH];
	unsigned int nv_property; /* 0 for volatile; 1 for non-volatile */
	unsigned int valid_size;
/*
 * The length of all parameters before crc is
 * saved as NVE_NV_CRC_HEADER_SIZE.
 */
	unsigned int crc;
	char nv_data[NVE_NV_DATA_SIZE];
};

struct nv_partittion_struct {
	struct nv_items_struct nv_items[NV_ITEMS_MAX_NUMBER];
	struct nve_partition_header_struct header;
};

struct nve_ramdisk_struct {
	int nve_major_number;
	int initialized;
	unsigned int nve_partition_count;
	unsigned int nve_current_id;
	struct nv_partittion_struct *nve_current_ramdisk;
	struct nv_partittion_struct *nve_update_ramdisk;
	struct nv_partittion_struct *nve_store_ramdisk;
};

#define NVE_NV_CRC_HEADER_SIZE (offsetof(struct nv_items_struct, crc))

#endif
