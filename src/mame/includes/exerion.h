// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    Jaleco Exerion

*************************************************************************/


#define EXERION_MASTER_CLOCK      (XTAL_19_968MHz)   /* verified on pcb */
#define EXERION_CPU_CLOCK         (EXERION_MASTER_CLOCK / 6)
#define EXERION_AY8910_CLOCK      (EXERION_CPU_CLOCK / 2)
#define EXERION_PIXEL_CLOCK       (EXERION_MASTER_CLOCK / 3)
#define EXERION_HCOUNT_START      (0x58)
#define EXERION_HTOTAL            (512-EXERION_HCOUNT_START)
#define EXERION_HBEND             (12*8)    /* ?? */
#define EXERION_HBSTART           (52*8)    /* ?? */
#define EXERION_VTOTAL            (256)
#define EXERION_VBEND             (16)
#define EXERION_VBSTART           (240)


class exerion_state : public driver_device
{
public:
	exerion_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_main_ram(*this, "main_ram"),
		m_videoram(*this, "videoram"),
		m_spriteram(*this, "spriteram"),
		m_maincpu(*this, "maincpu"),
		m_gfxdecode(*this, "gfxdecode"),
		m_screen(*this, "screen"),
		m_palette(*this, "palette") { }

	/* memory pointers */
	required_shared_ptr<uint8_t> m_main_ram;
	required_shared_ptr<uint8_t> m_videoram;
	required_shared_ptr<uint8_t> m_spriteram;

	/* video-related */
	uint8_t    m_cocktail_flip;
	uint8_t    m_char_palette;
	uint8_t    m_sprite_palette;
	uint8_t    m_char_bank;
	std::unique_ptr<uint16_t[]>  m_background_gfx[4];
	uint8_t    *m_background_mixer;
	uint8_t    m_background_latches[13];

	/* protection? */
	uint8_t m_porta;
	uint8_t m_portb;

	/* devices */
	required_device<cpu_device> m_maincpu;
	required_device<gfxdecode_device> m_gfxdecode;
	required_device<screen_device> m_screen;
	required_device<palette_device> m_palette;

	uint8_t exerion_protection_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void exerion_videoreg_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void exerion_video_latch_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	uint8_t exerion_video_timing_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	ioport_value exerion_controls_r(ioport_field &field, void *param);
	void coin_inserted(ioport_field &field, void *param, ioport_value oldval, ioport_value newval);
	uint8_t exerion_porta_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void exerion_portb_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);
	void init_exerion();
	void init_exerionb();
	virtual void machine_start() override;
	virtual void machine_reset() override;
	virtual void video_start() override;
	void palette_init_exerion(palette_device &palette);
	uint32_t screen_update_exerion(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void draw_background( bitmap_ind16 &bitmap, const rectangle &cliprect);
};
