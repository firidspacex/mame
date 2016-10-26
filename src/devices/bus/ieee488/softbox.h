// license:BSD-3-Clause
// copyright-holders:Curt Coder, Mike Naberezny
/**********************************************************************

    SSE SoftBox emulation

**********************************************************************/

#pragma once

#ifndef __PET_SOFTBOX__
#define __PET_SOFTBOX__

#include "ieee488.h"
#include "bus/rs232/rs232.h"
#include "cpu/z80/z80.h"
#include "imagedev/harddriv.h"
#include "machine/corvushd.h"
#include "machine/com8116.h"
#include "machine/i8251.h"
#include "machine/i8255.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> softbox_device

class softbox_device :  public device_t,
						public device_ieee488_interface
{
public:
	// construction/destruction
	softbox_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual const tiny_rom_entry *device_rom_region() const override;
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual ioport_constructor device_input_ports() const override;

	void dbrg_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t ppi0_pa_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ppi0_pb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t ppi1_pa_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ppi1_pb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t ppi1_pc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ppi1_pc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset_after_children() override;

	// device_ieee488_interface overrides
	virtual void ieee488_ifc(int state) override;

private:
	enum
	{
		LED_A,
		LED_B,
		LED_READY
	};

	required_device<cpu_device> m_maincpu;
	required_device<com8116_device> m_dbrg;
	required_device<corvus_hdc_t> m_hdc;

	int m_ifc;  // Tracks previous state of IEEE-488 IFC line
};


// device type definition
extern const device_type SOFTBOX;



#endif
