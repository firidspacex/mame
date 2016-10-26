// license:BSD-3-Clause
// copyright-holders:smf
/*

scsihle.h

Base class for HLE'd SCSI devices.

*/

#ifndef _SCSIHLE_H_
#define _SCSIHLE_H_

#include "scsi.h"
#include "machine/t10spc.h"

INPUT_PORTS_EXTERN( scsihle );

class scsihle_device : public device_t,
	public scsi_port_interface,
	public virtual t10spc
{
public:
	// construction/destruction
	scsihle_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, uint32_t clock, const char *shortname, const char *source);

	virtual int GetDeviceID(); // hack for legacy_scsi_host_adapter::get_device

	virtual void input_sel(int state) override;
	virtual void input_ack(int state) override;
	virtual void input_rst(int state) override;
	virtual void input_data0(int state) override { if (state) m_input_data |= 0x01; else m_input_data &= ~0x01; }
	virtual void input_data1(int state) override { if (state) m_input_data |= 0x02; else m_input_data &= ~0x02; }
	virtual void input_data2(int state) override { if (state) m_input_data |= 0x04; else m_input_data &= ~0x04; }
	virtual void input_data3(int state) override { if (state) m_input_data |= 0x08; else m_input_data &= ~0x08; }
	virtual void input_data4(int state) override { if (state) m_input_data |= 0x10; else m_input_data &= ~0x10; }
	virtual void input_data5(int state) override { if (state) m_input_data |= 0x20; else m_input_data &= ~0x20; }
	virtual void input_data6(int state) override { if (state) m_input_data |= 0x40; else m_input_data &= ~0x40; }
	virtual void input_data7(int state) override { if (state) m_input_data |= 0x80; else m_input_data &= ~0x80; }

protected:
	// device-level overrides
	virtual ioport_constructor device_input_ports() const override;
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;

private:
	required_ioport m_scsi_id;
	void data_out(uint8_t data);
	void scsi_out_req_delay(uint8_t state);
	void scsi_change_phase(uint8_t newphase);
	int get_scsi_cmd_len(int cbyte);
	uint8_t scsibus_driveno(uint8_t  drivesel);
	void scsibus_read_data();
	void scsibus_write_data();
	void scsibus_exec_command();
	void dump_command_bytes();
	void dump_data_bytes(int count);
	void dump_bytes(uint8_t *buff, int count);

	emu_timer *req_timer;
	emu_timer *sel_timer;
	emu_timer *dataout_timer;

	uint8_t cmd_idx;
	uint8_t is_linked;

	uint8_t buffer[ 1024 ];
	uint16_t data_idx;
	int bytes_left;
	int data_last;

	int scsiID;
	uint8_t m_input_data;
};

extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_0)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_1)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_2)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_3)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_4)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_5)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_6)[];
extern const input_device_default DEVICE_INPUT_DEFAULTS_NAME(SCSI_ID_7)[];

#define MCFG_SCSIDEV_ADD(_tag, _option, _type, _id) \
	MCFG_DEVICE_MODIFY(_tag ) \
	MCFG_SLOT_OPTION_ADD( _option, _type ) \
	MCFG_SLOT_OPTION_DEVICE_INPUT_DEFAULTS( _option, _id ) \
	MCFG_SLOT_DEFAULT_OPTION( _option )
#endif
