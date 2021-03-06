// license:LGPL-2.1+
// copyright-holders:Olivier Galibert, Angelo Salese, David Haywood, Tomasz Slanina
/***************************************************************************

 Seibu Cop (Co-Processor) emulation
  (new implementation, based on Raiden 2 code)

***************************************************************************/

#ifndef RAIDEN2COP_H
#define RAIDEN2COP_H

#define LOG_Commands    0
#define LOG_Phytagoras  0
#define LOG_Division    0
#define LOG_Move0205    0
#define LOG_Move0905    0

#define MCFG_RAIDEN2COP_VIDEORAM_OUT_CB(_devcb) \
	devcb = &raiden2cop_device::set_m_videoramout_cb(*device, DEVCB_##_devcb);

#define MCFG_RAIDEN2COP_ADD(_tag ) \
	MCFG_DEVICE_ADD(_tag, RAIDEN2COP, 0)

#define MCFG_LEGIONNACOP_ADD(_tag ) \
	MCFG_DEVICE_ADD(_tag, RAIDEN2COP, 0)


class raiden2cop_device : public device_t
{
public:
	raiden2cop_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	// Command Table
	UINT16 cop_func_trigger[0x100/8];       /* function trigger */
	UINT16 cop_func_value[0x100/8];         /* function value (?) */
	UINT16 cop_func_mask[0x100/8];          /* function mask (?) */
	UINT16 cop_program[0x100];              /* program "code" */
	UINT16 cop_latch_addr, cop_latch_trigger, cop_latch_value, cop_latch_mask;

	DECLARE_WRITE16_MEMBER( cop_pgm_data_w );
	DECLARE_WRITE16_MEMBER( cop_pgm_addr_w );
	DECLARE_WRITE16_MEMBER( cop_pgm_value_w );
	DECLARE_WRITE16_MEMBER( cop_pgm_mask_w );
	DECLARE_WRITE16_MEMBER( cop_pgm_trigger_w );

	// these are used by legionna.c to find the command based on trigger value
	// legionna/seicop implementation then looks up the command based on hardcoded sequences in the driver rather than actually using the trigger value (should be changed)
	UINT16 get_func_value(int command) { return cop_func_value[command]; }
	UINT16 get_func_mask(int command) { return cop_func_mask[command]; }
	int find_trigger_match(UINT16 triggerval, UINT16 mask);
	int check_command_matches(int command, UINT16 seq0, UINT16 seq1, UINT16 seq2, UINT16 seq3, UINT16 seq4, UINT16 seq5, UINT16 seq6, UINT16 seq7, UINT16 _funcval_, UINT16 _funcmask_);

	// DMA
	UINT16 cop_dma_v1, cop_dma_v2, cop_dma_mode;
	UINT16 cop_dma_src[0x200], cop_dma_dst[0x200], cop_dma_size[0x200];
	UINT16 cop_dma_adr_rel;

	UINT16 pal_brightness_val;
	UINT16 pal_brightness_mode;

	DECLARE_WRITE16_MEMBER( cop_dma_v1_w );
	DECLARE_WRITE16_MEMBER( cop_dma_v2_w );

	DECLARE_WRITE16_MEMBER( cop_dma_adr_rel_w );
	DECLARE_WRITE16_MEMBER( cop_dma_src_w );
	DECLARE_WRITE16_MEMBER( cop_dma_size_w );
	DECLARE_WRITE16_MEMBER( cop_dma_dst_w );
	DECLARE_READ16_MEMBER( cop_dma_mode_r );
	DECLARE_WRITE16_MEMBER( cop_dma_mode_w );

	DECLARE_WRITE16_MEMBER( cop_pal_brightness_val_w );
	DECLARE_WRITE16_MEMBER( cop_pal_brightness_mode_w );

	DECLARE_WRITE16_MEMBER ( cop_dma_trigger_w );

	UINT8 fade_table(int v);

	template<class _Object> static devcb_base &set_m_videoramout_cb(device_t &device, _Object object) { return downcast<raiden2cop_device &>(device).m_videoramout_cb.set_callback(object); }

	// Number Conversion

	DECLARE_WRITE16_MEMBER( cop_itoa_low_w );
	DECLARE_WRITE16_MEMBER( cop_itoa_high_w );
	DECLARE_WRITE16_MEMBER( cop_itoa_mode_w );
	DECLARE_READ16_MEMBER ( cop_itoa_digits_r );

	UINT32 cop_itoa;
	UINT16 cop_itoa_mode;
	UINT8 cop_itoa_digits[10];

	// Main COP functionality

	DECLARE_WRITE16_MEMBER( cop_scale_w );
	DECLARE_WRITE16_MEMBER( cop_angle_target_w );
	DECLARE_WRITE16_MEMBER( cop_angle_step_w );

	DECLARE_READ16_MEMBER ( cop_reg_high_r );
	DECLARE_WRITE16_MEMBER( cop_reg_high_w );
	DECLARE_READ16_MEMBER ( cop_reg_low_r );
	DECLARE_WRITE16_MEMBER( cop_reg_low_w );

	DECLARE_WRITE16_MEMBER( cop_cmd_w );
	DECLARE_READ16_MEMBER ( cop_collision_status_r );
	DECLARE_READ16_MEMBER (cop_collision_status_val_r);
	DECLARE_READ16_MEMBER (cop_collision_status_stat_r);

	DECLARE_READ16_MEMBER ( cop_status_r );
	DECLARE_READ16_MEMBER ( cop_dist_r );
	DECLARE_READ16_MEMBER ( cop_angle_r );

	DECLARE_WRITE16_MEMBER(cop_hitbox_baseadr_w);


	UINT32 cop_regs[8];
	UINT16 cop_status, cop_scale, cop_angle, cop_dist;


	UINT16 cop_angle_target;
	UINT16 cop_angle_step;


	struct colinfo {
		colinfo()
		{
			pos[0] = pos[1] = pos[2] = 0;
			dx[0] = dx[1] = dx[2] = 0;
			size[0] = size[1] = size[2] = 0;
			allow_swap = false;
			flags_swap = 0;
			spradr = 0;
			min[0] = min[1] = min[2] = 0;
			max[0] = max[1] = max[2] = 0;

		}


		INT16 pos[3];
		INT8 dx[3];
		UINT8 size[3];
		bool allow_swap;
		UINT16 flags_swap;
		UINT32 spradr;
		INT16 min[3], max[3];


	};

	colinfo cop_collision_info[2];

	UINT16 cop_hit_status, cop_hit_baseadr;
	INT16 cop_hit_val[3];
	UINT16 cop_hit_val_stat;

	// Sort DMA (zeroteam, cupsoc)

	UINT32 cop_sort_ram_addr, cop_sort_lookup;
	UINT16 cop_sort_param;

	DECLARE_WRITE16_MEMBER(cop_sort_lookup_hi_w);
	DECLARE_WRITE16_MEMBER(cop_sort_lookup_lo_w);
	DECLARE_WRITE16_MEMBER(cop_sort_ram_addr_hi_w);
	DECLARE_WRITE16_MEMBER(cop_sort_ram_addr_lo_w);
	DECLARE_WRITE16_MEMBER(cop_sort_param_w);
	DECLARE_WRITE16_MEMBER(cop_sort_dma_trig_w);

	// RNG (cupsoc)
	UINT16 m_cop_rng_max_value;
	DECLARE_READ16_MEMBER(cop_prng_r);
	DECLARE_WRITE16_MEMBER(cop_prng_maxvalue_w);
	DECLARE_READ16_MEMBER(cop_prng_maxvalue_r);

	// misc 68k (grainbow)
	UINT32 m_cop_sprite_dma_param;
	UINT32 m_cop_sprite_dma_src;

	DECLARE_WRITE16_MEMBER(cop_sprite_dma_param_hi_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_param_lo_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_size_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_src_hi_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_src_lo_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_inc_w);
	int m_cop_sprite_dma_size;

	// misc 68k
	UINT16 m_cop_unk_param_a,m_cop_unk_param_b;
	DECLARE_WRITE16_MEMBER(cop_unk_param_a_w);
	DECLARE_WRITE16_MEMBER(cop_unk_param_b_w);

	UINT16 m_cop_rom_addr_lo,m_cop_rom_addr_hi,m_cop_precmd;
	DECLARE_WRITE16_MEMBER(cop_precmd_w);
	DECLARE_WRITE16_MEMBER(cop_rom_addr_lo_w);
	DECLARE_WRITE16_MEMBER(cop_rom_addr_hi_w);

	int m_cop_sprite_dma_abs_x,m_cop_sprite_dma_abs_y;
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_abs_y_w);
	DECLARE_WRITE16_MEMBER(cop_sprite_dma_abs_x_w);

	// legacy code, to be removed / refactored into above
	int m_LEGACY_r0, m_LEGACY_r1;
	DECLARE_WRITE16_MEMBER(LEGACY_cop_cmd_w);

	// DEBUG
	void dump_table();

protected:
	// device-level overrides
	virtual void device_start() override;

private:
	// internal state
	devcb_write16       m_videoramout_cb;
	required_device<palette_device> m_palette;

	cpu_device *m_host_cpu;      /**< reference to the host cpu */
	address_space *m_host_space; /**< reference to the host cpu space */
	bool m_host_endian;          /**< reference to the host cpu endianness, some commands cares! */
	UINT8 m_byte_endian_val;     /**< 2 if m_host_endian is big (68k) else 0 */
	UINT8 m_word_endian_val;     /**< 3 if m_host_endian is big (68k) else 0 */


	void cop_collision_read_pos(int slot, UINT32 spradr, bool allow_swap);

	// commands, TODO: needs commenting!
	void execute_8100(int offset, UINT16 data);
	void execute_8900(int offset, UINT16 data);

	void execute_42c2(int offset, UINT16 data);
	void execute_4aa0(int offset, UINT16 data);
	void execute_6200(int offset, UINT16 data);

	void execute_338e(int offset, UINT16 data);
	void execute_2288(int offset, UINT16 data);
	void execute_0205(int offset, UINT16 data);
	void execute_3b30(int offset, UINT16 data);
	void execute_130e(int offset, UINT16 data);
	void execute_0904(int offset, UINT16 data);
	void execute_2a05(int offset, UINT16 data);
	void execute_7e05(int offset, UINT16 data);
	void execute_5205(int offset, UINT16 data);
	void execute_5a05(int offset, UINT16 data);

	void execute_f205(int offset, UINT16 data);
	void execute_a100(int offset, UINT16 data);
	void execute_a900(int offset, UINT16 data);
	void execute_b100(int offset, UINT16 data);
	void execute_b900(int offset, UINT16 data);
	void execute_f105(int offset, UINT16 data);

	// TODO: remove/rename these
	//void LEGACY_execute_130e(int offset, UINT16 data);
	void LEGACY_execute_130e_cupsoc(int offset, UINT16 data);
	//void LEGACY_execute_3b30(int offset, UINT16 data);
	//void LEGACY_execute_42c2(int offset, UINT16 data);
	void LEGACY_execute_e30e(int offset, UINT16 data);
	void LEGACY_execute_6200(int offset, UINT16 data);
	void LEGACY_execute_dde5(int offset, UINT16 data);
	void LEGACY_execute_d104(int offset, UINT16 data);
	void LEGACY_execute_6980(int offset, UINT16 data);
	void LEGACY_execute_c480(int offset, UINT16 data);

	void cop_collision_update_hitbox(UINT16 data, int slot, UINT32 hitadr);
	void bcd_update();

	UINT16 cop_read_word(int address);
	UINT8 cop_read_byte(int address);
	void cop_write_word(int address, UINT16 data);
	void cop_write_byte(int address, UINT8 data);

	void dma_tilemap_buffer();
	void dma_palette_buffer();
	void dma_fill();
	void dma_palette_brightness();
	void dma_zsorting(UINT16 data);
};

extern const device_type RAIDEN2COP;

#endif  /* RAIDEN2COP_H */
