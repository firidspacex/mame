// license:BSD-3-Clause
// copyright-holders:Ryan Holtz
#pragma once

#ifndef PIN64_H
#define PIN64_H

#include <cstdio>
#include <vector>

#include "emu.h"

class pin64_fileutil_t {
public:
	static void write(FILE* file, uint32_t data);
	static void write(FILE* file, const uint8_t* data, uint32_t size);
};

class pin64_command_t {
public:
	std::vector<uint8_t> data;
};

class pin64_data_t {
public:
	pin64_data_t()
		: m_offset(0)
		, m_old_offset(0) { }

	void reset();
	void clear();

	// setters
	virtual void put8(uint8_t data);
	virtual void put16(uint16_t data);
	virtual void put32(uint32_t data);
	virtual void put64(uint64_t data);

	// getters
	virtual uint8_t get8();
	virtual uint8_t get8(uint32_t offset, bool temp_access = false);
	virtual uint16_t get16();
	virtual uint16_t get16(uint32_t offset, bool temp_access = false);
	virtual uint32_t get32();
	virtual uint32_t get32(uint32_t offset, bool temp_access = false);
	virtual uint64_t get64();
	virtual uint64_t get64(uint32_t offset, bool temp_access = false);
	virtual uint32_t offset() { return m_offset; }
	uint8_t* bytes() { return (m_data.size() > 0) ? &m_data[0] : nullptr; }
	uint32_t size() { return m_data.size(); }

private:
	void update_offset(uint32_t offset, bool temp_access = false);

protected:
	std::vector<uint8_t> m_data;

	uint32_t m_offset;
	uint32_t m_old_offset;
};

class pin64_dummy_data_t : public pin64_data_t {
public:
	void put8(uint8_t data) override { }
	void put16(uint16_t data) override { }
	void put32(uint32_t data) override { }
	void put64(uint64_t data) override { }

	uint8_t get8() override { return 0; }
	uint8_t get8(uint32_t offset, bool update_current = true) override { return 0; }
	uint16_t get16() override  { return 0; }
	uint16_t get16(uint32_t offset, bool update_current = true) override { return 0; }
	uint32_t get32() override  { return 0; }
	uint32_t get32(uint32_t offset, bool update_current = true) override { return 0; }
	uint64_t get64() override  { return 0; }
	uint64_t get64(uint32_t offset, bool update_current = true) override { return 0; }

	uint32_t offset() override { return 0; }
};

class pin64_block_t {
public:
	pin64_block_t()
		: m_crc32{0} { }
	virtual ~pin64_block_t() { }

	void finalize();
	void clear();

	virtual void print();
	void write(FILE* file);

	// getters
	uint32_t size();
	pin64_data_t* data() { return &m_data; }
	util::crc32_t crc32() const { return m_crc32; }

protected:
	util::crc32_t m_crc32;
	pin64_data_t m_data;
};

class pin64_command_block_t : public pin64_block_t {
public:
	void print() override;
};

class pin64_t
{
public:
	pin64_t()
		: m_capture_file(nullptr)
		, m_capture_index(~0)
		, m_capture_frames(0)
		, m_current_block(nullptr)
		, m_current_cmdblock(nullptr)
		, m_playing(false)
	{ }
	~pin64_t();

	void start(int frames = 0);
	void finish();
	void clear();
	void print();

	void mark_frame(running_machine& machine);
	void play(int index);

	void command(uint64_t* cmd_data, uint32_t size);

	void data_begin();
	pin64_data_t* data_block();
	pin64_block_t& block() { return *m_blocks[m_blocks.size() - 1]; }
	void data_end();

	bool capturing() const { return m_capture_file != nullptr; }
	bool playing() const { return m_playing; }

	uint32_t size();

private:
	pin64_command_block_t* start_command_block();

	void write(FILE* file);

	uint32_t header_size();
	uint32_t blocks_size();
	uint32_t cmdblocks_size();

	void write_block_directory(FILE* file);
	void write_cmdblock_directory(FILE* file);
	void init_capture_index();

	FILE *m_capture_file;
	int32_t m_capture_index;
	int m_capture_frames;

	pin64_block_t* m_current_block;
	std::vector<pin64_block_t*> m_blocks;

	pin64_command_block_t* m_current_cmdblock;
	std::vector<pin64_command_block_t*> m_cmdblocks;

	bool m_playing;

	pin64_dummy_data_t m_dummy_data;
	static const uint8_t CAP_ID[8];
};

#endif // PIN64_H