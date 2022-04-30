#include "procedure.hpp"

#include "../engine.hpp"
#include "../engine/renderer.hpp"

#include "../feature/gui.hpp"
#include "../feature/menu.hpp"
#include "../feature/anim_state.hpp"
#include "../feature/lag_compensation.hpp"
#include "../feature/aimbot.hpp"
#include "../feature/movement.hpp"
#include "../feature/resolver.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

using namespace std::string_literals;

namespace source
{

namespace hooked
{

const VMatrix& WorldToScreenMatrix()
{
	static auto procedure = memory::vget< std::uintptr_t >( source::m_engine_client, index::IVEngineClient::WorldToScreenMatrix );

	auto render = *( std::uintptr_t* )( procedure + 0x0001 );
	auto mat = *( std::uintptr_t* )( render + 0x00DC );

	return *( const VMatrix* )( mat + 0x03DC );
}

auto WorldToScreen( const Vector& world, Vector2D& screen ) -> bool
{
	auto mat = WorldToScreenMatrix();

	auto z = mat[ 3 ][ 0 ] * world[ 0 ] + mat[ 3 ][ 1 ] * world[ 1 ] + mat[ 3 ][ 2 ] * world[ 2 ] + mat[ 3 ][ 3 ];

	if( z < 0.01f )
		return false;

	auto inverse = 1.f / z;

	screen[ 0 ] = ( mat[ 0 ][ 0 ] * world[ 0 ] + mat[ 0 ][ 1 ] * world[ 1 ] + mat[ 0 ][ 2 ] * world[ 2 ] + mat[ 0 ][ 3 ] ) * inverse;
	screen[ 1 ] = ( mat[ 1 ][ 0 ] * world[ 0 ] + mat[ 1 ][ 1 ] * world[ 1 ] + mat[ 1 ][ 2 ] * world[ 2 ] + mat[ 1 ][ 3 ] ) * inverse;

	auto w = 0, h = 0;
	source::m_engine_client->GetScreenSize( w, h );

	auto x = static_cast< float >( w ) / 2.f;
	auto y = static_cast< float >( h ) / 2.f;

	x += 0.5f * screen[ 0 ] * static_cast< float >( w ) + 0.5f;
	y -= 0.5f * screen[ 1 ] * static_cast< float >( h ) + 0.5f;

	screen[ 0 ] = x;
	screen[ 1 ] = y;

	return true;
}

void DrawBorderBox( const Vector2D& begin, const Vector2D& size, float t, const ImColor& color )
{
	auto& renderer = engine::Renderer::Instance();

	renderer.DrawRect( begin, { size[ 0 ], t }, color );
	renderer.DrawRect( begin, { t, size[ 1 ] }, color );

	renderer.DrawRect( { begin[ 0 ] + size[ 0 ], begin[ 1 ] }, { t, size[ 1 ] }, color );
	renderer.DrawRect( { begin[ 0 ], begin[ 1 ] + size[ 1 ] }, { size[ 0 ] + t, t }, color );
}

void DrawBorderBoxOut( const Vector2D& begin, const Vector2D& size, float t, const ImColor& color, const ImColor& out = { 0.f, 0.f, 0.f, 1.f } )
{
	DrawBorderBox( begin, size, t, color );
	DrawBorderBox( { begin[ 0 ] - t, begin[ 1 ] - t }, { size[ 0 ] + t * 2.f, size[ 1 ] + t * 2.f }, 1.f, out );
	DrawBorderBox( { begin[ 0 ] + t, begin[ 1 ] + t }, { size[ 0 ] - t * 2.f, size[ 1 ] - t * 2.f }, 1.f, out );
}

auto RenderSkeleton( C_CSPlayer* player ) -> void
{
	auto& renderer = engine::Renderer::Instance();

	matrix3x4_t bone_data[ 128 ] = { };

	if( !player->SetupBones( bone_data, 128, 0x0100, source::m_globals->curtime ) )
		return;

	auto model = player->GetModel();

	if( model )
	{
		auto studio = source::m_model_info_client->GetStudiomodel( model );

		if( studio )
		{
			for( auto i = 0; i < studio->numbones; i++ )
			{
				auto bone = studio->pBone( i );

				if( !bone )
					continue;

				if( !( bone->flags & 0x0100 ) )
					continue;

				if( bone->parent == -1 )
					continue;

				Vector bone_world, parent_world;
				Vector2D bone_screen, parent_screen;

				player->GetBoneWorld( i, bone_data, bone_world );
				player->GetBoneWorld( bone->parent, bone_data, parent_world );

				if( WorldToScreen( bone_world, bone_screen ) && WorldToScreen( parent_world, parent_screen ) )
					renderer.DrawLine( bone_screen, parent_screen, { 1.f, 1.f, 1.f, 1.f } );
			}
		}
	}
}

auto ansi_to_unicode( const std::string& ansi ) -> std::wstring
{
	wchar_t unicode[ 512 ] = { };
	MultiByteToWideChar( CP_UTF8, 0u, ansi.c_str(), -1, unicode, 512 );
	return unicode;
}

auto unicode_to_ansi( const std::wstring& unicode ) -> std::string
{
	char ansi[ 512 ] = { };
	WideCharToMultiByte( CP_UTF8, 0u, unicode.c_str(), -1, ansi, 512, nullptr, nullptr );
	return ansi;
}



void PaintEntity( int index, C_CSPlayer* player )
{
	using namespace engine;

	auto& esp = option::m_esp;
	auto& renderer = engine::Renderer::Instance();
	
	if( player->IsDormant() )
		return;

	if( player->m_lifeState() != LIFE_ALIVE )
		return;

	if( !esp.m_target_friendly )
	{
		if( player->m_iTeamNum() == C_CSPlayer::GetLocalCSPlayer()->m_iTeamNum() )
			return;
	}

	matrix3x4_t bone_data[ 128 ] = { };

	if( !player->GetBoneTransform( bone_data ) )
		return;

	Vector head = { };

	if( !player->GetBoxWorld( 0, bone_data, head ) )
		return;

	ImColor color = { 1.f, 1.f, 1.f, 1.f };

	if( player->m_iTeamNum() == 2 )
	{
		color = { 1.f, 0.f, 0.f, 1.f };
	}
	else if( player->m_iTeamNum() == 3 )
	{
		color = { 0.f, 0.5f, 1.f, 1.f };
	}

	const auto& transform = player->m_rgflCoordinateFrame();

	Vector position( transform[ 0 ][ 3 ], transform[ 1 ][ 3 ], transform[ 2 ][ 3 ] );

	Vector mins = player->OBBMins();
	Vector maxs = player->OBBMaxs();

	Vector points[] =
	{
		{ mins[ 0 ], mins[ 1 ], mins[ 2 ] },
		{ mins[ 0 ], maxs[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], maxs[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], mins[ 1 ], mins[ 2 ] },
		{ maxs[ 0 ], maxs[ 1 ], maxs[ 2 ] },
		{ mins[ 0 ], maxs[ 1 ], maxs[ 2 ] },
		{ mins[ 0 ], mins[ 1 ], maxs[ 2 ] },
		{ maxs[ 0 ], mins[ 1 ], maxs[ 2 ] },
	};

	Vector transformed[ 8 ] = { };

	for( auto i = 0; i < 8; i++ )
		transformed[ i ] = points[ i ].Transform( transform );

	Vector2D flb, brt, blb, frt, frb, brb, blt, flt;

	if( !WorldToScreen( transformed[ 3 ], flb ) ||
		!WorldToScreen( transformed[ 0 ], blb ) ||
		!WorldToScreen( transformed[ 2 ], frb ) ||
		!WorldToScreen( transformed[ 6 ], blt ) ||
		!WorldToScreen( transformed[ 5 ], brt ) ||
		!WorldToScreen( transformed[ 4 ], frt ) ||
		!WorldToScreen( transformed[ 1 ], brb ) ||
		!WorldToScreen( transformed[ 7 ], flt ) )
		return;

	Vector2D screen[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb[ 0 ];
	float top = flb[ 1 ];
	float right = flb[ 0 ];
	float bottom = flb[ 1 ];

	for( int i = 0; i < 8; i++ )
	{
		if( left > screen[ i ][ 0 ] )
			left = screen[ i ][ 0 ];
		if( top < screen[ i ][ 1 ] )
			top = screen[ i ][ 1 ];
		if( right < screen[ i ][ 0 ] )
			right = screen[ i ][ 0 ];
		if( bottom > screen[ i ][ 1 ] )
			bottom = screen[ i ][ 1 ];
	}

	auto x = std::round( left );
	auto y = std::round( bottom );

	auto w = std::round( right - left );
	auto h = std::round( top - bottom );

	if( esp.m_box_mode == 1 )
	{
		if( esp.m_outlined )
			DrawBorderBoxOut( { x, y }, { w, h }, 1.f, color );
		else
			DrawBorderBox( { x, y }, { w, h }, 1.f, color );
	}
	else if( esp.m_box_mode == 2 )
	{
		auto line_w = std::round( w / 4.f );
		auto line_h = std::round( h / 4.f );

		if( esp.m_outlined )
		{
			renderer.DrawRect( { x - 1.f, y - 1.f }, { line_w + 2.f, 3.f }, { 0.f, 0.f, 0.f, 1.f } ); // top left -> right
			renderer.DrawRect( { x - 1.f, y - 1.f }, { 3.f, line_h + 2.f }, { 0.f, 0.f, 0.f, 1.f } ); // top left -> bottom

			renderer.DrawRect( { x - 1.f, y + h - line_h }, { 3.f, line_h + 2.f }, { 0.f, 0.f, 0.f, 1.f } ); // bottom left -> top
			renderer.DrawRect( { x - 1.f, y + h - 1.f }, { line_w + 2.f, 3.f }, { 0.f, 0.f, 0.f, 1.f } ); // bottom left -> right

			renderer.DrawRect( { x + w - line_w, y - 1.f }, { line_w + 2.f, 3.f }, { 0.f, 0.f, 0.f, 1.f } ); // top right -> left
			renderer.DrawRect( { x + w - 1.f, y - 1.f }, { 3.f, line_h + 2.f }, { 0.f, 0.f, 0.f, 1.f } ); // top right -> bottom

			renderer.DrawRect( { x + w - line_w, y + h - 1.f }, { line_w + 2.f, 3.f }, { 0.f, 0.f, 0.f, 1.f } ); // bottom right -> left
			renderer.DrawRect( { x + w - 1.f, y + h - line_h }, { 3.f, line_h + 2 }, { 0.f, 0.f, 0.f, 1.f } ); // bottom right -> top
		}

		renderer.DrawRect( { x, y }, { line_w, 1.f }, color ); // top left -> right
		renderer.DrawRect( { x, y }, { 1.f, line_h }, color ); // top left -> bottom

		renderer.DrawRect( { x, y + h - line_h + 1.f }, { 1.f, line_h }, color ); // bottom left -> top
		renderer.DrawRect( { x, y + h }, { line_w, 1.f }, color ); // bottom left -> right

		renderer.DrawRect( { x + w - line_w + 1.f, y }, { line_w, 1.f }, color ); // top right -> left
		renderer.DrawRect( { x + w, y }, { 1.f, line_h }, color ); // top right -> bottom

		renderer.DrawRect( { x + w - line_w + 1.f, y + h }, { line_w, 1.f }, color ); // bottom right -> left
		renderer.DrawRect( { x + w, y + h - line_h + 1.f }, { 1.f, line_h }, color ); // bottom right -> top
	}

	auto& resolver = feature::Resolver::Instance();

	auto pad_h = 0.f;

	if( esp.m_name )
	{
		player_info_t info = { };

		if( source::m_engine_client->GetPlayerInfo( index, &info ) )
			renderer.DrawText( { x + w / 2.f, y - 16.f }, engine::TextCenterH, { 1.f, 1.f, 1.f, 1.f }, "%s", info.name );
	}

	if( esp.m_weapon )
	{
		auto weapon = ( C_WeaponCSBase* )player->GetActiveWeapon();

		if( weapon )
		{
			auto weapon_data = weapon->GetCSWeaponData();

			if( weapon_data )
			{
				auto weapon_name = weapon_data->m_weapon_name;

				if( weapon_name )
					renderer.DrawText( { x + w / 2.f, y + h + 2.f }, engine::TextCenterH, { 1.f, 1.f, 1.f, 1.f }, "%s", ( weapon_name + 7u ) );
			}
		}
	}

	if( esp.m_health == 1 ) // Text
	{
		auto pad_w = 0.f;

		if( esp.m_armor == 2 )
			pad_w += 5.f;

		renderer.DrawText( { x + w + 4.f + pad_w, y }, engine::TextLeft, { 1.f, 1.f, 1.f, 1.f }, "H: %i", player->m_iHealth() );

		pad_h += 12.f;
	}
	else if( esp.m_health == 2 ) // Bar
	{
		auto health = static_cast< float >( player->m_iHealth() );

		if( health > 100 )
			health = 100;

		auto size_h = std::round( ( ( h + 1.f ) * health ) / 100.f );
		auto real_h = ( h + 1.f ) - size_h;

		renderer.DrawRect( { x - 6.f, y - 1.f }, { 4.f, h + 3.f }, { 0.f, 0.f, 0.f, 1.f } );
		renderer.DrawRect( { x - 5.f, y + real_h }, { 2.f, size_h }, { 1.f - ( health * 0.01f ), health * 0.01f, 0.f, 1.f } );
	}

	if( esp.m_armor == 1 ) // Text
	{
		renderer.DrawText( { x + w + 4.f, y + pad_h }, engine::TextLeft, { 1.f, 1.f, 1.f, 1.f }, "A: %i", player->m_ArmorValue() );

		pad_h += 13.f;
	}
	else if( esp.m_armor == 2 ) // Bar
	{
		auto armor = static_cast< float >( player->m_ArmorValue() );

		if( armor > 100 )
			armor = 100;

		auto size_h = std::round( ( ( h + 1.f ) * armor ) / 100.f );
		auto real_h = ( h + 1.f ) - size_h;

		renderer.DrawRect( { x + w + 3.f, y - 1.f }, { 4.f, h + 3.f }, { 0.f, 0.f, 0.f, 1.f } );
		renderer.DrawRect( { x + w + 4.f, y + real_h }, { 2.f, size_h }, { 0.7f, 0.7f, 0.7f, 1.f } );
	}

	if( esp.m_skeleton )
	{
		RenderSkeleton( player );
	}

	if( esp.m_defusing )
	{
		if( esp.m_name )
			y -= 16.f;

		if( player->m_bIsDefusing() )
			renderer.DrawText( { x + w / 2.f, y - 16.f }, engine::TextCenterH, { 1.f, 1.f, 1.f, 1.f }, "defusing!" );
	}
}

float ScaleDamage( float damage, int armor )
{
	float ratio = 0.5f;
	float bonus = 0.5f;

	if( armor > 0 )
	{
		float new_damage = damage * ratio;
		float new_armor = ( damage - new_damage ) * bonus;

		if( new_armor > static_cast< float >( armor ) )
		{
			new_armor = static_cast< float >( armor ) * ( 1.0f / bonus );
			new_damage = damage - new_armor;
		}

		damage = new_damage;
	}

	return damage;
}

void PaintGround( C_BaseEntity* ent )
{
	auto& esp = option::m_esp;
	auto& misc = option::m_misc;
	auto& renderer = engine::Renderer::Instance();

	if( ent->IsDormant() )
		return;

	auto name = ent->GetClientClass()->m_pNetworkName;

	auto world = ent->m_vecOrigin();

	if( world.IsZero() )
		return;

	if( std::strcmp( name, "CPlantedC4" ) == 0 )
	{
		if( esp.m_target_bomb )
		{
			Vector2D screen = { };

			if( WorldToScreen( world, screen ) )
				renderer.DrawText( screen, engine::TextLeft, { 1.f, 0.f, 0.f, 1.f }, "[ c4 ]" );
		}

		if( misc.m_bomb_warning )
		{
			auto player = C_CSPlayer::GetLocalCSPlayer();

			Vector bomb_origin = ent->m_vecOrigin();
			Vector spot_top( bomb_origin[ 0 ], bomb_origin[ 1 ], bomb_origin[ 2 ] + 8.f );
			Vector spot_bottom( spot_top[ 0 ], spot_top[ 1 ], spot_top[ 2 ] - 40.f );

			CTraceFilterSimple skip( ent );

			Ray_t ray;
			ray.Init( spot_top, spot_bottom );

			trace_t trace = { };
			source::m_engine_trace->TraceRay( ray, 0x200400B, &skip, &trace );

			auto bomb_radius = 500.f;

			if( trace.fraction != 1.f )
				bomb_origin = trace.endpos + trace.plane.normal * 0.6f;

			auto damage = bomb_radius;
			auto radius = bomb_radius * 3.5f;

			float adjusted_damage, falloff, damage_percentage;

			auto src = bomb_origin;
			src[ 2 ] += 1.f;

			auto end = player->EyePosition();

			damage_percentage = 1.f;
			falloff = ( radius != 0.f ) ? damage / radius : 1.f;

			auto target = end - src;

			adjusted_damage = ( damage - target.Length() * falloff ) * damage_percentage;
			adjusted_damage = ScaleDamage( adjusted_damage, player->m_ArmorValue() );

			if( adjusted_damage > 0.f )
			{
				int w, h;
				source::m_engine_client->GetScreenSize( w, h );

				w /= 2;

				ImColor color = { 1.f, 0.f, 0.f, 1.f };

				if( adjusted_damage < 80.f )
					color = { 1.f, 1.f, 0.f, 1.f };
				else if( adjusted_damage < 20.f )
					color = { 0.f, 1.f, 0.f, 1.f };

				renderer.DrawText( { static_cast< float >( w ), 5.f }, engine::TextCenterH, color, "explosion warning! [%.2f]", adjusted_damage );
			}
		}
	}
}

auto RenderLocal() -> void
{
	using namespace engine;

	auto& renderer = engine::Renderer::Instance();
	auto& movement = feature::Movement::Instance();
	auto& anim_state = feature::AnimState::Instance();
	auto& data = g_render_data;

	auto local = C_CSPlayer::GetLocalCSPlayer();

	if( !local->IsGood() )
		return;


	auto screen_width = 0;
	auto screen_height = 0;
	source::m_engine_client->GetScreenSize( screen_width, screen_height );

	player_info_t info = { };
	
	if( source::m_engine_client->GetPlayerInfo( local->entindex(), &info ) )
	{
		// auto player_name = ansi_to_unicode( info.name );
		auto player_name = std::string( info.name );
		renderer.DrawText( { static_cast< float >( screen_width ) - 10.f, 10.f }, TextRight, { 1.f, 1.f, 1.f, 1.f }, "%s", player_name.c_str() );
	}


	auto center = Vector2D{ 1280.f * 0.5f, 10.f };
	auto color = ImColor{ 0.f, 1.f, 0.f, 1.f };

	if( movement.IsLowerBodyUpdate() )
		color = ImColor{ 1.f, 0.f, 0.f, 1.f };

	renderer.DrawText( center, TextCenterH, color, "LowerBodyUpdate" );

	auto render_color = ImColor{ 1.f, 1.f, 1.f, 1.f };
	auto render_begin = Vector2D{ 10.f, 10.f };

	renderer.DrawText( render_begin, TextLeft, render_color, "m_flSimulationTime: [ %.2f ]", data.m_flSimulationTime ); render_begin += Vector2D{ 0.f, 14.f };
	renderer.DrawText( render_begin, TextLeft, render_color, "m_flLowerBodyYawTarget: [ %.2f ]", anim_state.GetLowerBody() ); render_begin += Vector2D{ 0.f, 14.f };
	renderer.DrawText( render_begin, TextLeft, render_color, "m_angEyeAngles: [ %.2f, %.2f, %.2f ]", data.m_angEyeAngles[ 0 ], data.m_angEyeAngles[ 1 ], data.m_angEyeAngles[ 2 ] ); render_begin += Vector2D{ 0.f, 28.f };

	return;

	auto count_row = -1;
	auto count_column = 0;

	for( auto layer_index = 0; layer_index < MAX_LAYER_RECORDS; layer_index++ )
	{
		auto layer = &data.m_AnimationLayers[ layer_index ];

		if( layer )
		{
			renderer.DrawText( render_begin, TextLeft, render_color, "Layer: [ %i ]", layer_index ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_nOrder: [ %i ]", layer->m_nOrder ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_nSequence: [ %i ]", layer->m_nSequence ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_flPrevCycle: [ %.2f ]", layer->m_flPrevCycle ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_flWeight: [ %.2f ]", layer->m_flWeight ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_flWeightDeltaRate: [ %.2f ]", layer->m_flWeightDeltaRate ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_flPlaybackRate: [ %.2f ]", layer->m_flPlaybackRate ); render_begin += Vector2D{ 0.f, 14.f };
			renderer.DrawText( render_begin, TextLeft, render_color, "m_flCycle: [ %.2f ]", layer->m_flCycle ); render_begin += Vector2D{ 0.f, 28.f };

			count_row++;

			if( count_row >= 3 )
			{
				count_row = 0;
				count_column++;

				render_begin = Vector2D{ 10.f + ( 220.f * count_column ), 10.f };
			}
		}
	}
}

void ESP()
{
	auto player = C_CSPlayer::GetLocalCSPlayer();

	if( !player )
		return;

	auto size = source::m_client_entity_list->GetHighestEntityIndex();

	for( auto i = 0; i <= size; i++ )
	{
		auto ent = C_BaseEntity::GetBaseEntity( i );

		if( !ent )
			continue;

		PaintGround( ent );

		auto enemy = ToCSPlayer( ent );

		if( !enemy )
			continue;

		if( enemy == player )
			continue;

		PaintEntity( i, enemy );
	}
}

void Crosshair()
{
	auto& renderer = engine::Renderer::Instance();
	auto& misc = option::m_misc;

	auto player = C_CSPlayer::GetLocalCSPlayer();

	if( !player )
		return;

	int w, h;
	source::m_engine_client->GetScreenSize( w, h );

	auto x = static_cast< float >( w / 2 );
	auto y = static_cast< float >( h / 2 );

	if( misc.m_crosshair_recoil )
	{
		auto dx = static_cast< float >( w ) / 90.f;
		auto dy = static_cast< float >( h ) / 90.f;

		x -= ( dx * player->m_aimPunchAngle()[ 1 ] );
		y += ( dy * player->m_aimPunchAngle()[ 0 ] );
	}

	if( misc.m_crosshair_mode == 1 ) // Dot
	{
		if( misc.m_crosshair_outlined )
			renderer.DrawRect( { x - 2.f, y - 2.f }, { 4.f, 4.f }, { 0.f, 0.f, 0.f, 1.f } );

		renderer.DrawRect( { x - 1.f, y - 1.f }, { 2.f, 2.f }, { 1.f, 0.5f, 0.f, 1.f } );
	}
	else if( misc.m_crosshair_mode == 2 ) // Cross
	{
		if( misc.m_crosshair_outlined )
		{
			renderer.DrawRect( { x - 11.f, y - 1.f }, { 23.f, 3.f }, { 0.f, 0.f, 0.f, 1.f } );
			renderer.DrawRect( { x - 1.f, y - 11.f }, { 3.f, 23.f }, { 0.f, 0.f, 0.f, 1.f } );
		}

		renderer.DrawRect( { x - 10.f, y }, { 21.f, 1.f }, { 1.f, 0.5f, 0.f, 1.f } );
		renderer.DrawRect( { x, y - 10.f }, { 1.f, 21.f }, { 1.f, 0.5f, 0.f, 1.f } );
	}
}

struct TextureVertex
{
	float x, y, z, rhw;
	D3DCOLOR color;
	float u, v;

	enum
	{
		FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
	};
};

auto CreateRenderMaterial( const char* name ) -> IMaterial*
{
	static auto mat_created = 0;

	static const char mat_struct[] =
	{
		"\"UnlitGeneric\"\
		\n{\
		\n\t\"$basetexture\" \"%s\"\
		\n}\n"
	};

	char mat_data[ 512 ] = { };
	sprintf_s( mat_data, sizeof( mat_data ), mat_struct, name );

	char mat_name[ 512 ] = { };
	sprintf_s( mat_name, sizeof( mat_name ), "horizon_texture_%i.vmt", mat_created );

	mat_created++;

	auto key = new KeyValues( name );
	key->LoadFromBuffer( mat_name, mat_data );

	auto mat = source::m_material_system->CreateMaterial( mat_name, key );
	return mat;
}

PresentFn oPresent = nullptr;

auto api_d3d Present( IDirect3DDevice9* device, const RECT* source_rect, const RECT* dest_rect, HWND dest_window_override, const RGNDATA* dirty_region ) -> HRESULT
{
	auto& renderer = engine::Renderer::Instance();

	auto& gui = feature::Gui::Instance();

	if( renderer.Begin() )
	{
		ESP();

		// static auto render_texture = CreateRenderMaterial( "texture_rt" );
		// 
		// if( render_texture && source::m_texture_render_target )
		// {
		// 	auto width = 0;
		// 	auto height = 0;
		// 	source::m_engine_client->GetScreenSize( width, height );
		// 
		// 	auto context = source::m_material_system->GetRenderContext();
		// 
		// 	context->DrawScreenSpaceRectangle( render_texture, 0, 0, width, height, 0.f, 0.f, static_cast< float >( width ), static_cast< float >( height ), source::m_texture_render_target->GetActualWidth(), source::m_texture_render_target->GetActualHeight(), nullptr, 1, 1 );
		// 	context->Release();
		// }

		RenderLocal();
		Crosshair();

		renderer.End();
	}

	

	gui.Present();
	renderer.Present();

	// auto fn = ( source::m_direct_device_swap ?
	// 			source::m_direct_device_swap->Get< PresentFn >( index::IDirect3DDevice9::Present ) :
	// 			memory::vget< PresentFn >( device, index::IDirect3DDevice9::Present ) );
	// 
	// return fn( device, source_rect, dest_rect, dest_window_override, dirty_region );
	return oPresent( device, source_rect, dest_rect, dest_window_override, dirty_region );
}

}

}