// license:BSD-3-Clause
// copyright-holders:Miodrag Milanovic
/*****************************************************************************
 *
 * includes/vector06.h
 *
 ****************************************************************************/

#ifndef VECTOR06_H_
#define VECTOR06_H_

#include "emu.h"
#include "cpu/z80/z80.h"
#include "cpu/i8085/i8085.h"
#include "sound/speaker.h"
#include "sound/wave.h"
#include "sound/ay8910.h"
#include "machine/i8255.h"
#include "machine/pit8253.h"
#include "machine/ram.h"
#include "machine/wd_fdc.h"
#include "imagedev/cassette.h"
#include "imagedev/flopdrv.h"
#include "bus/generic/slot.h"
#include "bus/generic/carts.h"


class vector06_state : public driver_device
{
public:
	vector06_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu"),
		m_speaker(*this, "speaker"),
		m_cassette(*this, "cassette"),
		m_cart(*this, "cartslot"),
		m_fdc(*this, "wd1793"),
		m_floppy0(*this, "wd1793:0"),
		m_floppy1(*this, "wd1793:1"),
		m_ppi(*this, "ppi8255"),
		m_ppi2(*this, "ppi8255_2"),
		m_pit8253(*this, "pit8253"),
		m_ay(*this, "aysnd"),
		m_ram(*this, RAM_TAG),
		m_palette(*this, "palette"),
		m_bank1(*this, "bank1"),
		m_bank2(*this, "bank2"),
		m_bank3(*this, "bank3"),
		m_region_maincpu(*this, "maincpu"),
		m_line(*this, "LINE.%u", 0),
		m_reset(*this, "RESET")
	{ }

	DECLARE_FLOPPY_FORMATS(floppy_formats);

	uint8_t vector06_8255_portb_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	uint8_t vector06_8255_portc_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void vector06_8255_porta_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_8255_portb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_color_set(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t vector06_romdisk_portb_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void vector06_romdisk_portb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_romdisk_porta_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_romdisk_portc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t vector06_8255_1_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void vector06_8255_1_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t vector06_8255_2_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void vector06_8255_2_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_disc_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_status_callback(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void vector06_ramdisk_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void pit8253_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t pit8253_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void speaker_w(int state);
	void vector06_set_video_mode(int width);
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void palette_init_vector06(palette_device &palette);
	uint32_t screen_update_vector06(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void vector06_interrupt(device_t &device);
	void reset_check_callback(void *ptr, int32_t param);
	int vector06_irq_callback(device_t &device, int irqline);

private:
	required_device<cpu_device> m_maincpu;
	required_device<speaker_sound_device> m_speaker;
	required_device<cassette_image_device> m_cassette;
	required_device<generic_slot_device> m_cart;
	required_device<kr1818vg93_t> m_fdc;
	required_device<floppy_connector> m_floppy0;
	required_device<floppy_connector> m_floppy1;
	required_device<i8255_device> m_ppi;
	required_device<i8255_device> m_ppi2;
	required_device<pit8253_device> m_pit8253;
	required_device<ay8910_device> m_ay;
	required_device<ram_device> m_ram;
	required_device<palette_device> m_palette;
	required_memory_bank m_bank1;
	required_memory_bank m_bank2;
	required_memory_bank m_bank3;
	required_memory_region m_region_maincpu;
	required_ioport_array<9> m_line;
	required_ioport m_reset;

	uint8_t m_keyboard_mask;
	uint8_t m_color_index;
	uint8_t m_video_mode;
	uint8_t m_romdisk_msb;
	uint8_t m_romdisk_lsb;
	uint8_t m_vblank_state;
	uint8_t m_rambank;
	uint8_t m_aylatch;
	bool m_stack_state;
	bool m_romen;

	void update_mem();
};

#endif /* VECTOR06_H_ */
