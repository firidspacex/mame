// license:BSD-3-Clause
// copyright-holders:Nicola Salmoria
#include "emu.h"

#include "includes/crshrace.h"


/***************************************************************************

  Callbacks for the TileMap code

***************************************************************************/

void crshrace_state::get_tile_info1(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index)
{
	int code = m_videoram1[tile_index];

	SET_TILE_INFO_MEMBER(1, (code & 0xfff) + (m_roz_bank << 12), code >> 12, 0);
}

void crshrace_state::get_tile_info2(tilemap_t &tilemap, tile_data &tileinfo, tilemap_memory_index tile_index)
{
	int code = m_videoram2[tile_index];

	SET_TILE_INFO_MEMBER(0, code, 0, 0);
}


/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/


uint32_t crshrace_state::crshrace_tile_callback( uint32_t code )
{
	return m_spriteram2->buffer()[code&0x7fff];
}


void crshrace_state::video_start()
{
	m_tilemap1 = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(crshrace_state::get_tile_info1),this), TILEMAP_SCAN_ROWS, 16, 16, 64, 64);
	m_tilemap2 = &machine().tilemap().create(*m_gfxdecode, tilemap_get_info_delegate(FUNC(crshrace_state::get_tile_info2),this), TILEMAP_SCAN_ROWS, 8, 8, 64, 64);

	m_tilemap1->set_transparent_pen(0x0f);
	m_tilemap2->set_transparent_pen(0xff);

}


/***************************************************************************

  Memory handlers

***************************************************************************/

void crshrace_state::crshrace_videoram1_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask)
{
	COMBINE_DATA(&m_videoram1[offset]);
	m_tilemap1->mark_tile_dirty(offset);
}

void crshrace_state::crshrace_videoram2_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask)
{
	COMBINE_DATA(&m_videoram2[offset]);
	m_tilemap2->mark_tile_dirty(offset);
}

void crshrace_state::crshrace_roz_bank_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask)
{
	if (ACCESSING_BITS_0_7)
	{
		if (m_roz_bank != (data & 0xff))
		{
			m_roz_bank = data & 0xff;
			m_tilemap1->mark_all_dirty();
		}
	}
}


void crshrace_state::crshrace_gfxctrl_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask)
{
	if (ACCESSING_BITS_0_7)
	{
		m_gfxctrl = data & 0xdf;
		m_flipscreen = data & 0x20;
	}
}


/***************************************************************************

  Display refresh

***************************************************************************/

void crshrace_state::draw_bg( screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
	m_tilemap2->draw(screen, bitmap, cliprect, 0, 0);
}


void crshrace_state::draw_fg(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	m_k053936->zoom_draw(screen, bitmap, cliprect, m_tilemap1, 0, 0, 1);
}


uint32_t crshrace_state::screen_update_crshrace(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	if (m_gfxctrl & 0x04)   /* display disable? */
	{
		bitmap.fill(m_palette->black_pen(), cliprect);
		return 0;
	}

	bitmap.fill(0x1ff, cliprect);



	switch (m_gfxctrl & 0xfb)
	{
		case 0x00:  /* high score screen */
			m_spr->draw_sprites(m_spriteram->buffer(), 0x2000,  screen, bitmap, cliprect);
			draw_bg(screen, bitmap, cliprect);
			draw_fg(screen, bitmap, cliprect);
			break;
		case 0x01:
		case 0x02:
			draw_bg(screen, bitmap, cliprect);
			draw_fg(screen, bitmap, cliprect);
			m_spr->draw_sprites(m_spriteram->buffer(), 0x2000,  screen, bitmap, cliprect);
			break;
		default:
			popmessage("gfxctrl = %02x", m_gfxctrl);
			break;
	}
	return 0;
}

void crshrace_state::screen_eof_crshrace(screen_device &screen, bool state)
{
	m_spriteram->vblank_copy_rising(screen, state);
	m_spriteram2->vblank_copy_rising(screen, state);
}
