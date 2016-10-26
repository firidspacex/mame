// license:BSD-3-Clause
// copyright-holders:David Haywood



#define GRAP2_AREA( _BASE, name ) \
	AM_RANGE(_BASE+0x000000, _BASE+0x0003ff) AM_DEVREADWRITE(name,kaneko_grap2_device,unk1_r, unk1_w ) \
	AM_RANGE(_BASE+0x000400, _BASE+0x000401) AM_DEVWRITE(name,kaneko_grap2_device, galpani3_framebuffer1_scrollx_w) \
	AM_RANGE(_BASE+0x000800, _BASE+0x000bff) AM_DEVREADWRITE(name,kaneko_grap2_device,  unk2_r, unk2_w ) \
	AM_RANGE(_BASE+0x000c00, _BASE+0x000c01) AM_DEVWRITE(name,kaneko_grap2_device, galpani3_framebuffer1_scrolly_w) \
	AM_RANGE(_BASE+0x000c02, _BASE+0x000c03) AM_DEVWRITE(name,kaneko_grap2_device,galpani3_framebuffer1_enable_w) \
	AM_RANGE(_BASE+0x000c06, _BASE+0x000c07) AM_DEVWRITE(name,kaneko_grap2_device,galpani3_framebuffer1_bgcol_w) \
	AM_RANGE(_BASE+0x000c10, _BASE+0x000c11) AM_DEVREADWRITE(name,kaneko_grap2_device, galpani3_framebuffer1_fbbright1_r, galpani3_framebuffer1_fbbright1_w ) \
	AM_RANGE(_BASE+0x000c12, _BASE+0x000c13) AM_DEVREADWRITE(name,kaneko_grap2_device, galpani3_framebuffer1_fbbright2_r, galpani3_framebuffer1_fbbright2_w ) \
	AM_RANGE(_BASE+0x000c18, _BASE+0x000c1b) AM_DEVWRITE(name,kaneko_grap2_device,galpani3_regs1_address_w) \
	AM_RANGE(_BASE+0x000c1e, _BASE+0x000c1f) AM_DEVWRITE(name,kaneko_grap2_device,galpani3_regs1_go_w) \
	AM_RANGE(_BASE+0x000c00, _BASE+0x000c1f) AM_DEVREAD(name,kaneko_grap2_device,galpani3_regs1_r) \
	AM_RANGE(_BASE+0x080000, _BASE+0x0801ff) AM_DEVREADWRITE(name,kaneko_grap2_device, pal_r, galpani3_framebuffer1_palette_w ) \
	AM_RANGE(_BASE+0x100000, _BASE+0x17ffff) AM_DEVREADWRITE(name,kaneko_grap2_device, framebuffer_r, framebuffer_w )


class kaneko_grap2_device : public device_t
{
public:
	kaneko_grap2_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);


	int m_chipnum; // used to decide where we write the palette

	static void set_chipnum(device_t &device, int chipnum);
	static void static_set_palette_tag(device_t &device, const char *tag);

	uint16_t galpani3_regs1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff);
	void galpani3_regs1_go_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);


	void gp3_do_rle(uint32_t address, uint16_t*framebuffer, uint8_t* rledata);
	void set_color_555_gp3(pen_t color, int rshift, int gshift, int bshift, uint16_t data);

	uint16_t m_framebuffer_bgcol;
	uint16_t m_framebuffer_scrolly;
	uint16_t m_framebuffer_scrollx;
	uint16_t m_framebuffer_enable;
	int m_regs1_i;

	uint16_t m_framebuffer_bright1;
	uint16_t m_framebuffer_bright2;

	uint16_t m_regs1_address_regs[0x2];

	void galpani3_framebuffer1_enable_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { m_framebuffer_enable = data; }

	void galpani3_framebuffer1_scrolly_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { m_framebuffer_scrolly = data; }
	void galpani3_framebuffer1_scrollx_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { m_framebuffer_scrollx = data; }


	uint16_t galpani3_framebuffer1_fbbright1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer_bright1; }
	uint16_t galpani3_framebuffer1_fbbright2_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer_bright2; }


	void galpani3_framebuffer1_fbbright1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_framebuffer_bright1); }
	void galpani3_framebuffer1_fbbright2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_framebuffer_bright2); }

	void galpani3_framebuffer1_bgcol_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);

	void galpani3_regs1_address_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_regs1_address_regs[offset]); }

	uint16_t framebuffer_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer[offset]; }
	void framebuffer_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_framebuffer[offset]); }
	uint16_t pal_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer_palette[offset]; }
	void galpani3_framebuffer1_palette_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	uint16_t unk1_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer_unk1[offset]; }
	void unk1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_framebuffer_unk1[offset]); }
	uint16_t unk2_r(address_space &space, offs_t offset, uint16_t mem_mask = 0xffff) { return m_framebuffer_unk2[offset]; }
	void unk2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff) { COMBINE_DATA(&m_framebuffer_unk2[offset]); }

	std::unique_ptr<uint16_t[]> m_framebuffer;
	std::unique_ptr<uint16_t[]> m_framebuffer_palette;
	std::unique_ptr<uint16_t[]> m_framebuffer_unk1;
	std::unique_ptr<uint16_t[]> m_framebuffer_unk2;




protected:
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	required_device<palette_device> m_palette;
};


extern const device_type KANEKO_GRAP2;

#define MCFG_KANEKO_GRAP2_PALETTE(_palette_tag) \
	kaneko_grap2_device::static_set_palette_tag(*device, "^" _palette_tag);
