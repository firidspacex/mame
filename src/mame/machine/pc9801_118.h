// license:BSD-3-Clause
// copyright-holders:Angelo Salese
/***************************************************************************

    NEC PC-9801-118

***************************************************************************/


#pragma once

#ifndef __PC9801_118DEV_H__
#define __PC9801_118DEV_H__

#include "machine/pic8259.h"
#include "sound/2608intf.h"


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> pc9801_118_device

class pc9801_118_device : public device_t
{
public:
	// construction/destruction
	pc9801_118_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual machine_config_constructor device_mconfig_additions() const override;
	virtual ioport_constructor device_input_ports() const override;

	uint8_t opn_porta_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void opn_portb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t pc9801_118_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void pc9801_118_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t pc9801_118_ext_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void pc9801_118_ext_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pc9801_sound_irq(int state);

//  required_device<cpu_device>  m_maincpu;
	required_device<ym2608_device>  m_opn3;
	virtual const tiny_rom_entry *device_rom_region() const override;
protected:
	// device-level overrides
	virtual void device_validity_check(validity_checker &valid) const override;
	virtual void device_start() override;
	virtual void device_reset() override;
	void install_device(offs_t start, offs_t end, read8_delegate rhandler, write8_delegate whandler);

private:
	uint8_t m_joy_sel;
	uint8_t m_ext_reg;
};


// device type definition
extern const device_type PC9801_118;



//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************



#endif
