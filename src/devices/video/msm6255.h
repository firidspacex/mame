// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    OKI MSM6255 Dot Matrix LCD Controller implementation

**********************************************************************/

#pragma once

#ifndef __MSM6255__
#define __MSM6255__

#include "emu.h"



///*************************************************************************
//  TYPE DEFINITIONS
///*************************************************************************

// ======================> msm6255_device

class msm6255_device :  public device_t,
						public device_memory_interface,
						public device_video_interface
{
public:
	// construction/destruction
	msm6255_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual DECLARE_ADDRESS_MAP(map, 8);

	uint8_t ir_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void ir_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint8_t dr_r(address_space &space, offs_t offset, uint8_t mem_mask = 0xff);
	void dr_w(address_space &space, offs_t offset, uint8_t data, uint8_t mem_mask = 0xff);

	uint32_t screen_update(screen_device &device, bitmap_ind16 &bitmap, const rectangle &cliprect);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

	// device_memory_interface overrides
	virtual const address_space_config *memory_space_config(address_spacenum spacenum = AS_0) const override;

private:
	// registers
	enum
	{
		REGISTER_MOR = 0,
		REGISTER_PR,
		REGISTER_HNR,
		REGISTER_DVR,
		REGISTER_CPR,
		REGISTER_SLR,
		REGISTER_SUR,
		REGISTER_CLR,
		REGISTER_CUR
	};

	uint8_t read_byte(uint16_t ma, uint8_t ra);

	void update_cursor();
	void draw_scanline(bitmap_ind16 &bitmap, const rectangle &cliprect, int y, uint16_t ma, uint8_t ra = 0);
	void update_graphics(bitmap_ind16 &bitmap, const rectangle &cliprect);
	void update_text(bitmap_ind16 &bitmap, const rectangle &cliprect);

	const address_space_config m_space_config;

	uint8_t m_ir;                     // instruction register
	uint8_t m_mor;                    // mode control register
	uint8_t m_pr;                     // character pitch register
	uint8_t m_hnr;                    // horizontal character number register
	uint8_t m_dvr;                    // duty number register
	uint8_t m_cpr;                    // cursor form register
	uint8_t m_slr;                    // start address (lower) register
	uint8_t m_sur;                    // start address (upper) register
	uint8_t m_clr;                    // cursor address (lower) register
	uint8_t m_cur;                    // cursor address (upper) register

	int m_cursor;                   // is cursor displayed
	int m_frame;                    // frame counter
};


// device type definition
extern const device_type MSM6255;



#endif
