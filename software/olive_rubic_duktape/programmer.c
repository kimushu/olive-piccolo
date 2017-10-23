#include "system.h"
#include "epcs_fatfs.h"
#include "altera_onchip_flash.h"
#include "sys/alt_flash.h"
#include <string.h>

static alt_onchip_flash_dev *ufm_dev;

void prog_init(void)
{
    ufm_dev = (alt_onchip_flash_dev *)alt_flash_open_dev(UFM_DATA_NAME);
}

int prog_blocksize(const char *area, void *user_data)
{
    if (strcmp(area, "spi") == 0) {
        return epcs_fatfs_raw_get_sectsize();
    } else if (strcmp(area, "img") == 0) {
        return ufm_dev->page_size;
    } else if (strcmp(area, "ufm") == 0) {
        return ufm_dev->page_size;
    } else {
        return -1;
    }
}

int prog_reader(const char *area, void *user_data, int offset, void *ptr, int len)
{
    if (strcmp(area, "spi") == 0) {
        return epcs_fatfs_raw_read(offset, ptr, len);
    } else if (strcmp(area, "img") == 0) {
    	return alt_onchip_flash_read(&ufm_dev->dev, offset + ufm_dev->sector3_start_addr, ptr, len);
    } else if (strcmp(area, "ufm") == 0) {
    	return alt_onchip_flash_read(&ufm_dev->dev, offset + ufm_dev->sector1_start_addr, ptr, len);
    } else {
        return -1;
    }
}

int prog_writer(const char *area, void *user_data, int offset, const void *ptr, int len)
{
    if (strcmp(area, "spi") == 0) {
        return epcs_fatfs_raw_write(offset, ptr, len);
    } else if (strcmp(area, "img") == 0) {
    	return alt_onchip_flash_write(&ufm_dev->dev, offset + ufm_dev->sector3_start_addr, ptr, len);
    } else if (strcmp(area, "ufm") == 0) {
    	return alt_onchip_flash_write(&ufm_dev->dev, offset + ufm_dev->sector1_start_addr, ptr, len);
    } else {
        return -1;
    }
}
