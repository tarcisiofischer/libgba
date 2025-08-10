#ifndef _GBA_H
#define _GBA_H

#include <types.h>

// Memory data pointers
static auto const REG_DISPCNT  = (volatile u16_t*)0x04000000;
static auto const REG_VCOUNT   = (volatile u16_t*)0x04000006;
static auto const REG_BG0CNT   = (volatile u16_t*)0x04000008;
static auto const REG_BG1CNT   = (volatile u16_t*)0x0400000A;
static auto const REG_BG2CNT   = (volatile u16_t*)0x0400000C;
static auto const REG_BG3CNT   = (volatile u16_t*)0x0400000E;
static auto const REG_BG0HOFS  = (volatile u16_t*)0x04000010;
static auto const REG_BG0VOFS  = (volatile u16_t*)0x04000012;
static auto const REG_BG1HOFS  = (volatile u16_t*)0x04000014;
static auto const REG_BG1VOFS  = (volatile u16_t*)0x04000016;
static auto const REG_BG2HOFS  = (volatile u16_t*)0x04000018;
static auto const REG_BG2VOFS  = (volatile u16_t*)0x0400001A;
static auto const REG_BG3HOFS  = (volatile u16_t*)0x0400001C;
static auto const REG_BG3VOFS  = (volatile u16_t*)0x0400001E;
static auto const SOUND1CNT_L  = (volatile u16_t*)0x04000060;
static auto const SOUND1CNT_H  = (volatile u16_t*)0x04000062;
static auto const SOUND1CNT_X  = (volatile u16_t*)0x04000064;
static auto const SOUND2CNT_L  = (volatile u16_t*)0x04000068;
static auto const SOUND2CNT_H  = (volatile u16_t*)0x0400006C;
static auto const SOUND3CNT_L  = (volatile u16_t*)0x04000070;
static auto const SOUND3CNT_H  = (volatile u16_t*)0x04000072;
static auto const SOUND3CNT_X  = (volatile u16_t*)0x04000074;
static auto const SOUND4CNT_L  = (volatile u16_t*)0x04000078;
static auto const SOUND4CNT_H  = (volatile u16_t*)0x0400007C;
static auto const SOUNDCNT_L   = (volatile u16_t*)0x04000080;
static auto const SOUNDCNT_H   = (volatile u16_t*)0x04000082;
static auto const SOUNDCNT_X   = (volatile u16_t*)0x04000084;
static auto const WAVE_RAM0_L  = (volatile u16_t*)0x04000090;
static auto const WAVE_RAM0_H  = (volatile u16_t*)0x04000092;
static auto const FIFO_A_L     = (volatile u16_t*)0x040000A0;
static auto const FIFO_A_H     = (volatile u16_t*)0x040000A2;
static auto const REG_KEYINPUT = (volatile u16_t*)0x04000130;
static auto const BG_PALETTE   = (volatile u16_t*)0x05000000;
static auto const OBJ_PALETTE  = (volatile u16_t*)0x05000200;
static auto const OBJ_PALETTE1 = (volatile u16_t*)0x05000200;
static auto const OBJ_PALETTE2 = (volatile u16_t*)0x05000220;
static auto const OBJ_PALETTE3 = (volatile u16_t*)0x05000240;
static auto const OBJ_PALETTE4 = (volatile u16_t*)0x05000260;
static auto const PALETTE_END  = (volatile u16_t*)0x05000400;
// Note: VRAM can have different semantics depending on the configurations.
static auto const VRAM_PTR     = (volatile u16_t*)0x06000000;
static auto const OBJ_TILES    = (volatile u16_t*)0x06010000;
static auto const OAM          = (volatile u16_t*)0x07000000;

// Video modes
static auto constexpr MODE_0     = (u16_t)(0 << 0);
static auto constexpr BG0_ENABLE = (u16_t)(1 << 8);
static auto constexpr BG1_ENABLE = (u16_t)(1 << 9);
static auto constexpr BG2_ENABLE = (u16_t)(1 << 10);
static auto constexpr BG3_ENABLE = (u16_t)(1 << 11);
static auto constexpr OBJ_ENABLE = (u16_t)(1 << 12);

// Controller values
static auto constexpr BTN_A      = (1 << 0);
static auto constexpr BTN_B      = (1 << 1);
static auto constexpr BTN_SELECT = (1 << 2);
static auto constexpr BTN_START  = (1 << 3);
static auto constexpr BTN_RIGHT  = (1 << 4);
static auto constexpr BTN_LEFT   = (1 << 5);
static auto constexpr BTN_UP     = (1 << 6);
static auto constexpr BTN_DOWN   = (1 << 7);
static auto constexpr BTN_R      = (1 << 8);
static auto constexpr BTN_L      = (1 << 9);

struct OAM_attr {
  u16_t attr0;
  u16_t attr1;
  u16_t attr2;
  u16_t _unused;

  // Size  Square   Horizontal  Vertical
  // 0     8x8      16x8        8x16
  // 1     16x16    32x8        8x32
  // 2     32x32    32x16       16x32
  // 3     64x64    64x32       32x64
  enum class ObjectSize {
    _8x8 = 0,
    _16x16 = 1,
    _32x32 = 2,
    _64x64 = 3
  };

  static volatile OAM_attr* get_obj(u16_t obj_id) {
    return reinterpret_cast<volatile OAM_attr*>(OAM + obj_id * sizeof(OAM_attr));
  }
  inline void set_x(u16_t v) volatile {
    attr1 &= ~0x1ff;
    attr1 |= v & 0x1ff;
  }
  inline void set_y(u8_t v) volatile {
    attr0 &= ~0xff;
    attr0 |= v & 0xff;
  }
  inline void set_size(ObjectSize size) volatile {
    attr1 &= ~(0b11 << 14);
    attr1 |= (static_cast<u16_t>(size) << 14);
  }
  static constexpr u16_t step16x16(u16_t sprite_id) { return 4 * sprite_id; }
  inline void set_sprite(u16_t sprite_id) volatile {
    attr2 &= ~0x1ff;
    attr2 |= (sprite_id & 0x1ff);
  }
  inline void set_hflip(bool v) volatile {
    attr1 &= ~(1 << 12);
    attr1 |= (v << 12);
  }
  inline void set_palette(u16_t p) volatile {
    attr2 &= ~(0b1111 << 12);
    attr2 |= (p & 0b1111) << 12;
  }
};

struct _VRAM_t {
  inline volatile u16_t* at(int x, int y) { return &(VRAM_PTR[x + y*240]); }
};
static _VRAM_t VRAM;

void vid_vsync()
{
  while(*REG_VCOUNT >= 160);
  while(*REG_VCOUNT < 160);
}

// Controller
struct Controller {
  static inline bool is_pressed(u16_t btn) { return ~(*REG_KEYINPUT) & btn; }
  static inline bool is_released(u16_t btn) { return !Controller::is_pressed(btn); }

  inline bool is_just_pressed(u16_t btn) {
    return this->_is_just_pressed;
  }

  inline bool is_just_released(u16_t btn) {
    return this->_is_just_released;
  }

  void update() {
    this->_is_just_pressed = this->_pREG_KEYINPUT & ~*REG_KEYINPUT;
    this->_is_just_released = ~this->_pREG_KEYINPUT & *REG_KEYINPUT;
    this->_pREG_KEYINPUT = *REG_KEYINPUT;
  }

private:
  // Compare with REG_KEYINPUT to find out if the key has been just pressed
  u16_t _pREG_KEYINPUT = 0xffff;
  u16_t _is_just_pressed = 0x0000;
  u16_t _is_just_released = 0x0000;
};

// Sound
static const u16_t _ = 0;
static const u16_t C2 = 32;
static const u16_t Cs2 = 149;
static const u16_t D2 = 253;
static const u16_t Eb2 = 346;
static const u16_t E2 = 450;
static const u16_t F2 = 542;
static const u16_t Fs2 = 632;
static const u16_t G2 = 711;
static const u16_t Gs2 = 786;
static const u16_t A2 = 857;
static const u16_t Bb2 = 923;
static const u16_t B2 = 983;
static const u16_t C3 = 1040;
static const u16_t Cs3 = 1103;
static const u16_t D3 = 1151;
static const u16_t Eb3 = 1206;
static const u16_t E3 = 1249;
static const u16_t F3 = 1295;
static const u16_t Fs3 = 1340;
static const u16_t G3 = 1380;
static const u16_t Gs3 = 1417;
static const u16_t A3 = 1453;
static const u16_t Bb3 = 1486;
static const u16_t B3 = 1516;
static const u16_t C4 = 1546;
static const u16_t Cs4 = 1576;
static const u16_t D4 = 1601;
static const u16_t Eb4 = 1627;
static const u16_t E4 = 1650;
static const u16_t F4 = 1673;
static const u16_t Fs4 = 1694;
static const u16_t G4 = 1714;
static const u16_t Gs4 = 1733;
static const u16_t A4 = 1751;
static const u16_t Bb4 = 1767;
static const u16_t B4 = 1783;
static const u16_t C5 = 1798;
static const u16_t Cs5 = 1812;
static const u16_t D5 = 1825;
static const u16_t Eb5 = 1838;
static const u16_t E5 = 1850;
static const u16_t F5 = 1861;
static const u16_t Fs5 = 1871;
static const u16_t G5 = 1881;
static const u16_t Gs5 = 1891;
static const u16_t A5 = 1900;
static const u16_t Bb5 = 1908;
static const u16_t B5 = 1916;

struct Sound {
  Sound(u16_t* song, u16_t song_size, u16_t tempo = 8)
    : _song_ch1(song)
    , _song_size(song_size)
    , _tempo(tempo)
  {}

  Sound(u16_t* song_ch1, u16_t* song_ch2, u16_t song_size, u16_t tempo = 8)
    : _song_ch1(song_ch1)
    , _song_ch2(song_ch2)
    , _song_size(song_size)
    , _tempo(tempo)
  {}

  static void init() {
    // Setup. Note: SOUNDCNT_X _must_ be initialized first, or else the other
    // configs will be ignored.
    *SOUNDCNT_X = (0b1 << 7); // Master enable
    *SOUNDCNT_L =
      (0b111  <<  0)  | // Master sound volume RIGHT
      (0b111  <<  4)  | // Master sound volume LEFT
      (0b0011 <<  8) | // Sound channels enable RIGHT
      (0b0011 << 12)  // Sound channels enable LEFT
    ;
  }

  static constexpr u16_t TypeA = 
    (0b100000 <<  0) | // Sound length; units of (64-n)/256s  (0-63)
    (0b00     <<  6) | // Wave pattern duty (2 = 50% = normal)
    (0b111    <<  8) | // Envelope Step-Time; units of n/64s
    (0b1      << 11)   // Envelope Direction
  ;
  static constexpr u16_t TypeB = 
    (0b001000 <<  0) | // Sound length; units of (64-n)/256s  (0-63)
    (0b11     <<  6) | // Wave pattern duty
    (0b110    <<  8) | // Envelope Step-Time; units of n/64s
    (0b1      << 11)   // Envelope Direction
  ;
  static constexpr u16_t TypeC = 
    (0b011111 <<  0) | // Sound length; units of (64-n)/256s  (0-63)
    (0b00     <<  6) | // Wave pattern duty
    (0b001    <<  8) | // Envelope Step-Time; units of n/64s
    (0b1      << 11)   // Envelope Direction
  ;
  static constexpr u16_t TypeD = 
    (0b000000 <<  0) | // Sound length; units of (64-n)/256s  (0-63)
    (0b00     <<  6) | // Wave pattern duty (2 = 50% = normal)
    (0b001    <<  8) | // Envelope Step-Time; units of n/64s
    (0b0      << 11)   // Envelope Direction
  ;
  void config_ch1_type(u16_t type) { this->_ch1_type = type; }
  void config_ch2_type(u16_t type) { this->_ch2_type = type; }
  void config_ch1_volume(u16_t volume) { this->_ch1_volume = volume & 0b1111; }
  void config_ch2_volume(u16_t volume) { this->_ch2_volume = volume & 0b1111; }

  void update() {
    if (this->_is_paused) {
      return;
    }

    this->_frame_counter++;
    if (this->_frame_counter != this->_tempo) {
      return;
    }
    this->_frame_counter = 0;

    if (this->_song_ch1[this->_song_pos] != _) {
      *SOUND1CNT_H = this->_ch1_type | (this->_ch1_volume << 12);
      *SOUND1CNT_X = 0b1100'0000'0000'0000 | this->_song_ch1[this->_song_pos];
    }
    if (this->_song_ch2 && this->_song_ch2[this->_song_pos] != _) {
      *SOUND2CNT_H = this->_ch2_type | (this->_ch2_volume << 12);
      *SOUND2CNT_L = 0b1100'0000'0000'0000 | this->_song_ch2[this->_song_pos];
    }
    this->_song_pos++;
    if (this->_song_pos > this->_song_size) {
      this->_song_pos = 0;
      if (this->_stop_on_finish) {
        this->stop();
      }
    }
  }

  void set_stop_on_finish(bool v) {
    this->_stop_on_finish = v;
  }

  void pause() {
    this->_is_paused = true;
  }

  void reset() {
    this->_song_pos = 0;
  }

  void stop() {
    this->_is_paused = true;
    this->_song_pos = 0;
  }

  bool is_paused() { return this->_is_paused; }
  void play() {
    this->_frame_counter = 0;
    this->_is_paused = false;
  }

private:
  u16_t _frame_counter = 0;
  u16_t _tempo = 8;
  u16_t _song_pos = 0;
  u16_t* _song_ch1 = nullptr;
  u16_t* _song_ch2 = nullptr;
  u16_t _song_size = 0;
  bool _is_paused = false;
  u16_t _ch1_type = Sound::TypeA;
  u16_t _ch2_type = Sound::TypeA;
  u16_t _ch1_volume = 0b1111;
  u16_t _ch2_volume = 0b1000;
  bool _stop_on_finish = false;
};

#endif

