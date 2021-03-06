// license:BSD-3-Clause
// copyright-holders:Fabio Priuli,Acho A. Tang, R. Belmont
#pragma once
#ifndef __K037122_H__
#define __K037122_H__

class k037122_device : public device_t,
						public device_video_interface,
						public device_gfx_interface
{
public:
	k037122_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
	~k037122_device() {}

	// static configuration
	static void static_set_gfx_index(device_t &device, int index) { downcast<k037122_device &>(device).m_gfx_index = index; }

	void tile_draw( screen_device &screen, bitmap_rgb32 &bitmap, const rectangle &cliprect );
	DECLARE_READ32_MEMBER( sram_r );
	DECLARE_WRITE32_MEMBER( sram_w );
	DECLARE_READ32_MEMBER( char_r );
	DECLARE_WRITE32_MEMBER( char_w );
	DECLARE_READ32_MEMBER( reg_r );
	DECLARE_WRITE32_MEMBER( reg_w );

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;

private:
	// internal state
	tilemap_t     *m_layer[2];

	std::unique_ptr<UINT32[]>       m_tile_ram;
	std::unique_ptr<UINT32[]>       m_char_ram;
	std::unique_ptr<UINT32[]>       m_reg;

	int            m_gfx_index;

	TILE_GET_INFO_MEMBER(tile_info_layer0);
	TILE_GET_INFO_MEMBER(tile_info_layer1);
	void update_palette_color( UINT32 palette_base, int color );
};

extern const device_type K037122;

#define MCFG_K037122_ADD(_tag, _screen) \
	MCFG_DEVICE_ADD(_tag, K037122, 0) \
	MCFG_VIDEO_SET_SCREEN(_screen)

#define MCFG_K037122_PALETTE(_palette_tag) \
	MCFG_GFX_PALETTE(_palette_tag)

#endif
