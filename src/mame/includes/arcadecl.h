// license:BSD-3-Clause
// copyright-holders:Aaron Giles
/*************************************************************************

    Atari Arcade Classics hardware (prototypes)

*************************************************************************/

#include "machine/atarigen.h"
#include "video/atarimo.h"
#include "sound/okim6295.h"

class arcadecl_state : public atarigen_state
{
public:
	arcadecl_state(const machine_config &mconfig, device_type type, const char *tag)
		: atarigen_state(mconfig, type, tag),
			m_oki(*this, "oki"),
			m_mob(*this, "mob"),
			m_bitmap(*this, "bitmap") { }

	required_device<okim6295_device> m_oki;
	optional_device<atari_motion_objects_device> m_mob;
	required_shared_ptr<uint16_t> m_bitmap;

	virtual void update_interrupts() override;
	virtual void scanline_update(screen_device &screen, int scanline) override;
	void latch_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask = 0xffff);
	void init_sparkz();
	void machine_start_arcadecl();
	void machine_reset_arcadecl();
	void video_start_arcadecl();
	uint32_t screen_update_arcadecl(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void arcadecl_bitmap_render(bitmap_ind16 &bitmap, const rectangle &cliprect);

	static const atari_motion_objects_config s_mob_config;
};
