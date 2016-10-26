// license:BSD-3-Clause
// copyright-holders:Robbbert
/*********************************************************************

    micropolis.h

    Implementations of the Micropolis
    floppy disk controller for the Sorcerer

*********************************************************************/

#ifndef __MICROPOLIS_H__
#define __MICROPOLIS_H__

#include "imagedev/flopdrv.h"


/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

#define MCFG_MICROPOLIS_DRIVE_TAGS(_tag1, _tag2, _tag3, _tag4) \
	micropolis_device::set_drive_tags(*device, _tag1, _tag2, _tag3, _tag4);

#define MCFG_MICROPOLIS_DEFAULT_DRIVE4_TAGS \
	MCFG_MICROPOLIS_DRIVE_TAGS(FLOPPY_0, FLOPPY_1, FLOPPY_2, FLOPPY_3)

#define MCFG_MICROPOLIS_DDEN_CALLBACK(_read) \
	devcb = &micropolis_device::set_dden_rd_callback(*device, DEVCB_##_read);

#define MCFG_MICROPOLIS_INTRQ_CALLBACK(_write) \
	devcb = &micropolis_device::set_intrq_wr_callback(*device, DEVCB_##_write);

#define MCFG_MICROPOLIS_DRQ_CALLBACK(_write) \
	devcb = &micropolis_device::set_drq_wr_callback(*device, DEVCB_##_write);

/***************************************************************************
    MACROS
***************************************************************************/

class micropolis_device : public device_t
{
public:
	micropolis_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	~micropolis_device() {}

	template<class _Object> static devcb_base &set_dden_rd_callback(device_t &device, _Object object) { return downcast<micropolis_device &>(device).m_read_dden.set_callback(object); }
	template<class _Object> static devcb_base &set_intrq_wr_callback(device_t &device, _Object object) { return downcast<micropolis_device &>(device).m_write_intrq.set_callback(object); }
	template<class _Object> static devcb_base &set_drq_wr_callback(device_t &device, _Object object) { return downcast<micropolis_device &>(device).m_write_drq.set_callback(object); }

	static void set_drive_tags(device_t &device, const char *tag1, const char *tag2, const char *tag3, const char *tag4)
	{
		micropolis_device &dev = downcast<micropolis_device &>(device);
		dev.m_floppy_drive_tags[0] = tag1;
		dev.m_floppy_drive_tags[1] = tag2;
		dev.m_floppy_drive_tags[2] = tag3;
		dev.m_floppy_drive_tags[3] = tag4;
	}

	void set_drive(uint8_t drive); // set current drive (0-3)

	uint8_t status_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);

	void command_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t read(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void write(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	// internal state

	devcb_read_line m_read_dden;
	devcb_write_line m_write_intrq;
	devcb_write_line m_write_drq;

	const char *m_floppy_drive_tags[4];

	/* register */
	uint8_t m_data;
	uint8_t m_drive_num;
	uint8_t m_track;
	uint8_t m_sector;
	uint8_t m_command;
	uint8_t m_status;

	uint8_t   m_write_cmd;              /* last write command issued */

	uint8_t   m_buffer[6144];           /* I/O buffer (holds up to a whole track) */
	uint32_t  m_data_offset;            /* offset into I/O buffer */
	int32_t   m_data_count;             /* transfer count from/into I/O buffer */

	uint32_t  m_sector_length;          /* sector length (byte) */

	/* this is the drive currently selected */
	legacy_floppy_image_device *m_drive;

	void read_sector();
	void write_sector();
};

extern const device_type MICROPOLIS;


#endif /* __MICROPOLIS_H__ */
