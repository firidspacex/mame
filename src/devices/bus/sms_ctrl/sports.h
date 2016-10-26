// license:BSD-3-Clause
// copyright-holders:Fabio Priuli
/**********************************************************************

    Sega Master System "Sports Pad" (US model) emulation

**********************************************************************/

#pragma once

#ifndef __SMS_SPORTS_PAD__
#define __SMS_SPORTS_PAD__


#include "emu.h"
#include "smsctrl.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> sms_sports_pad_device

class sms_sports_pad_device : public device_t,
							public device_sms_control_port_interface
{
public:
	// construction/destruction
	sms_sports_pad_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	// optional information overrides
	virtual ioport_constructor device_input_ports() const override;

	ioport_value rldu_pins_r(ioport_field &field, void *param); // Right, Left, Down and Up lines.
	int th_pin_r();
	void th_pin_w(int state);

protected:
	// device-level overrides
	virtual void device_start() override;

	// device_sms_control_port_interface overrides
	virtual uint8_t peripheral_r() override;
	virtual void peripheral_w(uint8_t data) override;

private:
	required_ioport m_sports_in;
	required_ioport m_sports_out;
	required_ioport m_sports_x;
	required_ioport m_sports_y;

	uint8_t m_read_state;
	uint8_t m_th_pin_state;
	uint8_t m_x_axis_reset_value;
	uint8_t m_y_axis_reset_value;
	const attotime m_interval;
	emu_timer *m_sportspad_timer;
	static const device_timer_id TIMER_SPORTSPAD = 0;

	void device_timer(emu_timer &timer, device_timer_id id, int param, void *ptr) override;
};


// device type definition
extern const device_type SMS_SPORTS_PAD;


#endif
