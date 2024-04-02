/*
  jflp.c - Patch Freelancer to remove some FLSpew warnings.

  Jason Hood, 31 May, 2009.

  This patch removes the following spew warnings:
  * couldnt find material 163618903
    Generated when a waypoint is selected.  I was unable to fix it, so now I
    just remove the warning.
  * get_zone(N) failed
    Generated when a job sector has no name, which is quite frequently.
  * Disconnecting equipment with FATE_UNKNOWN, using FATE_DISAPPEAR
    Generated when a wreck drops its weapons.  This is a bug in Freelancer.exe,
    since it doesn't detect the "fate_loot" type.
  * Used Hostile Pick Assistance
    Not sure exactly when this is generated (or even why), but it's something
    to do with targetting enemy fighters.

  2 June, 2009:
  * replace the line number of the "Deform::start_aim" notice with the
    timestamp of the event (in milliseconds; it also replaces the string with
    "(null)", in order to fit the new code).

  26 & 27 August, 2009:
  * add proper stat displays for Powerplants, Engines, Scanners, Tractor Beams
    and Armor.	Not used by the vanilla game, but most mods display them.
    Also add a blank line after "Stats".

  7 to 9 September, 2009:
  * provide widescreen support.

  Build (VC 6):
    rc jflp.rc
    cl /nologo /W3 /Gf /Ox /MD /LD /Fepath\to\freelancer\exe\jflp.dll jflp.c jflp.res
*/


#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <math.h>

#pragma comment( lib, "user32.lib" )

#define PI 3.14159265358979

#define NAKED __declspec(naked)


void Stats_Hook( void );

const DWORD stats_jump_table[] = {
   (DWORD)Stats_Hook,	// Power
   (DWORD)Stats_Hook,	// Engine
   (DWORD)Stats_Hook,	// Scanner
   (DWORD)Stats_Hook,	// Tractor
   (DWORD)Stats_Hook,	// Armor
   0x484EE5,		// ShieldGenerator
   0x484DA9,		// Thruster
   0x484377,		// Gun
   0x4840E3,		// MineDropper
   0x48531E,		// CounterMeasureDropper
   0x484C9C,		// RepairKit
   0x484D23,		// ShieldBattery
   0x484A6B,		// Munition & Mine
   0x4851E5,		// CounterMeasure
   0x485495,		// default
};

const BYTE stats_jump_index[] = {
   0,	// Power
   1,	// Engine
  14,	// Shield
   5,	// ShieldGenerator
   6,	// Thruster
  14,	// Launcher
   7,	// Gun
   8,	// MineDropper
   9,	// CounterMeasureDropper
   2,	// Scanner
  14,	// Light
   3,	// Tractor
  14,	// AttachedFXEquip
  14,	// InternalFXEquip
  14,	// RepairDroid
  10,	// RepairKit
  11,	// ShieldBattery
  14,	// CloakingDevice
  14,	// TradeLaneEquip
  14,	// Projectile
  12,	// Munition
  12,	// Mine
  13,	// CounterMeasure
   4	// Armor
};

const BYTE stats_bool_index[] = {
   0,	// Power
   0,	// Engine
   1,	// Shield
   0,	// ShieldGenerator
   0,	// Thruster
   1,	// Launcher
   0,	// Gun
   0,	// MineDropper
   0,	// CounterMeasureDropper
   0,	// Scanner
   1,	// Light
   0,	// Tractor
   1,	// AttachedFXEquip
   1,	// InternalFXEquip
   1,	// RepairDroid
   0,	// RepairKit
   0,	// ShieldBattery
   1,	// CloakingDevice
   1,	// TradeLaneEquip
   1,	// Projectile
   0,	// Munition
   0,	// Mine
   0,	// CounterMeasure
   0	// Armor
};


UINT (*GetString)( LPVOID rsrc, UINT res, LPWSTR buf, UINT len )
		   = (LPVOID)0x4347e0;

UINT __stdcall Stats( int idx, LPBYTE equip, BYTE name, BYTE value )
{
  LPWSTR buf  = (LPWSTR)0x66fc60;
  UINT	 len  = 0;
  LPVOID rsrc = *(LPVOID*)0x67eca8;

  switch (idx)
  {
    case 0: // Power
    {
      float capacity, charge_rate, thrust_capacity, thrust_charge_rate;
      if (value)
      {
	capacity	   = *(float*)(equip+0x70);
	charge_rate	   = *(float*)(equip+0x74);
	thrust_capacity    = *(float*)(equip+0x78);
	thrust_charge_rate = *(float*)(equip+0x7c);
      }
      if (name)
	len = GetString( rsrc, 1758, buf, 128 );	// Energy Capacity:
      if (value)
	len += swprintf( buf+len, L"%d", (int)capacity );
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1753, buf+len, 128 );	// Regeneration Rate:
      if (value)
	len += swprintf( buf+len, L"%d", (int)charge_rate );
      buf[len++] = '\n';
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1698, buf+len, 128 );	// Thrust Capacity:
      if (value)
	len += swprintf( buf+len, L"%d", (int)thrust_capacity );
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1753, buf+len, 128 );	// Regeneration Rate:
      if (value)
	len += swprintf( buf+len, L"%d", (int)thrust_charge_rate );
    }
    break;
    case 1: // Engine
    {
      float max_force, linear_drag, power_usage, reverse_fraction;
      float cruise_charge_time, cruise_power_usage;
      if (value)
      {
	max_force	   = *(float*)(equip+0x70);
	linear_drag	   = *(float*)(equip+0x74);
	power_usage	   = *(float*)(equip+0x78);
	cruise_power_usage = *(float*)(equip+0x7c);
	cruise_charge_time = *(float*)(equip+0x80);
	reverse_fraction   = *(float*)(equip+0x88);
      }
      if (name)
	len = GetString( rsrc, 945, buf, 128 ); 	// Top Speed:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)(max_force / linear_drag) );
	len += GetString( rsrc, 1760, buf+len, 128 );	// m/s
      }
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1699, buf+len, 128 );	// Reverse Speed:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)(max_force * reverse_fraction
					       / linear_drag) );
	len += GetString( rsrc, 1760, buf+len, 128 );	// m/s
      }
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1755, buf+len, 128 );	// Energy Drain:
      if (value)
	len += swprintf( buf+len, L"%d", (int)power_usage );
      buf[len++] = '\n';
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1762, buf+len, 128 );	// Cruise Charge Time:
      if (value)
      {
	len += swprintf( buf+len, L"%.2f", cruise_charge_time );
	len += GetString( rsrc, 1764, buf+len, 128 );	// s
      }
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1763, buf+len, 128 );	// Cruise Power Drain:
      if (value)
	len += swprintf( buf+len, L"%d", (int)cruise_power_usage );
    }
    break;
    case 2: // Scanner
    {
      float range, cargo_scan_range;
      if (value)
      {
	range		 = *(float*)(equip+0x74);
	cargo_scan_range = *(float*)(equip+0x78);
      }
      if (name)
	len = GetString( rsrc, 1745, buf, 128 );	// Range:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)range );
	len += GetString( rsrc, 1759, buf+len, 128 );	// m
      }
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1697, buf+len, 128 );	// Cargo Scan Range:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)cargo_scan_range );
	len += GetString( rsrc, 1759, buf+len, 128 );	// m
      }
    }
    break;
    case 3: // Tractor
    {
      float max_length, reach_speed;
      if (value)
      {
	max_length  = *(float*)(equip+0x70);
	reach_speed = *(float*)(equip+0x74);
      }
      if (name)
	len = GetString( rsrc, 1745, buf, 128 );	// Range:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)max_length );
	len += GetString( rsrc, 1759, buf+len, 128 );	// m
      }
      buf[len++] = '\n';
      if (name)
	len += GetString( rsrc, 1696, buf+len, 128 );	// Extend Speed:
      if (value)
      {
	len += swprintf( buf+len, L"%d", (int)reach_speed );
	len += GetString( rsrc, 1760, buf+len, 128 );	// m/s
      }
    }
    break;
    case 4: // Armor
    {
      float armor;
      if (value)
	armor = *(float*)(equip+0x70);
      if (name)
	len = GetString( rsrc, 1695, buf, 128 );	// Armor Improvement:
      if (value)
	len += swprintf( buf+len, L"%.0f%%", (armor - 1) * 100 );
    }
    break;
  }

  return len;
}


NAKED
void Stats_Hook( void )
{
  __asm push	dword ptr [esp+0x22c]
  __asm push	dword ptr [esp+0x22c]
  __asm push	edi
  __asm push	eax
  __asm call	Stats
  __asm push	0
  __asm push	dword ptr [esp+0x224]
  __asm push	0x4854c0
  __asm ret
}


#define WIDTH	((PDWORD)0x679bc8)
#define HEIGHT	((PDWORD)0x679bcc)

float ratio;
DWORD width43;

// Given the 4:3 value of fovx, determine the value for the current resolution.
float calc_fovx( float deg )
{
  return (float)(atan( ratio / 2
		       / (4.0f/3 / 2
			  / tan( deg * PI / 180 ))) * 180 / PI);
}


// Called when the resolution is set.
void Resolution( void )
{
  #define FOVX_WIN    ((float*)0x59d5fd)
  #define BAR_WIDTH   ((double*)0x5d7e58)

  ratio = (float)*WIDTH / *HEIGHT;
  *FOVX_WIN = calc_fovx( 27.216f );	// set the default value for WinCamera
  *BAR_WIDTH = 4.0f/3 / 8 / ratio;	// set the width of the gauges

  width43 = *HEIGHT * 4 / 3;
}


NAKED
void Resolution_Hook( void )
{
  __asm call	Resolution
  __asm push	0x424510
  __asm ret
}


// Called when the fovx value is read from cameras.ini.
float __stdcall Fovx( BOOL WinCamera, float fovx )
{
  if (!fovx)
    fovx = calc_fovx( (WinCamera) ? 27.216f : 35.0f );
  return fovx;
}


NAKED
void Fovx_Hook( void )
{
  __asm xor	ebx, ebx
  __asm cmp	esi, 0x67dbf8		// WinCamera
  __asm sete	bl
  __asm fstp	dword ptr [esp]
  __asm push	ebx
  __asm call	Fovx
  __asm xor	ebx, ebx
  __asm push	0x40f628
  __asm ret
}


// The Nav Map seems to want the original 4:3 WinCamera value.

float old_val;

NAKED
void NavMap_On( void )
{
  __asm mov	eax, ds:[0x67dc90]
  __asm mov	dword ptr old_val, eax
  __asm mov	dword ptr ds:[0x67dc90], 0x41d9ba5e	// 27.216
  __asm push	0x491100
  __asm ret
}

NAKED
void NavMap_Off( void )
{
  __asm mov	eax, dword ptr old_val
  __asm mov	ds:[0x67dc90], eax
  __asm push	0x59dd00
  __asm ret
}


// Use the desktop resolution as the default and replace 800x600 with it.
void Patch_Dimensions( void )
{
  #define ADDR_DEFAULT ((PBYTE)0x56223f)
  #define ADDR_LIST    ((PBYTE)0x4a9835)

  DWORD dummy;
  RECT	desktop;
  PBYTE addr;

  static int res_width[]  = { 800, 1024, 1152, 1280, 1600 };
  static int res_height[] = { 600,  768,  864,	960, 1200 };
  int i, j;

  VirtualProtect( ADDR_DEFAULT, 11,   PAGE_EXECUTE_READWRITE, &dummy );
  VirtualProtect( ADDR_LIST,	0x5e, PAGE_EXECUTE_READWRITE, &dummy );

  GetWindowRect( GetDesktopWindow(), &desktop );
  *(DWORD*)ADDR_DEFAULT     = desktop.right;
  *(DWORD*)(ADDR_DEFAULT+7) = desktop.bottom;

  for (j = i = 0; i < 5; ++i)
  {
    if (desktop.right == res_width[i] && desktop.bottom == res_height[i])
      break;
    if (desktop.bottom >= res_height[i])
      j = i;
  }
  if (i == 5)
  {
    memcpy( res_width,	res_width + 1,	j * sizeof(int) );
    memcpy( res_height, res_height + 1, j * sizeof(int) );
    res_width[j]  = desktop.right;
    res_height[j] = desktop.bottom;
    for (addr = ADDR_LIST, i = 0; i < 5; addr += 10, ++i)
    {
      *(DWORD*)addr	 = res_width[i];
      *(DWORD*)(addr+50) = res_height[i];
    }
  }
}


void Patch_Common( void )
{
  #define ADDR_VERS10 (0x62c1485 - 0x6260000 + common)
  #define ADDR_FATE10 (0x6293727 - 0x6260000 + common)
  #define ADDR_ZONE10 (0x633c26c - 0x6260000 + common)

  #define ADDR_VERS11 (0x62c14e5 - 0x6260000 + common)
  #define ADDR_FATE11 (0x6293487 - 0x6260000 + common)
  #define ADDR_ZONE11 (0x633c34c - 0x6260000 + common)

  DWORD dummy;

  PBYTE addr_fate, addr_zone;

  PBYTE common = (PBYTE)GetModuleHandle( "common.dll" );
  if (*(DWORD*)ADDR_VERS11 == 0x639F3CC)
  {
    addr_fate = ADDR_FATE11;
    addr_zone = ADDR_ZONE11;
  }
  else if (*(DWORD*)ADDR_VERS10 == 0x639F39C)
  {
    addr_fate = ADDR_FATE10;
    addr_zone = ADDR_ZONE10;
  }
  else
    return;

  // Change the fate test to below 3 is bad, below/equal 5 is good.
  VirtualProtect( addr_fate, 6, PAGE_EXECUTE_READWRITE, &dummy );
  memcpy( addr_fate, "\x72\x05\x83\xf8\x05\x76", 6 );

  // Skip the zone message altogether.
  VirtualProtect( addr_zone, 1, PAGE_EXECUTE_READWRITE, &dummy );
  *addr_zone = 0x28;
}


NAKED
void RendComp_Patch( void )
{
  __asm mov edx, [ebp]
  __asm cmp edx, 163618903
  __asm lea eax, [esp+28]
  __asm ret
}


void Patch_RendComp( void )
{
  #define ADDR_MATERIAL (0x6c318be - 0x6c20000 + rendcomp)

  DWORD dummy;

  PBYTE rendcomp = (PBYTE)GetModuleHandle( "rendcomp.dll" );
  if (*ADDR_MATERIAL == 0x8b)
  {
    // Add in a test for the ignored material.
    static BYTE code[] =
    {
      0xE8,0x00,0x00,0x00,0x00, 	// call RendComp_Patch
      0x74,0x2D,			// jz	skip_log
      0x52
    };
    *(DWORD*)(code+1) = (PBYTE)RendComp_Patch - ADDR_MATERIAL - 5;
    VirtualProtect( ADDR_MATERIAL, sizeof(code), PAGE_EXECUTE_READWRITE, &dummy );
    memcpy( ADDR_MATERIAL, code, sizeof(code) );
  }
}


void Patch( void )
{
  #define ADDR_ASSIST	((PBYTE)0x4ede00)
  #define ADDR_DEFORM	((PBYTE)0x452648)
  #define ADDR_STATS1	((PBYTE)0x47c21f)
  #define ADDR_STATS2	((PBYTE)0x4840c9)
  #define ADDR_STATSNL1 ((PBYTE)0x47c355)
  #define ADDR_STATSNL2 ((PBYTE)0x47c501)
  #define ADDR_RES	((PBYTE)0x424ad4)
  #define ADDR_FOVX	((PBYTE)0x40f623)
  #define ADDR_MOUSE	((PBYTE)0x41d9c3)
  #define ADDR_NAVMAP	((PBYTE)0x499a3b)
  #define ADDR_NAVMAP1	((PBYTE)0x494605)
  #define ADDR_NAVMAP2	((PBYTE)0x49485b)
  #define ADDR_FONT1	((PDWORD*)0x415cc7)
  #define ADDR_FONT2	((PDWORD*)0x416663)
  #define ADDR_FONT3	((PDWORD*)0x416913)
  #define ADDR_FONT4	((PDWORD*)0x416be3)
  #define ADDR_FONT5	((PDWORD*)0x412e8d)	// determines shadow offset

  DWORD dummy;

  if (*ADDR_ASSIST == 0x68)
  {
    // Skip the "Used Hostile Pick Assistance" message.
    VirtualProtect( ADDR_ASSIST, 2, PAGE_EXECUTE_READWRITE, &dummy );
    ADDR_ASSIST[0] = 0xeb;
    ADDR_ASSIST[1] = 0x26;

    VirtualProtect( ADDR_DEFORM, 10, PAGE_EXECUTE_READWRITE, &dummy );
    memcpy( ADDR_DEFORM, "\x6a\x00"             // push 0 ;"(null)"
			 "\x8b\x76\x04"         // mov  esi, [esi+4]
			 "\xff\x76\x08"         // push dword[esi+8]
			 "\x90\x90", 10 );      // nop/nop

    // Replace the equipment stats jump table.
    VirtualProtect( ADDR_STATS1, 26, PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_STATS2, 19, PAGE_EXECUTE_READWRITE, &dummy );
    ADDR_STATS1[2] = ADDR_STATS2[2] = -11;
    ADDR_STATS1[5] = ADDR_STATS2[5] = 23;
    *(DWORD*)(ADDR_STATS2+15) = (DWORD)stats_jump_index;
    *(DWORD*)(ADDR_STATS2+22) = (DWORD)stats_jump_table;
    *(DWORD*)(ADDR_STATS1+15) = (DWORD)stats_bool_index;

    // Add a blank line after the equipment's "Stats".
    VirtualProtect( ADDR_STATSNL1, 20, PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_STATSNL2, 20, PAGE_EXECUTE_READWRITE, &dummy );
    *(DWORD*)(ADDR_STATSNL1+1)	=
    *(DWORD*)(ADDR_STATSNL1+16) =
    *(DWORD*)(ADDR_STATSNL2+1)	=
    *(DWORD*)(ADDR_STATSNL2+16) = 0x5d0a08;

    // Support widescreen displays.
    VirtualProtect( ADDR_RES,	  5,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_FOVX,	  5,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_MOUSE,   0x13, PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_NAVMAP,  0x3c, PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_NAVMAP1, 5,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_NAVMAP2, 5,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( FOVX_WIN,	  4,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( BAR_WIDTH,	  4,	PAGE_READWRITE, &dummy );
    VirtualProtect( ADDR_FONT1,   4,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_FONT2,   4,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_FONT3,   4,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_FONT4,   4,	PAGE_EXECUTE_READWRITE, &dummy );
    VirtualProtect( ADDR_FONT5,   4,	PAGE_EXECUTE_READWRITE, &dummy );
    ADDR_RES[0] = 0xe8;
    *(DWORD*)(ADDR_RES+1) = (PBYTE)Resolution_Hook - ADDR_RES - 5;
    ADDR_FOVX[0] = 0xe9;
    *(DWORD*)(ADDR_FOVX+1) = (PBYTE)Fovx_Hook - ADDR_FOVX - 5;
    ADDR_MOUSE[2] = 0x54;			 // use height instead of width
    *(DWORD*)(ADDR_MOUSE+16) = 0x5d1cfc;	 // 1/600 instead of 1/800
    ADDR_NAVMAP[1] = 0x54;			 // use height instead of width
    *(DWORD*)(ADDR_NAVMAP+9) = 1080;		 // and above use NavMap1600
    ADDR_NAVMAP[0x0e] = 0x20;
    *(DWORD*)(ADDR_NAVMAP+0x30) = 864;		 // and above use NavMap1280
    ADDR_NAVMAP[0x34] = 0x73;
    *(DWORD*)(ADDR_NAVMAP+0x37) = 768;		 // and above use NavMap1024
    ADDR_NAVMAP[0x3b] = 0x73;
    ADDR_NAVMAP[0x3c] = 0xeb;
    *(DWORD*)(ADDR_NAVMAP1+1) = (PBYTE)NavMap_On  - ADDR_NAVMAP1 - 5;
    *(DWORD*)(ADDR_NAVMAP2+1) = (PBYTE)NavMap_Off - ADDR_NAVMAP2 - 5;
    *ADDR_FONT1 = *ADDR_FONT2 = *ADDR_FONT3 = *ADDR_FONT4 =
    *ADDR_FONT5 = &width43;
    Patch_Dimensions();
  }

  Patch_Common();
  Patch_RendComp();
}


BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    Patch();

  return TRUE;
}
