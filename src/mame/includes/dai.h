// license:BSD-3-Clause
// copyright-holders:Krzysztof Strzecha, Nathan Woods
/*****************************************************************************
 *
 * includes/dai.h
 *
 ****************************************************************************/

#ifndef DAI_H_
#define DAI_H_

#include "cpu/i8085/i8085.h"
#include "audio/dai_snd.h"
#include "machine/i8255.h"
#include "machine/pit8253.h"
#include "machine/ram.h"
#include "machine/tms5501.h"
#include "imagedev/cassette.h"
#include "sound/wave.h"


class dai_state : public driver_device
{
public:
	enum
	{
		TIMER_BOOTSTRAP,
		TIMER_TMS5501
	};

	dai_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_pit(*this, "pit8253"),
		m_tms5501(*this, "tms5501"),
		m_sound(*this, "custom"),
		m_cassette(*this, "cassette"),
		m_ram(*this, RAM_TAG),
		m_palette(*this, "palette")  { }

	required_device<cpu_device> m_maincpu;
	required_device<pit8253_device> m_pit;
	required_device<tms5501_device> m_tms5501;
	required_device<dai_sound_device> m_sound;
	required_device<cassette_image_device> m_cassette;
	required_device<ram_device> m_ram;
	required_device<palette_device> m_palette;

	uint8_t m_paddle_select;
	uint8_t m_paddle_enable;
	uint8_t m_cassette_motor[2];
	uint8_t m_keyboard_scan_mask;
	unsigned short m_4_colours_palette[4];
	void dai_stack_interrupt_circuit_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t dai_io_discrete_devices_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dai_io_discrete_devices_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t dai_amd9511_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dai_amd9511_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t dai_pit_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dai_pit_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t dai_keyboard_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dai_keyboard_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void palette_init_dai(palette_device &palette);
	uint32_t screen_update_dai(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void dai_update_memory(int dai_rom_bank);
	int int_ack(device_t &device, int irqline);

protected:
	virtual void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};


/*----------- defined in video/dai.c -----------*/

extern const unsigned char dai_palette[16*3];


#endif /* DAI_H_ */
