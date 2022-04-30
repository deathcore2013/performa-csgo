#pragma once

#include "math/core.hpp"
#include "math/qangle.hpp"
#include "math/vector.hpp"
#include "math/vector2d.hpp"
#include "math/vector4d.hpp"

#include "../horizon/framework.hpp"

#pragma region region_index
namespace index
{
namespace IMoveHelper
{
constexpr auto SetHost = 1;
}
namespace IViewRender
{
constexpr auto RenderView = 6;
}
namespace INetChannelInfo
{
constexpr auto GetLatency = 9;
constexpr auto GetAvgLatency = 10;
}
namespace IMaterial
{
constexpr auto AlphaModulate = 29;
constexpr auto ColorModulate = 30;
constexpr auto SetMaterialVarFlag = 31;
}
namespace ITexture
{
constexpr auto GetActualWidth = 3;
constexpr auto GetActualHeight = 4;
}
namespace IMatRenderContext
{
constexpr auto Release = 1;
constexpr auto SetRenderTarget = 6;
constexpr auto DrawScreenSpaceRectangle = 113;
constexpr auto PushRenderTargetAndViewport = 118;
constexpr auto PopRenderTargetAndViewport = 119;
}

namespace IBaseClientDLL
{
constexpr auto GetAllClasses = 8;
constexpr auto CreateMove = 21;
constexpr auto FrameStageNotify = 36;
}
namespace IClientEntityList
{
constexpr auto GetClientEntity = 3;
constexpr auto GetClientEntityFromHandle = 4;
constexpr auto GetHighestEntityIndex = 6;
}
namespace IGameMovement
{
constexpr auto ProcessMovement = 1;
constexpr auto StartTrackPredictionErrors = 3;
constexpr auto FinishTrackPredictionErrors = 4;
}
namespace IPrediction
{
constexpr auto RunCommand = 19;
constexpr auto SetupMove = 20;
constexpr auto FinishMove = 21;
}
namespace IVEngineClient
{
constexpr auto GetScreenSize = 5;
constexpr auto GetPlayerInfo = 8;
constexpr auto GetLocalPlayer = 12;
constexpr auto GetViewAngles = 18;
constexpr auto SetViewAngles = 19;
constexpr auto GetMaxClients = 20;
constexpr auto WorldToScreenMatrix = 37;
constexpr auto GetNetChannelInfo = 78;
constexpr auto ClientCmd_Unrestricted = 114;
}
namespace IVModelInfoClient
{
constexpr auto GetModelName = 3;
constexpr auto GetStudiomodel = 30;
}
namespace IVRenderView
{
constexpr auto SetBlend = 4;
constexpr auto SetColorModulation = 6;
constexpr auto SceneEnd = 9;
}
namespace IVModelRender
{
constexpr auto ForcedMaterialOverride = 1;
}
namespace IEngineTrace
{
constexpr auto GetPointContents = 0;
constexpr auto TraceRay = 5;
}
namespace ICvar
{
constexpr auto RegisterConCommand = 11;
constexpr auto UnregisterConCommand = 12;
constexpr auto FindVar = 14;
}
namespace IPhysicsSurfaceProps
{
constexpr auto GetSurfaceData = 5;
}
namespace IMaterialSystem
{
constexpr auto GetBackBufferFormat = 36;
constexpr auto CreateMaterial = 83;
constexpr auto FindMaterial = 84;
constexpr auto BeginRenderTargetAllocation = 94;
constexpr auto EndRenderTargetAllocation = 95;
constexpr auto CreateNamedRenderTargetTextureEx = 97;
constexpr auto GetRenderContext = 115;
}
namespace IDirect3DDevice9
{
constexpr auto Reset = 16;
constexpr auto Present = 17;
}
}
#pragma endregion

#pragma region region_define
#define MULTIPLAYER_BACKUP 150

#define TICK_INTERVAL ( source::m_globals->interval_per_tick )
#define TIME_TO_TICKS( t ) ( static_cast< int >( 0.5f + static_cast< float >( t ) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t ) ( TICK_INTERVAL * ( t ) )
#define ROUND_TO_TICKS( t ) ( TICK_INTERVAL * TIME_TO_TICKS( t ) )
#define TICK_NEVER_THINK ( -1 )

#define LIFE_ALIVE 0

#define FL_ONGROUND ( 1 << 0 )
#define FL_DUCKING ( 1 << 1 )

#define IN_ATTACK ( 1 << 0 )
#define IN_JUMP ( 1 << 1 )
#define IN_DUCK ( 1 << 2 )
#define IN_FORWARD ( 1 << 3 )
#define IN_BACK ( 1 << 4 )
#define IN_USE ( 1 << 5 )
#define IN_CANCEL ( 1 << 6 )
#define IN_LEFT ( 1 << 7 )
#define IN_RIGHT ( 1 << 8 )
#define IN_MOVELEFT ( 1 << 9 )
#define IN_MOVERIGHT ( 1 << 10 )
#define IN_ATTACK2 ( 1 << 11 )
#define IN_RUN ( 1 << 12 )
#define IN_RELOAD ( 1 << 13 )
#define IN_ALT1 ( 1 << 14 )
#define IN_ALT2 ( 1 << 15 )
#define IN_SCORE ( 1 << 16 )
#define IN_SPEED ( 1 << 17 )
#define IN_WALK ( 1 << 18 )
#define IN_ZOOM ( 1 << 19 )
#define IN_WEAPON1 ( 1 << 20 )
#define IN_WEAPON2 ( 1 << 21 )
#define IN_BULLRUSH ( 1 << 22 )
#define IN_GRENADE1 ( 1 << 23 )
#define IN_GRENADE2 ( 1 << 24 )

#define FLOW_OUTGOING 0		
#define FLOW_INCOMING 1
#define MAX_FLOWS 2

#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10

#define	CONTENTS_EMPTY			0
#define	CONTENTS_SOLID			0x1
#define	CONTENTS_WINDOW			0x2
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_BLOCKLOS		0x40
#define CONTENTS_OPAQUE			0x80
#define	LAST_VISIBLE_CONTENTS	0x80
#define ALL_VISIBLE_CONTENTS	(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200
#define CONTENTS_UNUSED6		0x400
#define CONTENTS_TEAM1			0x800
#define CONTENTS_TEAM2			0x1000
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000
#define CONTENTS_MOVEABLE		0x4000
#define	CONTENTS_AREAPORTAL		0x8000
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000
#define	CONTENTS_ORIGIN			0x1000000
#define	CONTENTS_MONSTER		0x2000000
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000
#define	CONTENTS_TRANSLUCENT	0x10000000
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000

#define	SURF_LIGHT		0x0001
#define	SURF_SKY2D		0x0002
#define	SURF_SKY		0x0004
#define	SURF_WARP		0x0008
#define	SURF_TRANS		0x0010
#define SURF_NOPORTAL	0x0020
#define	SURF_TRIGGER	0x0040
#define	SURF_NODRAW		0x0080
#define	SURF_HINT		0x0100
#define	SURF_SKIP		0x0200
#define SURF_NOLIGHT	0x0400
#define SURF_BUMPLIGHT	0x0800
#define SURF_NOSHADOWS	0x1000
#define SURF_NODECALS	0x2000
#define SURF_NOCHOP		0x4000
#define SURF_HITBOX		0x8000

#define	MASK_ALL					(0xFFFFFFFF)
#define	MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define	MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define	MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
#define	MASK_VISIBLE				(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define	MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define	MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#define CS_MASK_SHOOT				( MASK_SOLID | CONTENTS_DEBRIS )

#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"

#define CREATERENDERTARGETFLAGS_HDR				0x00000001
#define CREATERENDERTARGETFLAGS_AUTOMIPMAP		0x00000002
#define CREATERENDERTARGETFLAGS_UNFILTERABLE_OK 0x00000004
// XBOX ONLY:
#define CREATERENDERTARGETFLAGS_NOEDRAM			0x00000008 // inhibit allocation in 360 EDRAM
#define CREATERENDERTARGETFLAGS_TEMP			0x00000010 // only allocates memory upon first resolve, destroyed at level end
#pragma endregion

#pragma region region_enum
enum SendPropType
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes,
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END,
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum CSItemDefinitionIndex
{
	ITEM_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 20,
	WEAPON_XM1014 = 21,
	WEAPON_BIZON = 22,
	WEAPON_MAG7 = 23,
	WEAPON_NEGEV = 24,
	WEAPON_SAWEDOFF = 25,
	WEAPON_TEC9 = 26,
	WEAPON_TASER = 27,
	WEAPON_HKP2000 = 28,
	WEAPON_MP7 = 29,
	WEAPON_MP9 = 30,
	WEAPON_NOVA = 31,
	WEAPON_P250 = 32,
	WEAPON_SCAR20 = 33,
	WEAPON_SG556 = 34,
	WEAPON_SSG08 = 35,
	WEAPON_KNIFE = 36,
	WEAPON_FLASHBANG = 37,
	WEAPON_HEGRENADE = 38,
	WEAPON_SMOKEGRENADE = 39,
	WEAPON_MOLOTOV = 40,
	WEAPON_DECOY = 41,
	WEAPON_INCGRENADE = 42,
	WEAPON_C4 = 43,
	WEAPON_KNIFE_T = 44,
	WEAPON_M4A1_SILENCER = 45,
	WEAPON_USP_SILENCER = 46,
	WEAPON_CZ75A = 47,
	WEAPON_REVOLVER = 48,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	ITEM_MAX,
};

enum MaterialType_t
{
	CHAR_TEX_ANTLION = 'A',
	CHAR_TEX_BLOODYFLESH = 'B',
	CHAR_TEX_CONCRETE = 'C',
	CHAR_TEX_DIRT = 'D',
	CHAR_TEX_EGGSHELL = 'E',
	CHAR_TEX_FLESH = 'F',
	CHAR_TEX_GRATE = 'G',
	CHAR_TEX_ALIENFLESH = 'H',
	CHAR_TEX_CLIP = 'I',
	CHAR_TEX_PLASTIC = 'L',
	CHAR_TEX_METAL = 'M',
	CHAR_TEX_SAND = 'N',
	CHAR_TEX_FOLIAGE = 'O',
	CHAR_TEX_COMPUTER = 'P',
	CHAR_TEX_SLOSH = 'S',
	CHAR_TEX_TILE = 'T',
	CHAR_TEX_VENT = 'V',
	CHAR_TEX_WOOD = 'W',
	CHAR_TEX_GLASS = 'Y',
	CHAR_TEX_WARPSHIELD = 'Z'
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = ( 1 << 0 ),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = ( 1 << 1 ),
	MATERIAL_VAR_NO_DRAW = ( 1 << 2 ),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = ( 1 << 3 ),
	MATERIAL_VAR_VERTEXCOLOR = ( 1 << 4 ),
	MATERIAL_VAR_VERTEXALPHA = ( 1 << 5 ),
	MATERIAL_VAR_SELFILLUM = ( 1 << 6 ),
	MATERIAL_VAR_ADDITIVE = ( 1 << 7 ),
	MATERIAL_VAR_ALPHATEST = ( 1 << 8 ),
	//MATERIAL_VAR_UNUSED = (1 << 9),
	MATERIAL_VAR_ZNEARER = ( 1 << 10 ),
	MATERIAL_VAR_MODEL = ( 1 << 11 ),
	MATERIAL_VAR_FLAT = ( 1 << 12 ),
	MATERIAL_VAR_NOCULL = ( 1 << 13 ),
	MATERIAL_VAR_NOFOG = ( 1 << 14 ),
	MATERIAL_VAR_IGNOREZ = ( 1 << 15 ),
	MATERIAL_VAR_DECAL = ( 1 << 16 ),
	MATERIAL_VAR_ENVMAPSPHERE = ( 1 << 17 ), // OBSOLETE
	MATERIAL_VAR_UNUSED = ( 1 << 18 ), // UNUSED
	MATERIAL_VAR_ENVMAPCAMERASPACE = ( 1 << 19 ), // OBSOLETE
	MATERIAL_VAR_BASEALPHAENVMAPMASK = ( 1 << 20 ),
	MATERIAL_VAR_TRANSLUCENT = ( 1 << 21 ),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = ( 1 << 22 ),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = ( 1 << 23 ), // OBSOLETE
	MATERIAL_VAR_OPAQUETEXTURE = ( 1 << 24 ),
	MATERIAL_VAR_ENVMAPMODE = ( 1 << 25 ), // OBSOLETE
	MATERIAL_VAR_SUPPRESS_DECALS = ( 1 << 26 ),
	MATERIAL_VAR_HALFLAMBERT = ( 1 << 27 ),
	MATERIAL_VAR_WIREFRAME = ( 1 << 28 ),
	MATERIAL_VAR_ALLOWALPHATOCOVERAGE = ( 1 << 29 ),
	MATERIAL_VAR_ALPHA_MODIFIED_BY_PROXY = ( 1 << 30 ),
	MATERIAL_VAR_VERTEXFOG = ( 1 << 31 ),
};

enum ImageFormat
{
	IMAGE_FORMAT_UNKNOWN = -1,
	IMAGE_FORMAT_RGBA8888 = 0,
	IMAGE_FORMAT_ABGR8888,
	IMAGE_FORMAT_RGB888,
	IMAGE_FORMAT_BGR888,
	IMAGE_FORMAT_RGB565,
	IMAGE_FORMAT_I8,
	IMAGE_FORMAT_IA88,
	IMAGE_FORMAT_P8,
	IMAGE_FORMAT_A8,
	IMAGE_FORMAT_RGB888_BLUESCREEN,
	IMAGE_FORMAT_BGR888_BLUESCREEN,
	IMAGE_FORMAT_ARGB8888,
	IMAGE_FORMAT_BGRA8888,
	IMAGE_FORMAT_DXT1,
	IMAGE_FORMAT_DXT3,
	IMAGE_FORMAT_DXT5,
	IMAGE_FORMAT_BGRX8888,
	IMAGE_FORMAT_BGR565,
	IMAGE_FORMAT_BGRX5551,
	IMAGE_FORMAT_BGRA4444,
	IMAGE_FORMAT_DXT1_ONEBITALPHA,
	IMAGE_FORMAT_BGRA5551,
	IMAGE_FORMAT_UV88,
	IMAGE_FORMAT_UVWQ8888,
	IMAGE_FORMAT_RGBA16161616F,
	IMAGE_FORMAT_RGBA16161616,
	IMAGE_FORMAT_UVLX8888,
	IMAGE_FORMAT_R32F,			// Single-channel 32-bit floating point
	IMAGE_FORMAT_RGB323232F,
	IMAGE_FORMAT_RGBA32323232F,

	// Depth-stencil texture formats for shadow depth mapping
	IMAGE_FORMAT_NV_DST16,		// 
	IMAGE_FORMAT_NV_DST24,		//
	IMAGE_FORMAT_NV_INTZ,		// Vendor-specific depth-stencil texture
	IMAGE_FORMAT_NV_RAWZ,		// formats for shadow depth mapping 
	IMAGE_FORMAT_ATI_DST16,		// 
	IMAGE_FORMAT_ATI_DST24,		//
	IMAGE_FORMAT_NV_NULL,		// Dummy format which takes no video memory

	// Compressed normal map formats
	IMAGE_FORMAT_ATI2N,			// One-surface ATI2N / DXN format
	IMAGE_FORMAT_ATI1N,			// Two-surface ATI1N format

	NUM_IMAGE_FORMATS
};

enum RenderTargetSizeMode_t
{
	RT_SIZE_NO_CHANGE = 0,			// Only allowed for render targets that don't want a depth buffer
	// (because if they have a depth buffer, the render target must be less than or equal to the size of the framebuffer).
	RT_SIZE_DEFAULT = 1,				// Don't play with the specified width and height other than making sure it fits in the framebuffer.
	RT_SIZE_PICMIP = 2,				// Apply picmip to the render target's width and height.
	RT_SIZE_HDR = 3,					// frame_buffer_width / 4
	RT_SIZE_FULL_FRAME_BUFFER = 4,	// Same size as frame buffer, or next lower power of 2 if we can't do that.
	RT_SIZE_OFFSCREEN = 5,			// Target of specified size, don't mess with dimensions
	RT_SIZE_FULL_FRAME_BUFFER_ROUNDED_UP = 6, // Same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
	RT_SIZE_REPLAY_SCREENSHOT = 7,	// Rounded down to power of 2, essentially...
	RT_SIZE_LITERAL = 8				// Use the size passed in. Don't clamp it to the frame buffer size. Really.
};

enum MaterialRenderTargetDepth_t
{
	MATERIAL_RT_DEPTH_SHARED = 0x0,
	MATERIAL_RT_DEPTH_SEPARATE = 0x1,
	MATERIAL_RT_DEPTH_NONE = 0x2,
	MATERIAL_RT_DEPTH_ONLY = 0x3,
};

enum TextureFlags
{
	// flags from the *.txt config file
	TEXTUREFLAGS_POINTSAMPLE = 0x00000001,
	TEXTUREFLAGS_TRILINEAR = 0x00000002,
	TEXTUREFLAGS_CLAMPS = 0x00000004,
	TEXTUREFLAGS_CLAMPT = 0x00000008,
	TEXTUREFLAGS_ANISOTROPIC = 0x00000010,
	TEXTUREFLAGS_HINT_DXT5 = 0x00000020,
	TEXTUREFLAGS_SRGB = 0x00000040,
	TEXTUREFLAGS_NORMAL = 0x00000080,
	TEXTUREFLAGS_NOMIP = 0x00000100,
	TEXTUREFLAGS_NOLOD = 0x00000200,
	TEXTUREFLAGS_ALL_MIPS = 0x00000400,
	TEXTUREFLAGS_PROCEDURAL = 0x00000800,

	// These are automatically generated by vtex from the texture data.
	TEXTUREFLAGS_ONEBITALPHA = 0x00001000,
	TEXTUREFLAGS_EIGHTBITALPHA = 0x00002000,

	// newer flags from the *.txt config file
	TEXTUREFLAGS_ENVMAP = 0x00004000,
	TEXTUREFLAGS_RENDERTARGET = 0x00008000,
	TEXTUREFLAGS_DEPTHRENDERTARGET = 0x00010000,
	TEXTUREFLAGS_NODEBUGOVERRIDE = 0x00020000,
	TEXTUREFLAGS_SINGLECOPY = 0x00040000,

	TEXTUREFLAGS_UNUSED_00080000 = 0x00080000,
	TEXTUREFLAGS_UNUSED_00100000 = 0x00100000,
	TEXTUREFLAGS_UNUSED_00200000 = 0x00200000,
	TEXTUREFLAGS_UNUSED_00400000 = 0x00400000,

	TEXTUREFLAGS_NODEPTHBUFFER = 0x00800000,

	TEXTUREFLAGS_UNUSED_01000000 = 0x01000000,

	TEXTUREFLAGS_CLAMPU = 0x02000000,

	TEXTUREFLAGS_VERTEXTEXTURE = 0x04000000,					// Useable as a vertex texture

	TEXTUREFLAGS_SSBUMP = 0x08000000,

	TEXTUREFLAGS_UNUSED_10000000 = 0x10000000,

	// Clamp to border color on all texture coordinates
	TEXTUREFLAGS_BORDER = 0x20000000,

	TEXTUREFLAGS_UNUSED_40000000 = 0x40000000,
	TEXTUREFLAGS_UNUSED_80000000 = 0x80000000,
};

enum ClearFlags_t
{
	VIEW_CLEAR_COLOR = 0x1,
	VIEW_CLEAR_DEPTH = 0x2,
	VIEW_CLEAR_FULL_TARGET = 0x4,
	VIEW_NO_DRAW = 0x8,
	VIEW_CLEAR_OBEY_STENCIL = 0x10, // Draws a quad allowing stencil test to clear through portals
	VIEW_CLEAR_STENCIL = 0x20,
};

enum RenderViewInfo_t
{
	RENDERVIEW_UNSPECIFIED = 0,
	RENDERVIEW_DRAWVIEWMODEL = ( 1 << 0 ),
	RENDERVIEW_DRAWHUD = ( 1 << 1 ),
	RENDERVIEW_SUPPRESSMONITORRENDERING = ( 1 << 2 ),
};
#pragma endregion

#pragma region region_struct
struct model_t;

struct player_info_t;
struct mstudiobone_t;
struct mstudiobbox_t;
struct mstudiohitboxset_t;
struct studiohdr_t;
struct cplane_t;
struct csurface_t;
struct Ray_t;
struct surfacephysicsparams_t;
struct surfaceaudioparams_t;
struct surfacesoundnames_t;
struct surfacegameprops_t;
struct surfacedata_t;
#pragma endregion

#pragma region region_class
class IConVar;
class CCommand;

class IHandleEntity;
class IClientUnknown;
class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class C_BaseAnimating;
class C_BaseCombatCharacter;
class C_BasePlayer;
class C_CSPlayer;
class C_BaseCombatWeapon;
class C_WeaponCSBase;
class C_WeaponCSBaseGun;
class CCSWeaponInfo;

class InterfaceReg;
class DVariant;
class CRecvProxyData;
class RecvProp;
class RecvTable;
class ClientClass;
class CMoveData;
class CUserCmd;
class CVerifiedUserCmd;
class CGlobalVarsBase;
class C_AnimationLayer;
class CBaseClientState;
class CBaseTrace;
class CGameTrace;
template< class T, class I = int >
class CUtlMemory;
template< class T, class A = CUtlMemory< T > >
class CUtlVector;
class ConCommandBase;
class ConVar;
class SpoofedConVar;
class KeyValues;
class CViewSetup;

class IMoveHelper;
class IViewRender;
class IInput;
class INetChannelInfo;
class IMaterial;
class ITexture;
class IMatRenderContext;

class ITraceFilter;
class CTraceFilter;
class CTraceFilterSimple;
#pragma endregion

#pragma region region_interface
class IBaseClientDLL;
class IClientEntityList;
class IGameMovement;
class IPrediction;
class IVEngineClient;
class IVModelInfoClient;
class IVRenderView;
class IVModelRender;
class IEngineTrace;
class ICvar;
class IMemAlloc;
class IPhysicsSurfaceProps;
#pragma endregion

#pragma region region_type
using Quaternion = Vector4D;
using RadianEuler = QAngle;
using VectorAligned = Vector4D;
using CRC32_t = unsigned int;
using trace_t = CGameTrace;
using CBaseHandle = unsigned long;

using InstantiateInterfaceFn = void*( * )();
using RecvVarProxyFn = void( * )( const CRecvProxyData* pData, void* pStruct, void* pOut );
using ArrayLengthRecvProxyFn = void( * )( void* pStruct, int objectID, int currentArrayLength );
using DataTableRecvVarProxyFn = void( * )( const RecvProp* pProp, void** pOut, void* pData, int objectID );
using CreateClientClassFn = IClientNetworkable*( * )( int entnum, int serialNum );
using CreateEventFn = IClientNetworkable*( * )();
using FnChangeCallback_t = void( * )( const CCommand& command );
#pragma endregion

#pragma region region_procedure
auto CRC32_Init( CRC32_t* pulCRC ) -> void;
auto CRC32_ProcessBuffer( CRC32_t* pulCRC, const void* p, int len ) -> void;
auto CRC32_Final( CRC32_t* pulCRC ) -> void;
auto CRC32_GetTableEntry( unsigned int slot ) -> CRC32_t;

auto RandomSeed( unsigned int seed ) -> void;
auto RandomFloat( float min, float max ) -> float;
#pragma endregion

// 
// implementation
// 

#pragma region region_struct_impl
struct player_info_t
{
	std::uint64_t version = 0u;
	std::uint64_t xuid = 0u;
	char name[ 128 ] = { };
	int userID = 0;
	char guid[ 20 ] = { };
	horizon_pad( 0x0010 );
	std::uint32_t friendsID = 0u;
	char friendsName[ 128 ] = { };
	bool fakeplayer = false;
	bool ishltv = false;
	std::uint32_t customFiles[ 4 ] = { };
	std::uint8_t filesDownloaded = 0u;
};

struct mstudiobone_t
{
	int sznameindex = 0;
	int parent = 0;
	int bonecontroller[ 6 ] = { };
	Vector pos = { };
	Quaternion quat = { };
	RadianEuler rot = { };
	Vector posscale = { };
	Vector rotscale = { };
	matrix3x4_t poseToBone = { };
	Quaternion qAlignment = { };
	int flags = 0;
	int proctype = 0;
	int procindex = 0;
	mutable int physicsbone = 0;
	int surfacepropidx = 0;
	int contents = 0;
	horizon_pad( 0x0020 );

	inline char* const pszName() const
	{
		return ( ( char* )this ) + sznameindex;
	}
};

struct mstudiobbox_t
{
	int bone = 0;
	int group = 0;
	Vector bbmin = { };
	Vector bbmax = { };
	int szhitboxnameindex = 0;
	horizon_pad( 0x000C );
	float radius = 0.f;
	horizon_pad( 0x0010 );

	const char* pszHitboxName()
	{
		if( szhitboxnameindex == 0 )
			return "";

		return ( ( const char* )this ) + szhitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int sznameindex = 0;
	int numhitboxes = 0;
	int hitboxindex = 0;

	inline mstudiobbox_t* pHitbox( int i ) const
	{
		return ( mstudiobbox_t* )( ( ( byte* )this ) + hitboxindex ) + i;
	};

	inline char* const pszName() const
	{
		return ( ( char* )this ) + sznameindex;
	}
};

struct studiohdr_t
{
	int id = 0;
	int version = 0;
	int checksum = 0;
	char name[ 64 ] = { };
	int length = 0;
	Vector eyeposition = { };
	Vector illumposition = { };
	Vector hull_min = { };
	Vector hull_max = { };
	Vector view_bbmin = { };
	Vector view_bbmax = { };
	int flags = 0;
	int numbones = 0;
	int boneindex = 0;
	int numbonecontrollers = 0;
	int bonecontrollerindex = 0;
	int numhitboxsets = 0;
	int hitboxsetindex = 0;

	inline mstudiobone_t* pBone( int i ) const
	{
		return ( mstudiobone_t* )( ( ( byte* )this ) + boneindex ) + i;
	}

	mstudiohitboxset_t* pHitboxSet( int i ) const
	{
		return ( mstudiohitboxset_t* )( ( ( byte* )this ) + hitboxsetindex ) + i;
	}

	inline mstudiobbox_t* pHitbox( int i, int set ) const
	{
		const mstudiohitboxset_t* s = pHitboxSet( set );

		if( !s )
			return nullptr;

		return s->pHitbox( i );
	}

	inline int iHitboxCount( int set ) const
	{
		const mstudiohitboxset_t* s = pHitboxSet( set );

		if( !s )
			return 0;

		return s->numhitboxes;
	}
};

struct cplane_t
{
	Vector normal = { };
	float dist = 0.f;
	std::uint8_t type = 0u;
	std::uint8_t signbits = 0u;
	horizon_pad( 0x02 );
};

struct csurface_t
{
	const char* name = nullptr;
	short surfaceProps = 0;
	std::uint16_t flags = 0u;
};

struct Ray_t
{
	VectorAligned m_Start = { };
	VectorAligned m_Delta = { };
	VectorAligned m_StartOffset = { };
	VectorAligned m_Extents = { };
	const matrix3x4_t* m_pWorldAxisTransform = nullptr;
	bool m_IsRay = false;
	bool m_IsSwept = false;

	Ray_t()
		: m_pWorldAxisTransform( nullptr )
	{ }

	auto Init( const Vector& start, const Vector& end ) -> void
	{
		m_Delta = ( end - start ).ToVector4D();
		m_IsSwept = ( m_Delta.LengthSquared() != 0.f );
		m_Extents.Init();
		m_pWorldAxisTransform = nullptr;
		m_IsRay = true;
		m_StartOffset.Init();
		m_Start = start.ToVector4D();
	}

	auto Init( const Vector& start, const Vector& end, const Vector& mins, const Vector& maxs )
	{
		m_Delta = ( end - start ).ToVector4D();
		m_pWorldAxisTransform = nullptr;
		m_IsSwept = ( m_Delta.LengthSquared() != 0.f );
		m_Extents = ( maxs - mins ).ToVector4D();
		m_Extents *= 0.5f;
		m_IsRay = ( m_Extents.LengthSquared() < 1e-6f );
		m_StartOffset = ( maxs + mins ).ToVector4D();
		m_StartOffset *= 0.5f;
		m_Start = start.ToVector4D() + m_StartOffset;
		m_StartOffset *= -1.f;
	}
};

struct surfacephysicsparams_t
{
	float    friction;
	float    elasticity;
	float    density;
	float    thickness;
	float    dampening;
};

struct surfaceaudioparams_t
{
	float    reflectivity;             // like elasticity, but how much sound should be reflected by this surface
	float    hardnessFactor;           // like elasticity, but only affects impact sound choices
	float    roughnessFactor;          // like friction, but only affects scrape sound choices   
	float    roughThreshold;           // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float    hardThreshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float    hardVelocityThreshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
	float    highPitchOcclusion;       //a value betweeen 0 and 100 where 0 is not occluded at all and 100 is silent (except for any additional reflected sound)
	float    midPitchOcclusion;
	float    lowPitchOcclusion;
};

struct surfacesoundnames_t
{
	unsigned short    walkStepLeft;
	unsigned short    walkStepRight;
	unsigned short	  runStepLeft;
	unsigned short	  runStepRight;
	unsigned short    impactSoft;
	unsigned short    impactHard;
	unsigned short    scrapeSmooth;
	unsigned short    scrapeRough;
	unsigned short    bulletImpact;
	unsigned short    rolling;
	unsigned short    breakSound;
	unsigned short    strainSound;
};

struct surfacegameprops_t
{
public:
	float max_speed_factor = 0.f;
	float jump_factor = 0.f;
	float penetration_modifier = 0.f;
	float damage_modifier = 0.f;
	unsigned short material = 0u;
	unsigned char climbable = 0u;
	horizon_pad( 0x04 );
};

struct surfacedata_t
{
	surfacephysicsparams_t	physics;
	surfaceaudioparams_t	audio;
	surfacesoundnames_t		sounds;
	surfacegameprops_t		game;
};
#pragma endregion

#pragma region region_class_impl
class InterfaceReg
{
public:
	InstantiateInterfaceFn m_CreateFn = nullptr;
	const char* m_pName = nullptr;
	InterfaceReg* m_pNext = nullptr;
};

class DVariant
{
	union
	{
		float m_Float;
		int m_Int;
		const char* m_pString;
		void* m_pData;
		float m_Vector[ 3 ];
		std::int64_t m_Int64;
	};
	SendPropType m_Type = { };
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp = nullptr;
	horizon_pad( 0x04 );
	DVariant m_Value = { };
	int m_iElement = 0;
	int m_ObjectID = 0;
};

class RecvProp
{
public:
	const char* m_pVarName = nullptr;
	SendPropType m_RecvType = { };
	int m_Flags = 0;
	int m_StringBufferSize = 0;
	bool m_bInsideArray = false;
	const void* m_pExtraData = nullptr;
	RecvProp* m_pArrayProp = nullptr;
	ArrayLengthRecvProxyFn m_ArrayLengthProxy = nullptr;
	RecvVarProxyFn m_ProxyFn = nullptr;
	DataTableRecvVarProxyFn m_DataTableProxyFn = nullptr;
	RecvTable* m_pDataTable = nullptr;
	int m_Offset = 0;
	int m_ElementStride = 0;
	int m_nElements = 0;
	const char* m_pParentArrayPropName = nullptr;
};

class RecvTable
{
public:
	RecvProp* m_pProps = nullptr;
	int m_nProps = 0;
	void* m_pDecoder = nullptr;
	const char* m_pNetTableName = nullptr;
	bool m_bInitialized = false;
	bool m_bInMainList = false;
};

class ClientClass
{
public:
	CreateClientClassFn m_pCreateFn = nullptr;
	CreateEventFn m_pCreateEventFn = nullptr;
	char* m_pNetworkName = nullptr;
	RecvTable* m_pRecvTable = nullptr;
	ClientClass* m_pNext = nullptr;
	int m_ClassID = 0;
};

class CMoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle = 0;
	int m_nImpulseCommand = 0;
	QAngle m_vecViewAngles = { };
	QAngle m_vecAbsViewAngles = { };
	int m_nButtons = 0;
	int m_nOldButtons = 0;
	float m_flForwardMove = 0.f;
	float m_flSideMove = 0.f;
	float m_flUpMove = 0.f;
	float m_flMaxSpeed = 0.f;
	float m_flClientMaxSpeed = 0.f;
	Vector m_vecVelocity = { };
	Vector m_vecAngles = { };
	Vector m_vecOldAngles = { };
	float m_outStepHeight = 0.f;
	Vector m_outWishVel = { };
	Vector m_outJumpVel = { };
	Vector m_vecConstraintCenter = { };
	float m_flConstraintRadius = 0.f;
	float m_flConstraintWidth = 0.f;
	float m_flConstraintSpeedFactor = 0.f;
	horizon_pad( 0x0014 );
	Vector m_vecAbsOrigin = { };
};

class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	CUserCmd( const CUserCmd& cmd )
	{
		*this = cmd;
	}

	virtual ~CUserCmd() = default;

public:
	auto Reset() -> void
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Init();
		aimdirection.Init();
		forwardmove = 0.f;
		sidemove = 0.f;
		upmove = 0.f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;
		hasbeenpredicted = false;
		headangles.Init();
		headoffset.Init();
	}

	auto GetChecksum() const -> CRC32_t
	{
		CRC32_t crc = 0u;

		CRC32_Init( &crc );
		CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
		CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
		CRC32_ProcessBuffer( &crc, &viewangles, sizeof( viewangles ) );
		CRC32_ProcessBuffer( &crc, &aimdirection, sizeof( aimdirection ) );
		CRC32_ProcessBuffer( &crc, &forwardmove, sizeof( forwardmove ) );
		CRC32_ProcessBuffer( &crc, &sidemove, sizeof( sidemove ) );
		CRC32_ProcessBuffer( &crc, &upmove, sizeof( upmove ) );
		CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
		CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );
		CRC32_ProcessBuffer( &crc, &weaponselect, sizeof( weaponselect ) );
		CRC32_ProcessBuffer( &crc, &weaponsubtype, sizeof( weaponsubtype ) );
		CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
		CRC32_ProcessBuffer( &crc, &mousedx, sizeof( mousedx ) );
		CRC32_ProcessBuffer( &crc, &mousedy, sizeof( mousedy ) );
		CRC32_Final( &crc );

		return crc;
	}

public:
	auto operator = ( const CUserCmd& cmd ) -> CUserCmd&
	{
		if( this == &cmd )
			return *this;

		command_number = cmd.command_number;
		tick_count = cmd.tick_count;
		viewangles = cmd.viewangles;
		aimdirection = cmd.aimdirection;
		forwardmove = cmd.forwardmove;
		sidemove = cmd.sidemove;
		upmove = cmd.upmove;
		buttons = cmd.buttons;
		impulse = cmd.impulse;
		weaponselect = cmd.weaponselect;
		weaponsubtype = cmd.weaponsubtype;
		random_seed = cmd.random_seed;
		mousedx = cmd.mousedx;
		mousedy = cmd.mousedy;
		hasbeenpredicted = cmd.hasbeenpredicted;
		headangles = cmd.headangles;
		headoffset = cmd.headoffset;

		return *this;
	}

public:
	int command_number = 0;
	int tick_count = 0;
	QAngle viewangles = { };
	Vector aimdirection = { };
	float forwardmove = 0.f;
	float sidemove = 0.f;
	float upmove = 0.f;
	int buttons = 0;
	std::uint8_t impulse = 0u;
	int weaponselect = 0;
	int weaponsubtype = 0;
	int random_seed = 0;
	short mousedx = 0;
	short mousedy = 0;
	bool hasbeenpredicted = false;
	QAngle headangles = { };
	Vector headoffset = { };
};

class CVerifiedUserCmd
{
public:
	CUserCmd m_cmd = { };
	CRC32_t m_crc = 0u;
};

class CGlobalVarsBase
{
public:
	float realtime = 0.f;
	int framecount = 0;
	float absoluteframetime = 0.f;
	float absoluteframestarttimestddev = 0.f;
	float curtime = 0.f;
	float frametime = 0.f;
	int maxClients = 0;
	int tickcount = 0;
	float interval_per_tick = 0.f;
	float interpolation_amount = 0.f;
	int simTicksThisFrame = 0;
	int network_protocol = 0;
	void* pSaveData = nullptr;
	bool m_bClient = false;
	int nTimestampNetworkingBase = 0;
	int nTimestampRandomizeWindow = 0;
};

class C_AnimationLayer
{
public:
	horizon_pad( 0x0014 );
	int m_nOrder = 0;
	int m_nSequence = 0;
	float m_flPrevCycle = 0.f;
	float m_flWeight = 0.f;
	float m_flWeightDeltaRate = 0.f;
	float m_flPlaybackRate = 0.f;
	float m_flCycle = 0.f;
	horizon_pad( 0x0008 );
};

class CBaseClientState
{
public:
	auto m_nChokedCommands() -> int&
	{
		static auto offset = 0x004CB0;
		return *( int* )( this + offset );
	}
};

class CBaseTrace
{
public:
	Vector startpos = { };
	Vector endpos = { };
	cplane_t plane = { };
	float fraction = 0.f;
	int contents = 0;
	std::uint16_t dispFlags = 0u;
	bool allsolid = false;
	bool startsolid = false;
};

class CGameTrace : public CBaseTrace
{
public:
	float fractionleftsolid = 0.f;
	csurface_t surface = { };
	int hitgroup = 0;
	short physicsbone = 0;
	std::uint16_t worldSurfaceIndex = 0u;
	C_BaseEntity* m_pEnt = nullptr;
	int hitbox = 0;

	auto DidHitWorld() const -> bool;
	auto DidHitNonWorldEntity() const -> bool;
	auto GetEntityIndex() const -> int;
};

template< class T, class I >
class CUtlMemory
{
public:
	T* m_pMemory = nullptr;
	int m_nAllocationCount = 0;
	int m_nGrowSize = 0;
};

template< class T, class A >
class CUtlVector
{
	using CAllocator = A;

public:
	CAllocator m_Memory = { };
	int m_Size = 0;
	T* m_pElements = nullptr;
};

class ConCommandBase
{
public:
	virtual ~ConCommandBase() = default;

public:
	ConCommandBase* m_pNext = nullptr;
	int m_bRegistered = 0;
	const char* m_pszName = nullptr;
	const char* m_pszHelpString = nullptr;
	int m_nFlags = 0;
};

class ConVar : public ConCommandBase
{
public:
	virtual ~ConVar() = default;

public:
	auto GetFloat() -> float
	{
		std::uint32_t xored = *( std::uint32_t* )&m_pParent->m_Value.m_fValue ^ ( std::uint32_t )this;
		return *( float* )&xored;
	}

	auto GetInt() -> int
	{
		return ( int )( m_pParent->m_Value.m_nValue ^ ( int )this );
	}

	auto GetBool() -> bool
	{
		return !!GetInt();
	}

	auto SetValue( float value ) -> void
	{
		float fNewValue = value;

		*( uint32_t* )&m_pParent->m_Value.m_fValue = *( uint32_t* )&fNewValue ^ ( uint32_t )this;
		*( uint32_t* )&m_pParent->m_Value.m_nValue = ( uint32_t )fNewValue ^ ( uint32_t )this;
	}

	auto SetValue( int value ) -> void
	{
		SetValue( static_cast< float >( value ) );
	}

	auto SetValue( bool value ) -> void
	{
		SetValue( static_cast< int >( value ) );
	}

public:
	struct CVValue_t
	{
		char* m_pszString = nullptr;
		int m_StringLength = 0;
		float m_fValue = 0.f;
		int m_nValue = 0;
	};

private:
	horizon_pad( 0x04 );
	ConVar* m_pParent = nullptr;
	const char* m_pszDefaultValue = nullptr;
	CVValue_t m_Value = { };
	int m_bHasMin = 0;
	float m_fMinVal = 0.f;
	int m_bHasMax = 0;
	float m_fMaxVal = 0.f;
	CUtlVector< FnChangeCallback_t > m_fnChangeCallbacks = { };
};

class CViewSetup
{
public:
	int x;
	int oldX;
	int y;
	int oldY;
	int width;
	int oldWidth;
	int height;
	int oldHeight;

	bool m_bOrtho;
	float m_OrthoLeft;
	float m_OrthoTop;
	float m_OrthoRight;
	float m_OrthoBottom;

private:
	char pad1[ 0x7C ];

public:
	float fov;
	float fovViewmodel;
	Vector origin;
	QAngle angles;

	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;

	float m_flAspectRatio;
	float m_flNearBlurDepth;
	float m_flNearFocusDepth;
	float m_flFarFocusDepth;
	float m_flFarBlurDepth;
	float m_flNearBlurRadius;
	float m_flFarBlurRadius;
	int m_nDoFQuality;
	unsigned int m_nMotionBlurMode;

	float m_flShutterTime;
	Vector m_vShutterOpenPosition;
	QAngle m_shutterOpenAngles;
	Vector m_vShutterClosePosition;
	QAngle m_shutterCloseAngles;

	float m_flOffCenterTop;
	float m_flOffCenterBottom;
	float m_flOffCenterLeft;
	float m_flOffCenterRight;

	bool m_bOffCenter : 1;
	bool m_bRenderToSubrectOfLargerScreen : 1;
	bool m_bDoBloomAndToneMapping : 1;
	bool m_bDoDepthOfField : 1;
	bool m_bHDRTarget : 1;
	bool m_bDrawWorldNormal : 1;
	bool m_bCullFrontFaces : 1;
	bool m_bCacheFullSceneState : 1;
	bool m_bRenderFlashlightDepthTranslucents : 1;
private:
	char pad2[ 0x40 ];
};

class IMoveHelper
{
public:
	auto SetHost( C_BaseEntity* host ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_BaseEntity* );
		return memory::vget< Fn >( this, index::IMoveHelper::SetHost )( this, host );
	}
};

#pragma pack( push, 1 )
class IInput
{
public:
	auto GetUserCmd( int sequence_number ) -> CUserCmd*
	{
		return &m_pCommands[ sequence_number % MULTIPLAYER_BACKUP ];
	}

	auto GetVerifiedCmd( int sequence_number ) -> CVerifiedUserCmd*
	{
		return &m_pVerifiedCommands[ sequence_number % MULTIPLAYER_BACKUP ];
	}

public:
	// horizon_pad( 0x00EC );

	horizon_pad( 0x00A5 );
	bool m_fCameraInThirdPerson = false;
	horizon_pad( 0x0002 );
	Vector m_vecCameraOffset = { };
	horizon_pad( 0x0038 );
	CUserCmd* m_pCommands = nullptr;
	CVerifiedUserCmd* m_pVerifiedCommands = nullptr;

	/*horizon_pad( 0x009D );								// 0x0000
	bool m_fCameraInThirdPerson = false;				// 0x009D
	horizon_pad( 0x0002 );								// 0x009E
	Vector m_vecCameraOffset = { };						// 0x00A0
	horizon_pad( 0x0040 );								// 0x00AC
	CUserCmd* m_pCommands = nullptr;					// 0x00EC
	CVerifiedUserCmd* m_pVerifiedCommands = nullptr;	// 0x00F0*/
};
#pragma pack( pop )

class INetChannelInfo
{
public:
	auto GetLatency( int flow ) -> float
	{
		using Fn = float( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, index::INetChannelInfo::GetLatency )( this, flow );
	}

	auto GetAvgLatency( int flow ) -> float
	{
		using Fn = float( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, index::INetChannelInfo::GetAvgLatency )( this, flow );
	}
};

class IMaterial
{
public:
	auto AlphaModulate( float alpha ) -> void
	{
		using Fn = void( __thiscall* )( void*, float );
		return memory::vget< Fn >( this, index::IMaterial::AlphaModulate )( this, alpha );
	}

	auto ColorModulate( float r, float g, float b ) -> void
	{
		using Fn = void( __thiscall* )( void*, float, float, float );
		return memory::vget< Fn >( this, index::IMaterial::ColorModulate )( this, r, g, b );
	}

	auto SetMaterialVarFlag( MaterialVarFlags_t flag, bool on ) -> void
	{
		using Fn = void( __thiscall* )( void*, MaterialVarFlags_t, bool );
		return memory::vget< Fn >( this, index::IMaterial::SetMaterialVarFlag )( this, flag, on );
	}
};

struct Texture_t
{
	horizon_pad( 0x0C );
	IDirect3DTexture9* m_pTexture = nullptr;
};

class ITexture
{
public:
	auto GetActualWidth() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::ITexture::GetActualWidth )( this );
	}

	auto GetActualHeight() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::ITexture::GetActualHeight )( this );
	}

public:
	horizon_pad( 0x0050 );
	Texture_t** m_pTextureHandles = nullptr;
};

class IMatRenderContext
{
public:
	auto Release() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMatRenderContext::Release )( this );
	}

	auto SetRenderTarget( ITexture* pTexture ) -> void
	{
		using Fn = void( __thiscall* )( void*, ITexture* );
		return memory::vget< Fn >( this, index::IMatRenderContext::SetRenderTarget )( this, pTexture );
	}

	auto DrawScreenSpaceRectangle( IMaterial* pMaterial, int destX, int destY, int width, int height, float srcTextureX0, float srcTextureY0, float srcTextureX1, float srcTextureY1, int srcTextureWidth, int srcTextureHeight, void* pClientRenderable, int nXDice, int nYDice ) -> void
	{
		using Fn = void( __thiscall* )( void*, IMaterial*, int, int, int, int, float, float, float, float, int, int, void*, int, int );
		return memory::vget< Fn >( this, index::IMatRenderContext::DrawScreenSpaceRectangle )( this, pMaterial, destX, destY, width, height, srcTextureX0, srcTextureY0, srcTextureX1, srcTextureY1, srcTextureWidth, srcTextureHeight, pClientRenderable, nXDice, nYDice );
	}

	auto PushRenderTargetAndViewport() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMatRenderContext::PushRenderTargetAndViewport )( this );
	}

	auto PopRenderTargetAndViewport() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMatRenderContext::PopRenderTargetAndViewport )( this );
	}
};

class ITraceFilter
{
public:
	virtual auto ShouldHitEntity( IHandleEntity* pEntity, int contentsMask ) -> bool = 0;
	virtual auto GetTraceType() const -> TraceType_t = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	virtual auto GetTraceType() const -> TraceType_t
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceFilterSimple : public CTraceFilter
{
public:
	CTraceFilterSimple( C_BaseEntity* base_entity = nullptr )
		: m_skip( base_entity )
	{ }

	virtual auto ShouldHitEntity( IHandleEntity* pEntity, int contentsMask ) -> bool
	{
		return !( pEntity == ( IHandleEntity* )m_skip );
	}

private:
	C_BaseEntity* m_skip = nullptr;
};
#pragma endregion

#pragma region region_interface_impl
class IBaseClientDLL
{
public:
	auto GetAllClasses() -> ClientClass*
	{
		using Fn = ClientClass*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IBaseClientDLL::GetAllClasses )( this );
	}
};

class IClientEntityList
{
public:
	auto GetClientEntity( int entnum ) -> IClientEntity*
	{
		using Fn = IClientEntity*( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, index::IClientEntityList::GetClientEntity )( this, entnum );
	}

	auto GetClientEntityFromHandle( CBaseHandle hEnt ) -> IClientEntity*
	{
		using Fn = IClientEntity*( __thiscall* )( void*, CBaseHandle );
		return memory::vget< Fn >( this, index::IClientEntityList::GetClientEntityFromHandle )( this, hEnt );
	}

	auto GetHighestEntityIndex() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IClientEntityList::GetHighestEntityIndex )( this );
	}
};

class IGameMovement
{
public:
	auto ProcessMovement( C_CSPlayer* pPlayer, CMoveData* pMove ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_CSPlayer*, CMoveData* );
		return memory::vget< Fn >( this, index::IGameMovement::ProcessMovement )( this, pPlayer, pMove );
	}

	auto StartTrackPredictionErrors( C_CSPlayer* pPlayer ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_CSPlayer* );
		return memory::vget< Fn >( this, index::IGameMovement::StartTrackPredictionErrors )( this, pPlayer );
	}

	auto FinishTrackPredictionErrors( C_CSPlayer* pPlayer ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_CSPlayer* );
		return memory::vget< Fn >( this, index::IGameMovement::FinishTrackPredictionErrors )( this, pPlayer );
	}
};

class IPrediction
{
public:
	auto SetupMove( C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, IMoveHelper*, CMoveData* );
		return memory::vget< Fn >( this, index::IPrediction::SetupMove )( this, player, ucmd, pHelper, move );
	}

	auto FinishMove( C_BasePlayer* player, CUserCmd* ucmd, CMoveData* move ) -> void
	{
		using Fn = void( __thiscall* )( void*, C_BasePlayer*, CUserCmd*, CMoveData* );
		return memory::vget< Fn >( this, index::IPrediction::FinishMove )( this, player, ucmd, move );
	}
};

class IVEngineClient
{
public:
	auto GetScreenSize( int& width, int& height ) -> void
	{
		using Fn = void( __thiscall* )( void*, int&, int& );
		return memory::vget< Fn >( this, index::IVEngineClient::GetScreenSize )( this, width, height );
	}

	auto GetPlayerInfo( int ent_num, player_info_t* pinfo ) -> bool
	{
		using Fn = bool( __thiscall* )( void*, int, player_info_t* );
		return memory::vget< Fn >( this, index::IVEngineClient::GetPlayerInfo )( this, ent_num, pinfo );
	}

	auto GetPlayerForUserID( int userID ) -> int
	{
		using Fn = int( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, 9 )( this, userID );
	}

	auto GetLocalPlayer() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IVEngineClient::GetLocalPlayer )( this );
	}

	auto GetViewAngles( QAngle& va ) -> void
	{
		using Fn = void( __thiscall* )( void*, QAngle& );
		return memory::vget< Fn >( this, index::IVEngineClient::GetViewAngles )( this, va );
	}

	auto SetViewAngles( QAngle& va ) -> void
	{
		using Fn = void( __thiscall* )( void*, QAngle& );
		return memory::vget< Fn >( this, index::IVEngineClient::SetViewAngles )( this, va );
	}

	auto GetMaxClients() -> int
	{
		using Fn = int( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IVEngineClient::GetMaxClients )( this );
	}

	auto WorldToScreenMatrix() -> const VMatrix&
	{
		using Fn = const VMatrix&( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IVEngineClient::WorldToScreenMatrix )( this );
	}

	auto GetNetChannelInfo() -> INetChannelInfo*
	{
		using Fn = INetChannelInfo*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IVEngineClient::GetNetChannelInfo )( this );
	}

	auto ClientCmd_Unrestricted( const char* szCmdString, const char* newFlag = nullptr ) -> void
	{
		using Fn = void( __thiscall* )( void*, const char*, const char* );
		return memory::vget< Fn >( this, index::IVEngineClient::ClientCmd_Unrestricted )( this, szCmdString, newFlag );
	}
};

class IVModelInfoClient
{
public:
	auto GetModelName( const model_t* model ) -> const char*
	{
		using Fn = const char*( __thiscall* )( void*, const model_t* );
		return memory::vget< Fn >( this, index::IVModelInfoClient::GetModelName )( this, model );
	}

	auto GetStudiomodel( const model_t* mod ) -> studiohdr_t*
	{
		using Fn = studiohdr_t*( __thiscall* )( void*, const model_t* );
		return memory::vget< Fn >( this, index::IVModelInfoClient::GetStudiomodel )( this, mod );
	}
};

class IVRenderView
{
public:
	auto SetBlend( float blend ) -> void
	{
		using Fn = void( __thiscall* )( void*, float );
		return memory::vget< Fn >( this, index::IVRenderView::SetBlend )( this, blend );
	}

	auto SetColorModulation( const float* color ) -> void
	{
		using Fn = void( __thiscall* )( void*, const float* );
		return memory::vget< Fn >( this, index::IVRenderView::SetColorModulation )( this, color );
	}
};

class IVModelRender
{
public:
	auto ForcedMaterialOverride( IMaterial* mat, OverrideType_t type = OVERRIDE_NORMAL ) -> void
	{
		using Fn = void( __thiscall* )( void*, IMaterial*, OverrideType_t, int );
		return memory::vget< Fn >( this, index::IVModelRender::ForcedMaterialOverride )( this, mat, type, 0 );
	}
};

class IEngineTrace
{
public:
	auto GetPointContents( const Vector& vecAbsPosition, int iMask, C_BaseEntity** ppEntity = nullptr ) -> int
	{
		using Fn = int( __thiscall* )( void*, const Vector&, int, C_BaseEntity** );
		return memory::vget< Fn >( this, index::IEngineTrace::GetPointContents )( this, vecAbsPosition, iMask, ppEntity );
	}

	auto ClipRayToEntity( const Ray_t& ray, unsigned int fMask, IHandleEntity* pEnt, trace_t* pTrace ) -> void
	{
		using Fn = void( __thiscall* )( void*, const Ray_t&, unsigned int, IHandleEntity*, trace_t* );
		return memory::vget< Fn >( this, 3 )( this, ray, fMask, pEnt, pTrace );
	}

	auto TraceRay( const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace ) -> void
	{
		using Fn = void( __thiscall* )( void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t* );
		return memory::vget< Fn >( this, index::IEngineTrace::TraceRay )( this, ray, fMask, pTraceFilter, pTrace );
	}
};

class ICvar
{
public:
	auto RegisterConCommand( ConVar* pCommandBase ) -> void
	{
		using Fn = void( __thiscall* )( void*, ConVar* );
		return memory::vget< Fn >( this, index::ICvar::RegisterConCommand )( this, pCommandBase );
	}

	auto UnregisterConCommand( ConVar* pCommandBase ) -> void
	{
		using Fn = void( __thiscall* )( void*, ConVar* );
		return memory::vget< Fn >( this, index::ICvar::UnregisterConCommand )( this, pCommandBase );
	}

	auto FindVar( const char* var_name ) -> ConVar*
	{
		using Fn = ConVar*( __thiscall* )( void*, const char* );
		return memory::vget< Fn >( this, index::ICvar::FindVar )( this, var_name );
	}
};

class IMemAlloc
{
public:
	auto Alloc( int nSize ) -> void*
	{
		using Fn = void*( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, 1 )( this, nSize );
	}

	auto Realloc( void* pMem, int nSize ) -> void*
	{
		using Fn = void*( __thiscall* )( void*, void*, int );
		return memory::vget< Fn >( this, 3 )( this, pMem, nSize );
	}

	auto Free( void* pMem ) -> void
	{
		using Fn = void( __thiscall* )( void*, void* );
		return memory::vget< Fn >( this, 5 )( this, pMem );
	}
};

class IPhysicsSurfaceProps
{
public:
	auto GetSurfaceData( int dataIndex ) -> surfacedata_t*
	{
		using Fn = surfacedata_t* ( __thiscall* )( void*, int );
		return memory::vget< Fn >( this, index::IPhysicsSurfaceProps::GetSurfaceData )( this, dataIndex );
	}
};

class IMaterialSystem
{
public:
	auto GetBackBufferFormat() -> ImageFormat
	{
		using Fn = ImageFormat( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMaterialSystem::GetBackBufferFormat )( this );
	}

	auto CreateMaterial( const char* name, KeyValues* key ) -> IMaterial*
	{
		using Fn = IMaterial* ( __thiscall* )( void*, const char*, KeyValues* );
		return memory::vget< Fn >( this, index::IMaterialSystem::CreateMaterial )( this, name, key );
	}

	auto FindMaterial( const char* name, const char* texture_group_name, bool complain = true, const char* complain_prefix = nullptr ) -> IMaterial*
	{
		using Fn = IMaterial* ( __thiscall* )( void*, const char*, const char*, bool, const char* );
		return memory::vget< Fn >( this, index::IMaterialSystem::FindMaterial )( this, name, texture_group_name, complain, complain_prefix );
	}

	auto BeginRenderTargetAllocation() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMaterialSystem::BeginRenderTargetAllocation )( this );
	}
	
	auto EndRenderTargetAllocation() -> void
	{
		using Fn = void( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMaterialSystem::EndRenderTargetAllocation )( this );
	}

	auto CreateNamedRenderTargetTextureEx( const char* name, int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags ) -> ITexture*
	{
		using Fn = ITexture*( __thiscall* )( void*, const char*, int, int, RenderTargetSizeMode_t, ImageFormat, MaterialRenderTargetDepth_t, unsigned int, unsigned int );
		return memory::vget< Fn >( this, index::IMaterialSystem::CreateNamedRenderTargetTextureEx )( this, name, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags );
	}

	auto GetRenderContext() -> IMatRenderContext*
	{
		using Fn = IMatRenderContext*( __thiscall* )( void* );
		return memory::vget< Fn >( this, index::IMaterialSystem::GetRenderContext )( this );
	}

public:
	auto m_bGameStarted() -> bool&;

public:
	auto ForceBeginRenderTargetAllocation() -> void;
	auto ForceEndRenderTargetAllocation() -> void;
	auto CreateFullFrameRenderTarget( const char* name )->ITexture*;
};
#pragma endregion







class IGameEvent
{
public:
	virtual ~IGameEvent() = 0;
	virtual const char *GetName() const = 0;

	virtual bool  IsReliable() const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsEmpty( const char *keyName = NULL ) = 0;

	virtual bool  GetBool( const char *keyName = NULL, bool defaultValue = false ) = 0;
	virtual int   GetInt( const char *keyName = NULL, int defaultValue = 0 ) = 0;
	virtual unsigned long GetUint64( const char *keyName = NULL, unsigned long defaultValue = 0 ) = 0;
	virtual float GetFloat( const char *keyName = NULL, float defaultValue = 0.0f ) = 0;
	virtual const char *GetString( const char *keyName = NULL, const char *defaultValue = "" ) = 0;
	virtual const wchar_t *GetWString( const char *keyName, const wchar_t *defaultValue = L"" ) = 0;

	virtual void SetBool( const char *keyName, bool value ) = 0;
	virtual void SetInt( const char *keyName, int value ) = 0;
	virtual void SetUint64( const char *keyName, unsigned long value ) = 0;
	virtual void SetFloat( const char *keyName, float value ) = 0;
	virtual void SetString( const char *keyName, const char *value ) = 0;
	virtual void SetWString( const char *keyName, const wchar_t *value ) = 0;
};

class IGameEventListener2
{
public:
	virtual ~IGameEventListener2() { }

	virtual void FireGameEvent( IGameEvent* event ) = 0;
	virtual int GetEventDebugID() = 0;
};

class IGameEventManager
{
public:
	auto AddListener( IGameEventListener2* listener, const char* name, bool bServerSide ) -> bool
	{
		using Fn = bool( __thiscall* )( void*, IGameEventListener2*, const char*, bool );
		return memory::vget< Fn >( this, 3 )( this, listener, name, bServerSide );
	}

	auto RemoveListener( IGameEventListener2* listener ) -> int
	{
		using Fn = int( __thiscall* )( void*, IGameEventListener2* );
		return memory::vget< Fn >( this, 5 )( this, listener );
	}

	auto FireEventClientSide( IGameEvent* event ) -> bool
	{
		using Fn = bool( __thiscall* )( void*, IGameEvent* );
		return  memory::vget< Fn >( this, 9 )( this, event );
	}
};

class SpoofedConVar
{
public:
	SpoofedConVar() = default;
	SpoofedConVar( const char* var_name );
	SpoofedConVar( ConVar* cvar );

	~SpoofedConVar();

	auto IsSpoofed() -> bool;
	auto Spoof() -> void;

	auto SetFlags( int flags ) -> void;
	auto GetFlags() -> int;

	auto SetBool( bool value ) -> void;
	auto SetInt( int value ) -> void;
	auto SetFloat( float value ) -> void;

private:
	ConVar* m_cvar_restore = nullptr;
	ConVar* m_cvar_replace = nullptr;

	char m_restore_name[ 128 ] = { };
	// char m_restore_value[ 128 ] = { };
	float m_restore_value = 0.f;

	char m_replace_name[ 128 ] = { };
	// char m_replace_value[ 128 ] = { };
	float m_replace_value = 0.f;

	int m_restore_flags = 0;
};

class KeyValues
{
public:
	KeyValues( const char* setName );

public:
	auto LoadFromBuffer( const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = nullptr ) -> bool;

private:
	horizon_pad( 0x0025 );
};