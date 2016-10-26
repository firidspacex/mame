// license:BSD-3-Clause
// copyright-holders:Olivier Galibert,Andreas Naive
#ifndef _AWBOARD_H_
#define _AWBOARD_H_

#include "naomig1.h"

#define MCFG_AW_ROM_BOARD_ADD(_tag, _keyregion, _irq_cb)  \
	MCFG_NAOMI_G1_ADD(_tag, AW_ROM_BOARD, _irq_cb)        \
	aw_rom_board::static_set_keyregion(*device, "^" _keyregion);

class aw_rom_board : public naomi_g1_device
{
public:
	aw_rom_board(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static void static_set_keyregion(device_t &device, const char *keyregion);

	DECLARE_ADDRESS_MAP(submap, 16);

	void epr_offsetl_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);          // 5f7000
	void epr_offseth_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);          // 5f7004
	void mpr_record_index_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);     // 5f700c
	void mpr_first_file_index_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff); // 5f7010
	void mpr_file_offsetl_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);     // 5f7014
	void mpr_file_offseth_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);     // 5f7018
	uint16_t pio_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);                   // 5f7080
	void pio_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);                  // 5f7080

protected:
	virtual void device_start() override;
	virtual void device_reset() override;

	virtual void dma_get_position(uint8_t *&base, uint32_t &limit, bool to_mainram) override;
	virtual void dma_advance(uint32_t size) override;

private:
	enum { EPR, MPR_RECORD, MPR_FILE };

	required_memory_region m_region;
	optional_memory_region m_keyregion;
	uint32_t rombd_key;
	uint32_t mpr_offset, mpr_bank;
	uint32_t epr_offset, mpr_file_offset;
	uint16_t mpr_record_index, mpr_first_file_index;
	uint16_t decrypted_buf[16];

	uint32_t dma_offset, dma_limit;

	struct sbox_set {
		uint8_t S0[32];
		uint8_t S1[16];
		uint8_t S2[16];
		uint8_t S3[8];
	};

	static const int permutation_table[4][16];
	static const sbox_set sboxes_table[4];
	static uint16_t decrypt(uint16_t cipherText, uint32_t address, const uint32_t key);
	uint16_t decrypt16(uint32_t address) { return decrypt(m_region->u16(address), address, rombd_key); }

	void set_key();
	void recalc_dma_offset(int mode);
};

extern const device_type AW_ROM_BOARD;

#endif
