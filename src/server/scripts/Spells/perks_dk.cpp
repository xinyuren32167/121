#include "PetDefines.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "SpellAuraEffects.h"
#include "SpellInfo.h"
#include "SpellMgr.h"
#include "SpellScript.h"
#include "Totem.h"
#include "UnitAI.h"
#include "Log.h"


enum DeathKnightSpells
{

};

class rune_deathknight_diseased_attacks : public AuraScript {
    PrepareAuraScript(rune_deathknight_diseased_attacks); void Register() { }
};
class rune_deathknight_runic_attenuation : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_attenuation); void Register() { }
};
class rune_deathknight_sudden_doom : public AuraScript {
    PrepareAuraScript(rune_deathknight_sudden_doom); void Register() { }
};
class rune_deathknight_harbinger_of_doom : public AuraScript {
    PrepareAuraScript(rune_deathknight_harbinger_of_doom); void Register() { }
};
class rune_deathknight_permafrost : public AuraScript {
    PrepareAuraScript(rune_deathknight_permafrost); void Register() { }
};
class rune_deathknight_frozen_pulse : public AuraScript {
    PrepareAuraScript(rune_deathknight_frozen_pulse); void Register() { }
};
class rune_deathknight_insidious_chill : public AuraScript {
    PrepareAuraScript(rune_deathknight_insidious_chill); void Register() { }
};
class rune_deathknight_cold_steel : public AuraScript {
    PrepareAuraScript(rune_deathknight_cold_steel); void Register() { }
};
class rune_deathknight_anti_magic_barrier : public AuraScript {
    PrepareAuraScript(rune_deathknight_anti_magic_barrier); void Register() { }
};
class rune_deathknight_blood_feast : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_feast); void Register() { }
};
class rune_deathknight_assimilation : public AuraScript {
    PrepareAuraScript(rune_deathknight_assimilation);  void Register() { }
};
class rune_deathknight_reinforced_shell : public AuraScript {
    PrepareAuraScript(rune_deathknight_reinforced_shell);  void Register() { }
};
class rune_deathknight_amplification_zone : public AuraScript {
    PrepareAuraScript(rune_deathknight_amplification_zone);  void Register() { }
};
class rune_deathknight_hammer_of_the_damned : public AuraScript {
    PrepareAuraScript(rune_deathknight_hammer_of_the_damned);  void Register() { }
};
class rune_deathknight_blood_transfusion : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_transfusion);  void Register() { }
};
class rune_deathknight_boiling_blood : public AuraScript {
    PrepareAuraScript(rune_deathknight_boiling_blood);  void Register() { }
};
class rune_deathknight_hemostasis : public AuraScript {
    PrepareAuraScript(rune_deathknight_hemostasis);  void Register() { }
};
class rune_deathknight_debilitating_malady : public AuraScript {
    PrepareAuraScript(rune_deathknight_debilitating_malady);  void Register() { }
};
class rune_deathknight_disease_and_decay : public AuraScript {
    PrepareAuraScript(rune_deathknight_disease_and_decay);  void Register() { }
};
class rune_deathknight_lasting_effects : public AuraScript {
    PrepareAuraScript(rune_deathknight_lasting_effects);  void Register() { }
};
class rune_deathknight_hypothermia : public AuraScript {
    PrepareAuraScript(rune_deathknight_hypothermia);  void Register() { }
};
class rune_deathknight_infectious : public AuraScript {
    PrepareAuraScript(rune_deathknight_infectious);  void Register() { }
};
class rune_deathknight_brittle : public AuraScript {
    PrepareAuraScript(rune_deathknight_brittle);  void Register() { }
};
class rune_deathknight_crimson_scourge : public AuraScript {
    PrepareAuraScript(rune_deathknight_crimson_scourge);  void Register() { }
};
class rune_deathknight_ebon_fever : public AuraScript {
    PrepareAuraScript(rune_deathknight_ebon_fever);  void Register() { }
};
class rune_deathknight_invigorating_freeze : public AuraScript {
    PrepareAuraScript(rune_deathknight_invigorating_freeze);  void Register() { }
};
class rune_deathknight_morbidity : public AuraScript {
    PrepareAuraScript(rune_deathknight_morbidity);  void Register() { }
};
class rune_deathknight_superstrain : public AuraScript {
    PrepareAuraScript(rune_deathknight_superstrain);  void Register() { }
};
class rune_deathknight_lingering_plague : public AuraScript {
    PrepareAuraScript(rune_deathknight_lingering_plague);  void Register() { }
};
class rune_deathknight_thassarians_teachings : public AuraScript {
    PrepareAuraScript(rune_deathknight_thassarians_teachings);  void Register() { }
};
class rune_deathknight_proliferating_chill : public AuraScript {
    PrepareAuraScript(rune_deathknight_proliferating_chill);  void Register() { }
};
class rune_deathknight_cold_heart : public AuraScript {
    PrepareAuraScript(rune_deathknight_cold_heart);  void Register() { }
};
class rune_deathknight_explosion_of_death : public AuraScript {
    PrepareAuraScript(rune_deathknight_explosion_of_death);  void Register() { }
};
class rune_deathknight_hemoglobin : public AuraScript {
    PrepareAuraScript(rune_deathknight_hemoglobin);  void Register() { }
};
class rune_deathknight_phearomones : public AuraScript {
    PrepareAuraScript(rune_deathknight_phearomones);  void Register() { }
};
class rune_deathknight_quick_decay : public AuraScript {
    PrepareAuraScript(rune_deathknight_quick_decay);  void Register() { }
};
class rune_deathknight_aura_of_decay : public AuraScript {
    PrepareAuraScript(rune_deathknight_aura_of_decay);  void Register() { }
};
class rune_deathknight_grip_of_the_dead : public AuraScript {
    PrepareAuraScript(rune_deathknight_grip_of_the_dead);  void Register() { }
};
class rune_deathknight_rapid_decomposition : public AuraScript {
    PrepareAuraScript(rune_deathknight_rapid_decomposition);  void Register() { }
};
class rune_deathknight_sanguine_ground : public AuraScript {
    PrepareAuraScript(rune_deathknight_sanguine_ground);  void Register() { }
};
class rune_deathknight_unholy_ground : public AuraScript {
    PrepareAuraScript(rune_deathknight_unholy_ground);  void Register() { }
};
class rune_deathknight_deathly_empowerment : public AuraScript {
    PrepareAuraScript(rune_deathknight_deathly_empowerment);  void Register() { }
};
class rune_deathknight_scourge_of_worlds : public AuraScript {
    PrepareAuraScript(rune_deathknight_scourge_of_worlds);  void Register() { }
};
class rune_deathknight_coil_of_devastation : public AuraScript {
    PrepareAuraScript(rune_deathknight_coil_of_devastation);  void Register() { }
};
class rune_deathknight_improved_death_coil : public AuraScript {
    PrepareAuraScript(rune_deathknight_improved_death_coil);  void Register() { }
};
class rune_deathknight_clenching_grasp : public AuraScript {
    PrepareAuraScript(rune_deathknight_clenching_grasp);  void Register() { }
};
class rune_deathknight_voracious : public AuraScript {
    PrepareAuraScript(rune_deathknight_voracious);  void Register() { }
};
class rune_deathknight_soul_drinker : public AuraScript {
    PrepareAuraScript(rune_deathknight_soul_drinker);  void Register() { }
};
class rune_deathknight_shackles_of_brindaor : public AuraScript {
    PrepareAuraScript(rune_deathknight_shackles_of_brindaor);  void Register() { }
};
class rune_deathknight_blood_barrier : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_barrier);  void Register() { }
};
class rune_deathknight_dine_and_dash : public AuraScript {
    PrepareAuraScript(rune_deathknight_dine_and_dash);  void Register() { }
};
class rune_deathknight_adaptation : public AuraScript {
    PrepareAuraScript(rune_deathknight_adaptation);  void Register() { }
};
class rune_deathknight_death_tap : public AuraScript {
    PrepareAuraScript(rune_deathknight_death_tap);  void Register() { }
};
class rune_deathknight_coagulopathy : public AuraScript {
    PrepareAuraScript(rune_deathknight_coagulopathy);  void Register() { }
};
class rune_deathknight_death_bringer : public AuraScript {
    PrepareAuraScript(rune_deathknight_death_bringer);  void Register() { }
};
class rune_deathknight_endless_death : public AuraScript {
    PrepareAuraScript(rune_deathknight_endless_death);  void Register() { }
};
class rune_deathknight_ice_veins : public AuraScript {
    PrepareAuraScript(rune_deathknight_ice_veins);  void Register() { }
};
class rune_deathknight_acclimation : public AuraScript {
    PrepareAuraScript(rune_deathknight_acclimation);  void Register() { }
};
class rune_deathknight_heart_of_ice : public AuraScript {
    PrepareAuraScript(rune_deathknight_heart_of_ice);  void Register() { }
};
class rune_deathknight_unholy_endurance : public AuraScript {
    PrepareAuraScript(rune_deathknight_unholy_endurance);  void Register() { }
};
class rune_deathknight_coldthirst : public AuraScript {
    PrepareAuraScript(rune_deathknight_coldthirst);  void Register() { }
};
class rune_deathknight_the_unbeautiful : public AuraScript {
    PrepareAuraScript(rune_deathknight_the_unbeautiful);  void Register() { }
};
class rune_deathknight_neverending_frenzy : public AuraScript {
    PrepareAuraScript(rune_deathknight_neverending_frenzy);  void Register() { }
};
class rune_deathknight_all_will_serve : public AuraScript {
    PrepareAuraScript(rune_deathknight_all_will_serve);  void Register() { }
};
class rune_deathknight_enfeeble : public AuraScript {
    PrepareAuraScript(rune_deathknight_enfeeble);  void Register() { }
};
class rune_deathknight_runic_empowerment : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_empowerment);  void Register() { }
};
class rune_deathknight_unleashed_frenzy : public AuraScript {
    PrepareAuraScript(rune_deathknight_unleashed_frenzy);  void Register() { }
};
class rune_deathknight_runic_mastery : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_mastery);  void Register() { }
};
class rune_deathknight_rune_mastery : public AuraScript {
    PrepareAuraScript(rune_deathknight_rune_mastery);  void Register() { }
};
class rune_deathknight_unholy_bond : public AuraScript {
    PrepareAuraScript(rune_deathknight_unholy_bond);  void Register() { }
};
class rune_deathknight_march_of_darkness : public AuraScript {
    PrepareAuraScript(rune_deathknight_march_of_darkness);  void Register() { }
};
class rune_deathknight_cold_soul : public AuraScript {
    PrepareAuraScript(rune_deathknight_cold_soul);  void Register() { }
};
class rune_deathknight_blood_draw : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_draw);  void Register() { }
};
class rune_deathknight_will_of_the_necropolis : public AuraScript {
    PrepareAuraScript(rune_deathknight_will_of_the_necropolis);  void Register() { }
};
class rune_deathknight_purgatory : public AuraScript {
    PrepareAuraScript(rune_deathknight_purgatory);  void Register() { }
};
class rune_deathknight_bloody_armor : public AuraScript {
    PrepareAuraScript(rune_deathknight_bloody_armor);  void Register() { }
};
class rune_deathknight_blood_eater : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_eater);  void Register() { }
};
class rune_deathknight_ossuary : public AuraScript {
    PrepareAuraScript(rune_deathknight_ossuary);  void Register() { }
};
class rune_deathknight_foul_bulwark : public AuraScript {
    PrepareAuraScript(rune_deathknight_foul_bulwark);  void Register() { }
};
class rune_deathknight_reinforced_bones : public AuraScript {
    PrepareAuraScript(rune_deathknight_reinforced_bones);  void Register() { }
};
class rune_deathknight_improved_bone_shield : public AuraScript {
    PrepareAuraScript(rune_deathknight_improved_bone_shield);  void Register() { }
};
class rune_deathknight_shattering_bone : public AuraScript {
    PrepareAuraScript(rune_deathknight_shattering_bone);  void Register() { }
};
class rune_deathknight_runic_bones : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_bones);  void Register() { }
};
class rune_deathknight_bloodshot : public AuraScript {
    PrepareAuraScript(rune_deathknight_bloodshot);  void Register() { }
};
class rune_deathknight_iron_heart : public AuraScript {
    PrepareAuraScript(rune_deathknight_iron_heart);  void Register() { }
};
class rune_deathknight_dwarven_runes : public AuraScript {
    PrepareAuraScript(rune_deathknight_dwarven_runes);  void Register() { }
};
class rune_deathknight_eternal_rune_weapon : public AuraScript {
    PrepareAuraScript(rune_deathknight_eternal_rune_weapon);  void Register() { }
};
class rune_deathknight_elven_runes : public AuraScript {
    PrepareAuraScript(rune_deathknight_elven_runes);  void Register() { }
};
class rune_deathknight_insatiable_blade : public AuraScript {
    PrepareAuraScript(rune_deathknight_insatiable_blade);  void Register() { }
};
class rune_deathknight_everlasting_bond : public AuraScript {
    PrepareAuraScript(rune_deathknight_everlasting_bond);  void Register() { }
};
class rune_deathknight_heart_grasp : public AuraScript {
    PrepareAuraScript(rune_deathknight_heart_grasp);  void Register() { }
};
class rune_deathknight_doomed_heart : public AuraScript {
    PrepareAuraScript(rune_deathknight_doomed_heart);  void Register() { }
};
class rune_deathknight_withering_plague : public AuraScript {
    PrepareAuraScript(rune_deathknight_withering_plague);  void Register() { }
};
class rune_deathknight_heartrend : public AuraScript {
    PrepareAuraScript(rune_deathknight_heartrend);  void Register() { }
};
class rune_deathknight_leeching_strike : public AuraScript {
    PrepareAuraScript(rune_deathknight_leeching_strike);  void Register() { }
};
class rune_deathknight_collective_hysteria : public AuraScript {
    PrepareAuraScript(rune_deathknight_collective_hysteria);  void Register() { }
};
class rune_deathknight_runic_defense : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_defense);  void Register() { }
};
class rune_deathknight_runic_transfusion : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_transfusion);  void Register() { }
};
class rune_deathknight_ice_thirst : public AuraScript {
    PrepareAuraScript(rune_deathknight_ice_thirst);  void Register() { }
};
class rune_deathknight_red_thirst : public AuraScript {
    PrepareAuraScript(rune_deathknight_red_thirst);  void Register() { }
};
class rune_deathknight_blood_bath : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_bath);  void Register() { }
};
class rune_deathknight_vampire_tribe : public AuraScript {
    PrepareAuraScript(rune_deathknight_vampire_tribe);  void Register() { }
};
class rune_deathknight_hungry_for_blood : public AuraScript {
    PrepareAuraScript(rune_deathknight_hungry_for_blood);  void Register() { }
};
class rune_deathknight_service_of_gorefiend : public AuraScript {
    PrepareAuraScript(rune_deathknight_service_of_gorefiend);  void Register() { }
};
class rune_deathknight_unending_thirst : public AuraScript {
    PrepareAuraScript(rune_deathknight_unending_thirst);  void Register() { }
};
class rune_deathknight_umbilicus_eternus : public AuraScript {
    PrepareAuraScript(rune_deathknight_umbilicus_eternus);  void Register() { }
};
class rune_deathknight_hardened_blood : public AuraScript {
    PrepareAuraScript(rune_deathknight_hardened_blood);  void Register() { }
};
class rune_deathknight_tightening_grasp : public AuraScript {
    PrepareAuraScript(rune_deathknight_tightening_grasp);  void Register() { }
};
class rune_deathknight_perseverance_of_the_ebon_blade : public AuraScript {
    PrepareAuraScript(rune_deathknight_perseverance_of_the_ebon_blade);  void Register() { }
};
class rune_deathknight_made_of_rock : public AuraScript {
    PrepareAuraScript(rune_deathknight_made_of_rock);  void Register() { }
};
class rune_deathknight_torment : public AuraScript {
    PrepareAuraScript(rune_deathknight_torment);  void Register() { }
};
class rune_deathknight_life_siphon : public AuraScript {
    PrepareAuraScript(rune_deathknight_life_siphon);  void Register() { }
};
class rune_deathknight_relish_in_blood : public AuraScript {
    PrepareAuraScript(rune_deathknight_relish_in_blood);  void Register() { }
};
class rune_deathknight_blood_lover : public AuraScript {
    PrepareAuraScript(rune_deathknight_blood_lover);  void Register() { }
};
class rune_deathknight_damnation : public AuraScript {
    PrepareAuraScript(rune_deathknight_damnation);  void Register() { }
};
class rune_deathknight_cold_sensitivity : public AuraScript {
    PrepareAuraScript(rune_deathknight_cold_sensitivity);  void Register() { }
};
class rune_deathknight_frozen_talon : public AuraScript {
    PrepareAuraScript(rune_deathknight_frozen_talon);  void Register() { }
};
class rune_deathknight_shattering_blade : public AuraScript {
    PrepareAuraScript(rune_deathknight_shattering_blade);  void Register() { }
};
class rune_deathknight_runic_blast : public AuraScript {
    PrepareAuraScript(rune_deathknight_runic_blast);  void Register() { }
};
class rune_deathknight_master_blaster : public AuraScript {
    PrepareAuraScript(rune_deathknight_master_blaster);  void Register() { }
};
class rune_deathknight_echoing_howl : public AuraScript {
    PrepareAuraScript(rune_deathknight_echoing_howl);  void Register() { }
};
class rune_deathknight_murderous_efficiency : public AuraScript {
    PrepareAuraScript(rune_deathknight_murderous_efficiency);  void Register() { }
};
class rune_deathknight_razors_edge : public AuraScript {
    PrepareAuraScript(rune_deathknight_razors_edge);  void Register() { }
};
class rune_deathknight_cyclopean_focus : public AuraScript {
    PrepareAuraScript(rune_deathknight_cyclopean_focus);  void Register() { }
};
class rune_deathknight_bonegrinder : public AuraScript {
    PrepareAuraScript(rune_deathknight_bonegrinder);  void Register() { }
};
class rune_deathknight_chain_killing : public AuraScript {
    PrepareAuraScript(rune_deathknight_chain_killing);  void Register() { }
};
class rune_deathknight_rage_of_the_frozen_champion : public AuraScript {
    PrepareAuraScript(rune_deathknight_rage_of_the_frozen_champion);  void Register() { }
};
class rune_deathknight_icebreaker : public AuraScript {
    PrepareAuraScript(rune_deathknight_icebreaker);  void Register() { }
};
class rune_deathknight_avalanche : public AuraScript {
    PrepareAuraScript(rune_deathknight_avalanche);  void Register() { }
};
class rune_deathknight_improved_rime : public AuraScript {
    PrepareAuraScript(rune_deathknight_improved_rime);  void Register() { }
};
class rune_deathknight_inexorable_assault : public AuraScript {
    PrepareAuraScript(rune_deathknight_inexorable_assault);  void Register() { }
};
class rune_deathknight_koltiras_favor : public AuraScript {
    PrepareAuraScript(rune_deathknight_koltiras_favor);  void Register() { }
};
class rune_deathknight_frost_blade : public AuraScript {
    PrepareAuraScript(rune_deathknight_frost_blade);  void Register() { }
};
class rune_deathknight_decaying_obliteration : public AuraScript {
    PrepareAuraScript(rune_deathknight_decaying_obliteration);  void Register() { }
};
class rune_deathknight_frigid_executioner : public AuraScript {
    PrepareAuraScript(rune_deathknight_frigid_executioner);  void Register() { }
};
class rune_deathknight_eradicating_blow : public AuraScript {
    PrepareAuraScript(rune_deathknight_eradicating_blow);  void Register() { }
};
class rune_deathknight_obliteration : public AuraScript {
    PrepareAuraScript(rune_deathknight_obliteration);  void Register() { }
};
class rune_deathknight_eternal_pillar : public AuraScript {
    PrepareAuraScript(rune_deathknight_eternal_pillar);  void Register() { }
};
class rune_deathknight_frostwhelps_breath : public AuraScript {
    PrepareAuraScript(rune_deathknight_frostwhelps_breath);  void Register() { }
};
class rune_deathknight_enduring_strength : public AuraScript {
    PrepareAuraScript(rune_deathknight_enduring_strength);  void Register() { }
};
class rune_deathknight_icecap : public AuraScript {
    PrepareAuraScript(rune_deathknight_icecap);  void Register() { }
};
class rune_deathknight_impending_doom : public AuraScript {
    PrepareAuraScript(rune_deathknight_impending_doom);  void Register() { }
};
class rune_deathknight_soul_harvest : public AuraScript {
    PrepareAuraScript(rune_deathknight_soul_harvest);  void Register() { }
};
class rune_deathknight_biting_cold : public AuraScript {
    PrepareAuraScript(rune_deathknight_biting_cold);  void Register() { }
};
class rune_deathknight_everfrost : public AuraScript {
    PrepareAuraScript(rune_deathknight_everfrost);  void Register() { }
};
class rune_deathknight_gathering_storm : public AuraScript {
    PrepareAuraScript(rune_deathknight_gathering_storm);  void Register() { }
};
class rune_deathknight_piercing_chill : public AuraScript {
    PrepareAuraScript(rune_deathknight_piercing_chill);  void Register() { }
};
class rune_deathknight_enduring_chill : public AuraScript {
    PrepareAuraScript(rune_deathknight_enduring_chill);  void Register() { }
};
class rune_deathknight_accelerated_cold : public AuraScript {
    PrepareAuraScript(rune_deathknight_accelerated_cold);  void Register() { }
};
class rune_deathknight_absolute_zero : public AuraScript {
    PrepareAuraScript(rune_deathknight_absolute_zero);  void Register() { }
};
class rune_deathknight_chilled_to_the_bone : public AuraScript {
    PrepareAuraScript(rune_deathknight_chilled_to_the_bone);  void Register() { }
};
class rune_deathknight_harvest_time : public AuraScript {
    PrepareAuraScript(rune_deathknight_harvest_time);  void Register() { }
};
class rune_deathknight_necrosis : public AuraScript {
    PrepareAuraScript(rune_deathknight_necrosis);  void Register() { }
};
class rune_deathknight_plaguebringer : public AuraScript {
    PrepareAuraScript(rune_deathknight_plaguebringer);  void Register() { }
};
class rune_deathknight_rotten_touch : public AuraScript {
    PrepareAuraScript(rune_deathknight_rotten_touch);  void Register() { }
};
class rune_deathknight_reaping : public AuraScript {
    PrepareAuraScript(rune_deathknight_reaping);  void Register() { }
};
class rune_deathknight_all_will_do : public AuraScript {
    PrepareAuraScript(rune_deathknight_all_will_do);  void Register() { }
};
class rune_deathknight_light_and_dark : public AuraScript {
    PrepareAuraScript(rune_deathknight_light_and_dark);  void Register() { }
};
class rune_deathknight_notre_dame : public AuraScript {
    PrepareAuraScript(rune_deathknight_notre_dame);  void Register() { }
};
class rune_deathknight_bombardment_specialist : public AuraScript {
    PrepareAuraScript(rune_deathknight_bombardment_specialist);  void Register() { }
};
class rune_deathknight_friend_of_death : public AuraScript {
    PrepareAuraScript(rune_deathknight_friend_of_death);  void Register() { }
};
class rune_deathknight_corpsekrieg : public AuraScript {
    PrepareAuraScript(rune_deathknight_corpsekrieg);  void Register() { }
};
class rune_deathknight_portal_to_the_underworld : public AuraScript {
    PrepareAuraScript(rune_deathknight_portal_to_the_underworld);  void Register() { }
};
class rune_deathknight_magus_of_the_dead : public AuraScript {
    PrepareAuraScript(rune_deathknight_magus_of_the_dead);  void Register() { }
};
class rune_deathknight_lasting_army : public AuraScript {
    PrepareAuraScript(rune_deathknight_lasting_army);  void Register() { }
};
class rune_deathknight_call_of_the_dead : public AuraScript {
    PrepareAuraScript(rune_deathknight_call_of_the_dead);  void Register() { }
};
class rune_deathknight_ruptured_viscera : public AuraScript {
    PrepareAuraScript(rune_deathknight_ruptured_viscera);  void Register() { }
};
class rune_deathknight_unholy_aura : public AuraScript {
    PrepareAuraScript(rune_deathknight_unholy_aura);  void Register() { }
};
class rune_deathknight_army_of_the_damned : public AuraScript {
    PrepareAuraScript(rune_deathknight_army_of_the_damned);  void Register() { }
};
class rune_deathknight_bursting_sores : public AuraScript {
    PrepareAuraScript(rune_deathknight_bursting_sores);  void Register() { }
};
class rune_deathknight_pestilence : public AuraScript {
    PrepareAuraScript(rune_deathknight_pestilence);  void Register() { }
};
class rune_deathknight_festermight : public AuraScript {
    PrepareAuraScript(rune_deathknight_festermight);  void Register() { }
};
class rune_deathknight_convocation_of_the_dead : public AuraScript {
    PrepareAuraScript(rune_deathknight_convocation_of_the_dead);  void Register() { }
};
class rune_deathknight_master_of_desease : public AuraScript {
    PrepareAuraScript(rune_deathknight_master_of_desease);  void Register() { }
};
class rune_deathknight_feasting_strikes : public AuraScript {
    PrepareAuraScript(rune_deathknight_feasting_strikes);  void Register() { }
};
class rune_deathknight_pestiferous : public AuraScript {
    PrepareAuraScript(rune_deathknight_pestiferous);  void Register() { }
};
class rune_deathknight_unholy_pact : public AuraScript {
    PrepareAuraScript(rune_deathknight_unholy_pact);  void Register() { }
};
class rune_deathknight_eternal_agony : public AuraScript {
    PrepareAuraScript(rune_deathknight_eternal_agony);  void Register() { }
};
class rune_deathknight_commander_of_the_dead : public AuraScript {
    PrepareAuraScript(rune_deathknight_commander_of_the_dead);  void Register() { }
};
class rune_deathknight_ghoulish_frenzy : public AuraScript {
    PrepareAuraScript(rune_deathknight_ghoulish_frenzy);  void Register() { }
};
class rune_deathknight_eternal_hunger : public AuraScript {
    PrepareAuraScript(rune_deathknight_eternal_hunger);  void Register() { }
};
class rune_deathknight_preparation : public AuraScript {
    PrepareAuraScript(rune_deathknight_preparation);  void Register() { }
};
class rune_deathknight_pandemic : public AuraScript {
    PrepareAuraScript(rune_deathknight_pandemic);  void Register() { }
};
class rune_deathknight_death_rot : public AuraScript {
    PrepareAuraScript(rune_deathknight_death_rot);  void Register() { }
};
class rune_deathknight_gift_of_knull : public AuraScript {
    PrepareAuraScript(rune_deathknight_gift_of_knull);  void Register() { }
};
class rune_deathknight_army_of_the_apocalypse : public AuraScript {
    PrepareAuraScript(rune_deathknight_army_of_the_apocalypse);  void Register() { }
};
class rune_deathknight_inevitable : public AuraScript {
    PrepareAuraScript(rune_deathknight_inevitable);  void Register() { }
};
class rune_deathknight_insanity : public AuraScript {
    PrepareAuraScript(rune_deathknight_insanity);  void Register() { }
};
class rune_deathknight_high_metabolism : public AuraScript {
    PrepareAuraScript(rune_deathknight_high_metabolism);  void Register() { }
};


void AddSC_deathknight_perks_scripts()
{
    RegisterSpellScript(rune_deathknight_diseased_attacks);
    RegisterSpellScript(rune_deathknight_runic_attenuation);
    RegisterSpellScript(rune_deathknight_sudden_doom);
    RegisterSpellScript(rune_deathknight_harbinger_of_doom);
    RegisterSpellScript(rune_deathknight_permafrost);
    RegisterSpellScript(rune_deathknight_frozen_pulse);
    RegisterSpellScript(rune_deathknight_insidious_chill);
    RegisterSpellScript(rune_deathknight_cold_steel);
    RegisterSpellScript(rune_deathknight_anti_magic_barrier);
    RegisterSpellScript(rune_deathknight_blood_feast);
    RegisterSpellScript(rune_deathknight_assimilation);
    RegisterSpellScript(rune_deathknight_reinforced_shell);
    RegisterSpellScript(rune_deathknight_amplification_zone);
    RegisterSpellScript(rune_deathknight_hammer_of_the_damned);
    RegisterSpellScript(rune_deathknight_blood_transfusion);
    RegisterSpellScript(rune_deathknight_boiling_blood);
    RegisterSpellScript(rune_deathknight_hemostasis);
    RegisterSpellScript(rune_deathknight_debilitating_malady);
    RegisterSpellScript(rune_deathknight_disease_and_decay);
    RegisterSpellScript(rune_deathknight_lasting_effects);
    RegisterSpellScript(rune_deathknight_hypothermia);
    RegisterSpellScript(rune_deathknight_infectious);
    RegisterSpellScript(rune_deathknight_brittle);
    RegisterSpellScript(rune_deathknight_crimson_scourge);
    RegisterSpellScript(rune_deathknight_ebon_fever);
    RegisterSpellScript(rune_deathknight_invigorating_freeze);
    RegisterSpellScript(rune_deathknight_morbidity);
    RegisterSpellScript(rune_deathknight_superstrain);
    RegisterSpellScript(rune_deathknight_lingering_plague);
    RegisterSpellScript(rune_deathknight_thassarians_teachings);
    RegisterSpellScript(rune_deathknight_proliferating_chill);
    RegisterSpellScript(rune_deathknight_cold_heart);
    RegisterSpellScript(rune_deathknight_explosion_of_death);
    RegisterSpellScript(rune_deathknight_hemoglobin);
    RegisterSpellScript(rune_deathknight_phearomones);
    RegisterSpellScript(rune_deathknight_quick_decay);
    RegisterSpellScript(rune_deathknight_aura_of_decay);
    RegisterSpellScript(rune_deathknight_grip_of_the_dead);
    RegisterSpellScript(rune_deathknight_rapid_decomposition);
    RegisterSpellScript(rune_deathknight_sanguine_ground);
    RegisterSpellScript(rune_deathknight_unholy_ground);
    RegisterSpellScript(rune_deathknight_deathly_empowerment);
    RegisterSpellScript(rune_deathknight_scourge_of_worlds);
    RegisterSpellScript(rune_deathknight_coil_of_devastation);
    RegisterSpellScript(rune_deathknight_improved_death_coil);
    RegisterSpellScript(rune_deathknight_clenching_grasp);
    RegisterSpellScript(rune_deathknight_voracious);
    RegisterSpellScript(rune_deathknight_soul_drinker);
    RegisterSpellScript(rune_deathknight_shackles_of_brindaor);
    RegisterSpellScript(rune_deathknight_blood_barrier);
    RegisterSpellScript(rune_deathknight_dine_and_dash);
    RegisterSpellScript(rune_deathknight_adaptation);
    RegisterSpellScript(rune_deathknight_death_tap);
    RegisterSpellScript(rune_deathknight_coagulopathy);
    RegisterSpellScript(rune_deathknight_death_bringer);
    RegisterSpellScript(rune_deathknight_endless_death);
    RegisterSpellScript(rune_deathknight_ice_veins);
    RegisterSpellScript(rune_deathknight_acclimation);
    RegisterSpellScript(rune_deathknight_heart_of_ice);
    RegisterSpellScript(rune_deathknight_unholy_endurance);
    RegisterSpellScript(rune_deathknight_coldthirst);
    RegisterSpellScript(rune_deathknight_the_unbeautiful);
    RegisterSpellScript(rune_deathknight_neverending_frenzy);
    RegisterSpellScript(rune_deathknight_all_will_serve);
    RegisterSpellScript(rune_deathknight_enfeeble);
    RegisterSpellScript(rune_deathknight_runic_empowerment);
    RegisterSpellScript(rune_deathknight_unleashed_frenzy);
    RegisterSpellScript(rune_deathknight_runic_mastery);
    RegisterSpellScript(rune_deathknight_rune_mastery);
    RegisterSpellScript(rune_deathknight_unholy_bond);
    RegisterSpellScript(rune_deathknight_march_of_darkness);
    RegisterSpellScript(rune_deathknight_cold_soul);
    RegisterSpellScript(rune_deathknight_blood_draw);
    RegisterSpellScript(rune_deathknight_will_of_the_necropolis);
    RegisterSpellScript(rune_deathknight_purgatory);
    RegisterSpellScript(rune_deathknight_bloody_armor);
    RegisterSpellScript(rune_deathknight_blood_eater);
    RegisterSpellScript(rune_deathknight_ossuary);
    RegisterSpellScript(rune_deathknight_foul_bulwark);
    RegisterSpellScript(rune_deathknight_reinforced_bones);
    RegisterSpellScript(rune_deathknight_improved_bone_shield);
    RegisterSpellScript(rune_deathknight_shattering_bone);
    RegisterSpellScript(rune_deathknight_runic_bones);
    RegisterSpellScript(rune_deathknight_bloodshot);
    RegisterSpellScript(rune_deathknight_iron_heart);
    RegisterSpellScript(rune_deathknight_dwarven_runes);
    RegisterSpellScript(rune_deathknight_eternal_rune_weapon);
    RegisterSpellScript(rune_deathknight_elven_runes);
    RegisterSpellScript(rune_deathknight_insatiable_blade);
    RegisterSpellScript(rune_deathknight_everlasting_bond);
    RegisterSpellScript(rune_deathknight_heart_grasp);
    RegisterSpellScript(rune_deathknight_doomed_heart);
    RegisterSpellScript(rune_deathknight_withering_plague);
    RegisterSpellScript(rune_deathknight_heartrend);
    RegisterSpellScript(rune_deathknight_leeching_strike);
    RegisterSpellScript(rune_deathknight_collective_hysteria);
    RegisterSpellScript(rune_deathknight_runic_defense);
    RegisterSpellScript(rune_deathknight_runic_transfusion);
    RegisterSpellScript(rune_deathknight_ice_thirst);
    RegisterSpellScript(rune_deathknight_red_thirst);
    RegisterSpellScript(rune_deathknight_blood_bath);
    RegisterSpellScript(rune_deathknight_vampire_tribe);
    RegisterSpellScript(rune_deathknight_hungry_for_blood);
    RegisterSpellScript(rune_deathknight_service_of_gorefiend);
    RegisterSpellScript(rune_deathknight_unending_thirst);
    RegisterSpellScript(rune_deathknight_umbilicus_eternus);
    RegisterSpellScript(rune_deathknight_hardened_blood);
    RegisterSpellScript(rune_deathknight_tightening_grasp);
    RegisterSpellScript(rune_deathknight_perseverance_of_the_ebon_blade);
    RegisterSpellScript(rune_deathknight_made_of_rock);
    RegisterSpellScript(rune_deathknight_torment);
    RegisterSpellScript(rune_deathknight_life_siphon);
    RegisterSpellScript(rune_deathknight_relish_in_blood);
    RegisterSpellScript(rune_deathknight_blood_lover);
    RegisterSpellScript(rune_deathknight_damnation);
    RegisterSpellScript(rune_deathknight_cold_sensitivity);
    RegisterSpellScript(rune_deathknight_frozen_talon);
    RegisterSpellScript(rune_deathknight_shattering_blade);
    RegisterSpellScript(rune_deathknight_runic_blast);
    RegisterSpellScript(rune_deathknight_master_blaster);
    RegisterSpellScript(rune_deathknight_echoing_howl);
    RegisterSpellScript(rune_deathknight_murderous_efficiency);
    RegisterSpellScript(rune_deathknight_razors_edge);
    RegisterSpellScript(rune_deathknight_cyclopean_focus);
    RegisterSpellScript(rune_deathknight_bonegrinder);
    RegisterSpellScript(rune_deathknight_chain_killing);
    RegisterSpellScript(rune_deathknight_rage_of_the_frozen_champion);
    RegisterSpellScript(rune_deathknight_icebreaker);
    RegisterSpellScript(rune_deathknight_avalanche);
    RegisterSpellScript(rune_deathknight_improved_rime);
    RegisterSpellScript(rune_deathknight_inexorable_assault);
    RegisterSpellScript(rune_deathknight_koltiras_favor);
    RegisterSpellScript(rune_deathknight_frost_blade);
    RegisterSpellScript(rune_deathknight_decaying_obliteration);
    RegisterSpellScript(rune_deathknight_frigid_executioner);
    RegisterSpellScript(rune_deathknight_eradicating_blow);
    RegisterSpellScript(rune_deathknight_obliteration);
    RegisterSpellScript(rune_deathknight_eternal_pillar);
    RegisterSpellScript(rune_deathknight_frostwhelps_breath);
    RegisterSpellScript(rune_deathknight_enduring_strength);
    RegisterSpellScript(rune_deathknight_icecap);
    RegisterSpellScript(rune_deathknight_impending_doom);
    RegisterSpellScript(rune_deathknight_soul_harvest);
    RegisterSpellScript(rune_deathknight_biting_cold);
    RegisterSpellScript(rune_deathknight_everfrost);
    RegisterSpellScript(rune_deathknight_gathering_storm);
    RegisterSpellScript(rune_deathknight_piercing_chill);
    RegisterSpellScript(rune_deathknight_enduring_chill);
    RegisterSpellScript(rune_deathknight_accelerated_cold);
    RegisterSpellScript(rune_deathknight_absolute_zero);
    RegisterSpellScript(rune_deathknight_chilled_to_the_bone);
    RegisterSpellScript(rune_deathknight_harvest_time);
    RegisterSpellScript(rune_deathknight_necrosis);
    RegisterSpellScript(rune_deathknight_plaguebringer);
    RegisterSpellScript(rune_deathknight_rotten_touch);
    RegisterSpellScript(rune_deathknight_reaping);
    RegisterSpellScript(rune_deathknight_all_will_do);
    RegisterSpellScript(rune_deathknight_light_and_dark);
    RegisterSpellScript(rune_deathknight_notre_dame);
    RegisterSpellScript(rune_deathknight_bombardment_specialist);
    RegisterSpellScript(rune_deathknight_friend_of_death);
    RegisterSpellScript(rune_deathknight_corpsekrieg);
    RegisterSpellScript(rune_deathknight_portal_to_the_underworld);
    RegisterSpellScript(rune_deathknight_magus_of_the_dead);
    RegisterSpellScript(rune_deathknight_lasting_army);
    RegisterSpellScript(rune_deathknight_call_of_the_dead);
    RegisterSpellScript(rune_deathknight_ruptured_viscera);
    RegisterSpellScript(rune_deathknight_unholy_aura);
    RegisterSpellScript(rune_deathknight_army_of_the_damned);
    RegisterSpellScript(rune_deathknight_bursting_sores);
    RegisterSpellScript(rune_deathknight_pestilence);
    RegisterSpellScript(rune_deathknight_festermight);
    RegisterSpellScript(rune_deathknight_convocation_of_the_dead);
    RegisterSpellScript(rune_deathknight_master_of_desease);
    RegisterSpellScript(rune_deathknight_feasting_strikes);
    RegisterSpellScript(rune_deathknight_pestiferous);
    RegisterSpellScript(rune_deathknight_unholy_pact);
    RegisterSpellScript(rune_deathknight_eternal_agony);
    RegisterSpellScript(rune_deathknight_commander_of_the_dead);
    RegisterSpellScript(rune_deathknight_ghoulish_frenzy);
    RegisterSpellScript(rune_deathknight_eternal_hunger);
    RegisterSpellScript(rune_deathknight_preparation);
    RegisterSpellScript(rune_deathknight_pandemic);
    RegisterSpellScript(rune_deathknight_death_rot);
    RegisterSpellScript(rune_deathknight_gift_of_knull);
    RegisterSpellScript(rune_deathknight_army_of_the_apocalypse);
    RegisterSpellScript(rune_deathknight_inevitable);
    RegisterSpellScript(rune_deathknight_insanity);
    RegisterSpellScript(rune_deathknight_high_metabolism);
}
