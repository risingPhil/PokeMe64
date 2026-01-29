#include "menu/PokeShopEntries.h"
#include "Moves.h"
#include "gen1/Gen1Localization.h"
#include "gen2/Gen2Localization.h"

static const Gen1DistributionPokemon g1_starter_Charmander = {
    .name = "Charmander",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0xB0,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 20, // FIRE
        .type2 = 20, // FIRE
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::SCRATCH,
        .index_move2 = (uint8_t)Move::GROWL,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_starter_Squirtle = {
    .name = "Squirtle",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0xB1,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 21, // WATER
        .type2 = 21, // WATER
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_starter_Bulbasaur = {
    .name = "Bulbasaur",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x99,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 22, // GRASS
        .type2 = 22, // GRASS
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::GROWL,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_mutex_Eevee = {
    .name = "Eevee",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x66,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0, // NORMAL
        .type2 = 0, // NORMAL
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::SAND_ATTACK,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 15,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_mutex_Kabuto = {
    .name = "Kabuto",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x5A,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0x5, // ROCK
        .type2 = 0x15, // WATER
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::HARDEN,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_mutex_Omanyte = {
    .name = "Omanyte",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x62,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0x5, // ROCK
        .type2 = 0x15, // WATER
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::WATER_GUN,
        .index_move2 = (uint8_t)Move::WITHDRAW,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_mutex_Hitmonlee = {
    .name = "Hitmonlee",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x2B,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0x01, // FIGHTING
        .type2 = 0x01, // FIGHTING
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::DOUBLE_KICK,
        .index_move2 = (uint8_t)Move::MEDITATE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_mutex_Hitmonchan = {
    .name = "Hitmonchan",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x2C,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0x01, // FIGHTING
        .type2 = 0x01, // FIGHTING
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::AGILITY,
        .index_move2 = (uint8_t)Move::COMET_PUNCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 15,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Weedle = {
    .name = "Weedle",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x70,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 7, // BUG
        .type2 = 3, // POISON
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::POISON_STING,
        .index_move2 = (uint8_t)Move::STRING_SHOT,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Ekans = {
    .name = "Ekans",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x6C,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 3, // POISON
        .type2 = 3, // POISON
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::WRAP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 20,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Pikachu = {
    .name = "Pikachu",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x54,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 23, // ELECTRIC
        .type2 = 23, // ELECTRIC
        .catch_rate_or_held_item = 0xBE,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::THUNDER_SHOCK,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Sandshrew = {
    .name = "Sandshrew",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x60,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 4, // GROUND
        .type2 = 4, // GROUND
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::SCRATCH,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Vulpix = {
    .name = "Vulpix",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x52,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 20, // FIRE
        .type2 = 20, // FIRE
        .catch_rate_or_held_item = 0xBE,
        .index_move1 = (uint8_t)Move::EMBER,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Oddish = {
    .name = "Oddish",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0xB9,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 22, // GRASS
        .type2 = 22, // GRASS
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::ABSORB,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Meowth = {
    .name = "Meowth",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x4D,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 0, // NORMAL
        .type2 = 0, // NORMAL
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Mankey = {
    .name = "Mankey",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x39,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 1, // FIGHTING
        .type2 = 1, // FIGHTING
        .catch_rate_or_held_item = 0xBE,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Growlithe = {
    .name = "Growlithe",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x21,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 20, // FIRE
        .type2 = 20, // FIRE
        .catch_rate_or_held_item = 0xBE,
        .index_move1 = (uint8_t)Move::BITE,
        .index_move2 = (uint8_t)Move::ROAR,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 20,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Bellsprout = {
    .name = "Bellsprout",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0xBC,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 22, // GRASS
        .type2 = 22, // GRASS
        .catch_rate_or_held_item = 0xFF,
        .index_move1 = (uint8_t)Move::GROWTH,
        .index_move2 = (uint8_t)Move::VINE_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 20,
        .pp_move2 = 25,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Koffing = {
    .name = "Koffing",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x37,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 3, // POISON
        .type2 = 3, // POISON
        .catch_rate_or_held_item = 0xBE,
        .index_move1 = (uint8_t)Move::SMOG,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 20,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Scyther = {
    .name = "Scyther",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x1A,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 7, // BUG
        .type2 = 2, // FLYING
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::QUICK_ATTACK,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Jynx = {
    .name = "Jynx",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x48,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 25, // ICE
        .type2 = 24, // PSYCHIC
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::LOVELY_KISS,
        .index_move2 = (uint8_t)Move::POUND,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 10,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Electabuzz = {
    .name = "Electabuzz",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x35,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 23, // ELECTRIC
        .type2 = 23, // ELECTRIC
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::QUICK_ATTACK,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Magmar = {
    .name = "Magmar",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x33,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 20, // FIRE
        .type2 = 20, // FIRE
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::EMBER,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen1DistributionPokemon g1_versionexclusive_Pinsir = {
    .name = "Pinsir",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .iv_data = {0, 0},
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .isJapanese = false,
    .held_item_override = 0,
    .poke = {
        .poke_index = 0x1D,
        .current_hp = 1,
        .level = 5,
        .status_condition = 0,
        .type1 = 7, // BUG
        .type2 = 7, // BUG
        .catch_rate_or_held_item = 0x2D,
        .index_move1 = (uint8_t)Move::VICE_GRIP,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GRW_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Cyndaquil = {
    .name = "Cyndaquil",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x9B,
        .held_item_index = 0xAD, // Berry
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Totodile = {
    .name = "Totodile",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x9E,
        .held_item_index = 0xAD, // Berry
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Chikorita = {
    .name = "Chikorita",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x98,
        .held_item_index = 0xAD, // Berry
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Charmander = {
    .name = "Charmander",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x04,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Squirtle = {
    .name = "Squirtle",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x07,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_starter_Bulbasaur = {
    .name = "Bulbasaur",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x01,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::GROWL,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_mutex_Eevee = {
    .name = "Eevee",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x85,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_mutex_Tyrogue = {
    .name = "Tyrogue",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xEC,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Vulpix = {
    .name = "Vulpix",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x25,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::EMBER,
        .index_move2 = (uint8_t)Move::TAIL_WHIP,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 30,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Meowth = {
    .name = "Meowth",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x34,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Mankey = {
    .name = "Mankey",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x38,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Growlithe = {
    .name = "Growlithe",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x3A,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::BITE,
        .index_move2 = (uint8_t)Move::ROAR,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 20,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Ledyba = {
    .name = "Ledyba",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xA5,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::TACKLE,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Spinarak = {
    .name = "Spinarak",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xA7,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::POISON_STING,
        .index_move2 = (uint8_t)Move::STRING_SHOT,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Mareep = {
    .name = "Mareep",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xB3,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Girafarig = {
    .name = "Girafarig",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xCB,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::CONFUSION,
        .index_move2 = (uint8_t)Move::GROWL,
        .index_move3 = (uint8_t)Move::STOMP,
        .index_move4 = (uint8_t)Move::TACKLE,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 40,
        .pp_move3 = 20,
        .pp_move4 = 35,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Gligar = {
    .name = "Gligar",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xCF,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::POISON_STING,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Teddiursa = {
    .name = "Teddiursa",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xD8,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Remoraid = {
    .name = "Remoraid",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xDF,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::WATER_GUN,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 25,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Delibird = {
    .name = "Delibird",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xE1,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::PRESENT,
        .index_move2 = 0,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 15,
        .pp_move2 = 0,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Mantine = {
    .name = "Mantine",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xE2,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::BUBBLE,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Skarmory = {
    .name = "Skarmory",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xE3,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::LEER,
        .index_move2 = (uint8_t)Move::PECK,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_SLOW),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Phanpy = {
    .name = "Phanpy",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0xE7,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::GROWL,
        .index_move2 = (uint8_t)Move::TACKLE,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 40,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Kabuto = {
    .name = "Kabuto",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x8C,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::HARDEN,
        .index_move2 = (uint8_t)Move::SCRATCH,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 30,
        .pp_move2 = 35,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static const Gen2DistributionPokemon g2_versionexclusive_Omanyte = {
    .name = "Omanyte",
    .originalTrainer = "",
    .originalTrainerID = 0,
    .setPlayerAsOriginalTrainer = true,
    .regenerateTrainerID = false,
    .shinyChance = 0,
    .overrideLevel = 0,
    .isEgg = false,
    .iv_data = {0, 0},
    .randomizeIVs = true,
    .isJapanese = false,
    .poke = {
        .poke_index = 0x8A,
        .held_item_index = 0,
        .index_move1 = (uint8_t)Move::CONSTRICT,
        .index_move2 = (uint8_t)Move::WITHDRAW,
        .index_move3 = 0,
        .index_move4 = 0,
        .original_trainer_ID = 0,
        .exp = getExpNeededForLevel(5, GEN2_GRW_MEDIUM_FAST),
        .hp_effort_value = 0,
        .atk_effort_value = 0,
        .def_effort_value = 0,
        .speed_effort_value = 0,
        .special_effort_value = 0,
        .iv_data = {0, 0},
        .pp_move1 = 35,
        .pp_move2 = 40,
        .pp_move3 = 0,
        .pp_move4 = 0,
        .friendship_or_remaining_egg_cycles = 70,
        .pokerus = 0,
        .caught_data = 0,
        .level = 5,
        .status_condition = 0,
        .unused_byte = 0,
        .current_hp = 0,
        .max_hp = 0,
        .atk = 0,
        .def = 0,
        .speed = 0,
        .special_atk = 0,
        .special_def = 0
    }
};

static Gen1PokeShopEntry dummyGen1Entries[] = {};
static Gen2PokeShopEntry dummyGen2Entries[] = {};

static Gen1PokeShopEntry g1_shop_red_entries[] = {
    {
        .pokemon = &g1_starter_Charmander,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Squirtle,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Bulbasaur,
        .price = 10000
    },
    {
        .pokemon = &g1_versionexclusive_Sandshrew,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Vulpix,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Meowth,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Bellsprout,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Magmar,
        .price = 7500
    },
    {
        .pokemon = &g1_versionexclusive_Pinsir,
        .price = 7500
    },
    {
        .pokemon = &g1_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g1_mutex_Hitmonlee,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Hitmonchan,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Kabuto,
        .price = 30000
    },
    {
        .pokemon = &g1_mutex_Omanyte,
        .price = 30000
    }
};

static Gen1PokeShopEntry g1_shop_green_entries[] = {
    {
        .pokemon = &g1_starter_Charmander,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Squirtle,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Bulbasaur,
        .price = 10000
    },
    {
        .pokemon = &g1_versionexclusive_Ekans,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Oddish,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Mankey,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Growlithe,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Scyther,
        .price = 7500
    },
    {
        .pokemon = &g1_versionexclusive_Electabuzz,
        .price = 7500
    },
    {
        .pokemon = &g1_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g1_mutex_Hitmonlee,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Hitmonchan,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Kabuto,
        .price = 30000
    },
    {
        .pokemon = &g1_mutex_Omanyte,
        .price = 30000
    }
};

static Gen1PokeShopEntry g1_shop_jpn_blue_entries[] = {
    {
        .pokemon = &g1_starter_Charmander,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Squirtle,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Bulbasaur,
        .price = 10000
    },
    {
        .pokemon = &g1_versionexclusive_Ekans,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Vulpix,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Mankey,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Bellsprout,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Electabuzz,
        .price = 7500
    },
    {
        .pokemon = &g1_versionexclusive_Magmar,
        .price = 7500
    },
    {
        .pokemon = &g1_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g1_mutex_Hitmonlee,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Hitmonchan,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Kabuto,
        .price = 30000
    },
    {
        .pokemon = &g1_mutex_Omanyte,
        .price = 30000
    }
};

static Gen1PokeShopEntry g1_shop_yellow_entries[] = {
    {
        .pokemon = &g1_starter_Charmander,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Squirtle,
        .price = 10000
    },
    {
        .pokemon = &g1_starter_Bulbasaur,
        .price = 10000
    },
    {
        .pokemon = &g1_versionexclusive_Weedle,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Ekans,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Pikachu,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Meowth,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Koffing,
        .price = 5000
    },
    {
        .pokemon = &g1_versionexclusive_Jynx,
        .price = 7500
    },
    {
        .pokemon = &g1_versionexclusive_Electabuzz,
        .price = 7500
    },
    {
        .pokemon = &g1_versionexclusive_Magmar,
        .price = 7500
    },
    {
        .pokemon = &g1_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g1_mutex_Hitmonlee,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Hitmonchan,
        .price = 20000
    },
    {
        .pokemon = &g1_mutex_Kabuto,
        .price = 30000
    },
    {
        .pokemon = &g1_mutex_Omanyte,
        .price = 30000
    }
};

static Gen2PokeShopEntry g2_shop_gold_entries[] = {
    {
        .pokemon = &g2_starter_Cyndaquil,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Totodile,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Chikorita,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Charmander,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Squirtle,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Bulbasaur,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Vulpix,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Meowth,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Ledyba,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Delibird,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Skarmory,
        .price = 7500
    },
    {
        .pokemon = &g2_versionexclusive_Phanpy,
        .price = 7500
    },
    {
        .pokemon = &g2_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g2_mutex_Tyrogue,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Kabuto,
        .price = 40000
    },
    {
        .pokemon = &g2_versionexclusive_Omanyte,
        .price = 40000
    }
};

static Gen2PokeShopEntry g2_shop_silver_entries[] = {
    {
        .pokemon = &g2_starter_Cyndaquil,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Totodile,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Chikorita,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Charmander,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Squirtle,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Bulbasaur,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Mankey,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Growlithe,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Spinarak,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Gligar,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Mantine,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Teddiursa,
        .price = 7500
    },
    {
        .pokemon = &g2_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g2_mutex_Tyrogue,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Kabuto,
        .price = 40000
    },
    {
        .pokemon = &g2_versionexclusive_Omanyte,
        .price = 40000
    }
};

static Gen2PokeShopEntry g2_shop_crystal_entries[] = {
    {
        .pokemon = &g2_starter_Cyndaquil,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Totodile,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Chikorita,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Charmander,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Squirtle,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Bulbasaur,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Vulpix,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Mankey,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Mareep,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Remoraid,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Girafarig,
        .price = 7500
    },
    {
        .pokemon = &g2_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g2_mutex_Tyrogue,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Kabuto,
        .price = 40000
    },
    {
        .pokemon = &g2_versionexclusive_Omanyte,
        .price = 40000
    }
};

static Gen2PokeShopEntry g2_shop_jpn_kor_gold_entries[] = {
    {
        .pokemon = &g2_starter_Cyndaquil,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Totodile,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Chikorita,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Charmander,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Squirtle,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Bulbasaur,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Vulpix,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Meowth,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Ledyba,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Delibird,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Skarmory,
        .price = 7500
    },
    {
        .pokemon = &g2_versionexclusive_Teddiursa,
        .price = 7500
    },
    {
        .pokemon = &g2_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g2_mutex_Tyrogue,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Kabuto,
        .price = 40000
    },
    {
        .pokemon = &g2_versionexclusive_Omanyte,
        .price = 40000
    }
};

static Gen2PokeShopEntry g2_shop_jpn_kor_silver_entries[] = {
    {
        .pokemon = &g2_starter_Cyndaquil,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Totodile,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Chikorita,
        .price = 10000
    },
    {
        .pokemon = &g2_starter_Charmander,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Squirtle,
        .price = 12500
    },
    {
        .pokemon = &g2_starter_Bulbasaur,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Mankey,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Growlithe,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Spinarak,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Gligar,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Mantine,
        .price = 5000
    },
    {
        .pokemon = &g2_versionexclusive_Phanpy,
        .price = 7500
    },
    {
        .pokemon = &g2_mutex_Eevee,
        .price = 10000
    },
    {
        .pokemon = &g2_mutex_Tyrogue,
        .price = 12500
    },
    {
        .pokemon = &g2_versionexclusive_Kabuto,
        .price = 40000
    },
    {
        .pokemon = &g2_versionexclusive_Omanyte,
        .price = 40000
    }
};

void getGen1PokeShopEntries(uint8_t gameVariant, uint8_t localization, const Gen1PokeShopEntry *&entries, uint32_t &outNumEntries)
{
    // The version exclusives for the international Blue release are the same as the Japanese Green release.
    // But there's a dedicated set of exclusives for the Japanese Blue release.
    // source: https://bulbapedia.bulbagarden.net/wiki/Version-exclusive_Pok%C3%A9mon#Generation_I
    if( static_cast<Gen1LocalizationLanguage>(localization) == Gen1LocalizationLanguage::JAPANESE && 
        static_cast<Gen1GameType>(gameVariant) == Gen1GameType::BLUE)
    {
        entries = g1_shop_jpn_blue_entries;
        outNumEntries = sizeof(g1_shop_jpn_blue_entries) / sizeof(Gen1PokeShopEntry);
        return;
    }

    switch(static_cast<Gen1GameType>(gameVariant))
    {
    case Gen1GameType::RED:
        entries = g1_shop_red_entries;
        outNumEntries = sizeof(g1_shop_red_entries) / sizeof(Gen1PokeShopEntry);
        break;
    // International Blue has the same exclusives as Green.
    // see comment above
    case Gen1GameType::GREEN:
    case Gen1GameType::BLUE:
        entries = g1_shop_green_entries;
        outNumEntries = sizeof(g1_shop_green_entries) / sizeof(Gen1PokeShopEntry);
        break;
    case Gen1GameType::YELLOW:
        entries = g1_shop_yellow_entries;
        outNumEntries = sizeof(g1_shop_yellow_entries) / sizeof(Gen1PokeShopEntry);
        break;
    default:
        entries = dummyGen1Entries;
        outNumEntries = 0;
        break;
    }
}

void getGen2PokeShopEntries(uint8_t gameVariant, uint8_t localization, const Gen2PokeShopEntry *&entries, uint32_t &outNumEntries)
{
    // While it's a bit silly to duplicate the switch statements specifically for Japanese/Korean,
    // it's the easiest way to keep things clear and avoid mistakes.
    // Even though the only real difference between these and the international versions is the fact that
    // Phanpy and Teddiursa are swapped between Silver and Gold.
    if( static_cast<Gen2LocalizationLanguage>(localization) == Gen2LocalizationLanguage::JAPANESE ||
        static_cast<Gen2LocalizationLanguage>(localization) == Gen2LocalizationLanguage::KOREAN)
    {
        switch(static_cast<Gen2GameType>(gameVariant))
        {
        case Gen2GameType::GOLD:
            entries = g2_shop_jpn_kor_gold_entries;
            outNumEntries = sizeof(g2_shop_jpn_kor_gold_entries) / sizeof(Gen2PokeShopEntry);
            break;
        case Gen2GameType::SILVER:
            entries = g2_shop_jpn_kor_silver_entries;
            outNumEntries = sizeof(g2_shop_jpn_kor_silver_entries) / sizeof(Gen2PokeShopEntry);
            break;
        case Gen2GameType::CRYSTAL:
            entries = g2_shop_crystal_entries;
            outNumEntries = sizeof(g2_shop_crystal_entries) / sizeof(Gen2PokeShopEntry);
            break;
        default:
            entries = dummyGen2Entries;
            outNumEntries = 0;
            break;
        }
        return;
    }
    else
    {
        switch(static_cast<Gen2GameType>(gameVariant))
        {
        case Gen2GameType::GOLD:
            entries = g2_shop_gold_entries;
            outNumEntries = sizeof(g2_shop_gold_entries) / sizeof(Gen2PokeShopEntry);
            break;
        case Gen2GameType::SILVER:
            entries = g2_shop_silver_entries;
            outNumEntries = sizeof(g2_shop_silver_entries) / sizeof(Gen2PokeShopEntry);
            break;
        case Gen2GameType::CRYSTAL:
            entries = g2_shop_crystal_entries;
            outNumEntries = sizeof(g2_shop_crystal_entries) / sizeof(Gen2PokeShopEntry);
            break;
        default:
            entries = dummyGen2Entries;
            outNumEntries = 0;
            break;
        }
    }
}