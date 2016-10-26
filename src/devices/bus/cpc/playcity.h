// license:BSD-3-Clause
// copyright-holders:Barry Rodewald
/*
   PlayCity expansion device

    I/O ports:
    * F880 - Z80CTC channel 0 (input is system clock (4MHz), output to YMZ294 clock)
    * F881 - Z80CTC channel 1 (input from CRTC CURSOR, output to /NMI)
    * F882 - Z80CTC channel 2 (input is system clock (4MHz), output to channel 3 input)
    * F883 - Z80CTC channel 3 (input is channel 2 output)
    * F884 - YMZ294 #1 (right) data
    * F888 - YMZ294 #2 (left) data
    * F984 - YMZ294 #1 (right) register select
    * F988 - YMZ294 #2 (left) register select
*/

#ifndef CPC_PLAYCITY_H_
#define CPC_PLAYCITY_H_


#include "emu.h"
#include "cpcexp.h"
#include "sound/ay8910.h"
#include "machine/z80ctc.h"

class cpc_playcity_device : public device_t,
				public device_cpc_expansion_card_interface
{
public:
	// construction/destruction
	cpc_playcity_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual machine_config_constructor device_mconfig_additions() const override;

	uint8_t ctc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ctc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ymz1_address_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ymz2_address_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ymz1_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void ymz2_data_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t ymz1_data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t ymz2_data_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ctc_zc1_cb(int state) { if(state) { m_slot->nmi_w(1); m_slot->nmi_w(0); } }
	void ctc_intr_cb(int state) { m_slot->irq_w(state); }

	virtual void cursor_w(int state) override { m_ctc->trg1(state); }

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	cpc_expansion_slot_device *m_slot;

	required_device<z80ctc_device> m_ctc;
	required_device<ymz294_device> m_ymz1;
	required_device<ymz294_device> m_ymz2;

	void update_ymz_clock();
};

// device type definition
extern const device_type CPC_PLAYCITY;


#endif /* CPC_PLAYCITY_H_ */
