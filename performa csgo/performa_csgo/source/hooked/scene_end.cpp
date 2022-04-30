#include "procedure.hpp"

#include "../engine.hpp"

#include "../feature/movement.hpp"

#include "../../valve/source/player.hpp"
#include "../../valve/source/weapon.hpp"

#include "../../option/storage.hpp"

#include "../../imgui/imgui.hpp"

namespace source
{

namespace hooked
{

auto ForceMaterial( const ImColor& color, IMaterial* mat, bool color_change /*= true*/ ) -> void
{
	if( color_change )
	{
		float color_data[] =
		{
			color.Value[ 0 ],
			color.Value[ 1 ],
			color.Value[ 2 ],
		};

		auto color_alpha = color.Value[ 3 ];

		source::m_render_view->SetBlend( color_alpha );
		source::m_render_view->SetColorModulation( color_data );
	}

	source::m_model_render->ForcedMaterialOverride( mat );
}

auto CreateMaterialMemory( bool texture, bool ignore_z ) -> IMaterial*
{
	static auto mat_created = 0;

	static const char mat_struct[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$flat\" \"1\"\
		\n}\n"
	};

	auto base_type = ( texture ? "VertexLitGeneric" : "UnlitGeneric" );

	char mat_data[ 512 ] = { };
	sprintf_s( mat_data, sizeof( mat_data ), mat_struct, base_type, ignore_z ? 1 : 0 );

	char mat_name[ 512 ] = { };
	sprintf_s( mat_name, sizeof( mat_name ), "horizon_mat_%i.vmt", mat_created );

	mat_created++;

	auto key = new KeyValues( base_type );
	key->LoadFromBuffer( mat_name, mat_data );

	auto mat = source::m_material_system->CreateMaterial( mat_name, key );
	return mat;
}

auto CreateMaterial( bool vertex, bool ignorez, bool wireframe = false ) -> IMaterial*
{
	auto mat_name = "horizon"s;

	if( vertex )
		mat_name.append( "_vertex" );

	if( ignorez )
		mat_name.append( "_ignorez" );

	auto mat_base = ( vertex ? "\"VertexLitGeneric\""s : "\"UnlitGeneric\""s );

	std::ofstream( "csgo\\materials\\"s + mat_name + ".vmt"s ) << mat_base << R"#(
	{
	  "$basetexture"  "vgui/white_additive"
	  "$ignorez"      "0"
	  "$envmap"       ""
	  "$nofog"        "1"
	  "$model"        "1"
	  "$nocull"       "0"
	  "$selfillum"    "1"
	  "$halflambert"  "1"
	  "$znearer"      "0"
	  "$flat"         "1"
	}
	)#";

	auto mat = source::m_material_system->FindMaterial( mat_name.c_str(), TEXTURE_GROUP_MODEL );

	// if( mat )
	// {
	// 	mat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, ignorez );
	// 	mat->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, wireframe );
	// }

	return mat;
}

auto __fastcall SceneEnd( void* ecx, void* edx ) -> void
{
	auto& movement = feature::Movement::Instance();
	auto& esp = option::m_esp;
	auto& misc = option::m_misc;

	auto fn = ( source::m_render_view_swap ?
				source::m_render_view_swap->Get< SceneEndFn >( index::IVRenderView::SceneEnd ) :
				memory::vget< SceneEndFn >( ecx, index::IVRenderView::SceneEnd ) );

	fn( ecx );

	static auto color_transparent = ImColor{ 0.f, 0.f, 0.f, 0.f };
	static auto color_fake = ImColor{ 1.f, 1.f, 1.f, 1.f };
	static auto color_lower_body = ImColor{ 1.f, 1.f, 0.f, 1.f };

	static auto color_player_t = ImColor{ 1.f, 0.f, 0.f, 1.f };
	static auto color_player_ct = ImColor{ 0.f, 0.5f, 1.f, 1.f };

	static auto mat = CreateMaterialMemory( true, true );

	if( mat && g_chams )
	{
		for( auto i = 1; i <= source::m_engine_client->GetMaxClients(); i++ )
		{
			auto player = C_CSPlayer::GetCSPlayer( i );

			if( player->IsGood() && !player->IsLocal() )
			{
				ForceMaterial( { 0.f, 0.5f, 1.f, 1.f }, mat );
				player->DrawModel( 0x0001, 255 );
				ForceMaterial( { 0.f, 0.f, 0.f, 0.f }, nullptr, false );
			}
		}
	}

	// 
	// chams
	// 

	static auto mat_player = CreateMaterial( true, false );
	static auto mat_player_ignorez = CreateMaterial( true, true );
	static auto mat_player_wireframe = CreateMaterial( true, false, true );

	if( mat_player && mat_player_ignorez && mat_player_wireframe )
	{
		mat_player->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		mat_player->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );

		mat_player_ignorez->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
		mat_player_ignorez->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, false );

		mat_player_wireframe->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
		mat_player_wireframe->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, true );

		auto local = C_CSPlayer::GetLocalCSPlayer();

		if( local->IsGood() )
		{
			if( misc.m_thirdperson && ( misc.m_ghost_fake || misc.m_ghost_lower_body ) )
			{
				auto angles = local->GetAbsAngles();

				if( misc.m_ghost_fake )
				{
					ForceMaterial( color_fake, mat_player );

					local->SetAbsAngles( movement.GetFakeAngles() );
					local->DrawModel( 0x0001, 255 );
				}

				if( misc.m_ghost_lower_body )
				{
					ForceMaterial( color_lower_body, mat_player );

					local->SetAbsAngles( movement.GetLowerBodyAngles() );
					local->DrawModel( 0x0001, 255 );
				}

				local->SetAbsAngles( angles );

				ForceMaterial( color_transparent, nullptr, false );
			}

			if( esp.m_chams )
			{
				for( auto player_index = 1; player_index <= source::m_engine_client->GetMaxClients(); player_index++ )
				{
					auto player = C_CSPlayer::GetCSPlayer( player_index );

					if( player->IsGood() && !player->IsLocal() )
					{
						if( player->m_iTeamNum() == 2 )
							ForceMaterial( color_player_t, mat_player_ignorez );
						else if( player->m_iTeamNum() == 3 )
							ForceMaterial( color_player_ct, mat_player_ignorez );

						player->DrawModel( 0x0001, 255 );
					}
				}

				ForceMaterial( color_transparent, nullptr, false );
			}
		}
	}
}

}

}