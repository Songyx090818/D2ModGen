/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#include "ModuleCube.hpp"

#include "AttributeHelper.hpp"
#include "TableUtils.hpp"

namespace D2ModGen {

namespace {
const bool s_init = registerHelper<ModuleCube>();

const std::map<std::string, std::string> s_craftedTypeReplace{
    { "\"fhl,mag,upg\"", "\"helm,mag\"" },
    { "\"mbt,mag,upg\"", "\"boot,mag\"" },
    { "\"mgl,mag,upg\"", "\"glov,mag\"" },
    { "\"tbl,mag,upg\"", "\"belt,mag\"" },
    { "\"gts,mag,upg\"", "\"shld,mag\"" },
    { "\"fld,mag,upg\"", "\"tors,mag\"" },
    { "\"blun,mag\"", "\"weap,mag\"" },

    { "\"hlm,mag,upg\"", "\"helm,mag\"" },
    { "\"tbt,mag,upg\"", "\"boot,mag\"" },
    { "\"vgl,mag,upg\"", "\"glov,mag\"" },
    { "\"mbl,mag,upg\"", "\"belt,mag\"" },
    { "\"spk,mag,upg\"", "\"shld,mag\"" },
    { "\"plt,mag,upg\"", "\"tors,mag\"" },
    { "\"axe,mag\"", "\"weap,mag\"" },

    { "\"msk,mag,upg\"", "\"helm,mag\"" },
    { "\"lbt,mag,upg\"", "\"boot,mag\"" },
    { "\"lgl,mag,upg\"", "\"glov,mag\"" },
    { "\"vbl,mag,upg\"", "\"belt,mag\"" },
    { "\"sml,mag,upg\"", "\"shld,mag\"" },
    { "\"ltp,mag,upg\"", "\"tors,mag\"" },
    { "\"rod,mag\"", "\"weap,mag\"" },

    { "\"crn,mag,upg\"", "\"helm,mag\"" },
    { "\"hbt,mag,upg\"", "\"boot,mag\"" },
    { "\"hgl,mag,upg\"", "\"glov,mag\"" },
    { "\"lbl,mag,upg\"", "\"belt,mag\"" },
    { "\"kit,mag,upg\"", "\"shld,mag\"" },
    { "\"brs,mag,upg\"", "\"tors,mag\"" },
    { "\"spea,mag\"", "\"miss,mag\"" },
};

const std::map<std::string, std::string> s_craftedGemReplace{
    { "gpb", "gems" },
    { "gpr", "gemr" },
    { "gpv", "gema" },
    { "gpg", "geme" },
    { "gpy", "gemt" },
    { "skz", "gemz" },
};

}

void ModuleCube::generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const
{
    if (input.isAllDefault())
        return;

    const bool noGemUpgrade  = input.getInt("noGemUpgrade");
    const bool quickPortals  = input.getInt("quickPortals");
    const bool quickQuests   = input.getInt("quickQuests");
    const bool newSocketing  = input.getInt("socketing");
    const bool upgrading     = input.getInt("upgrading");
    const bool craftNoStrict = input.getInt("craftNoStrict");
    const bool craftNoRunes  = input.getInt("craftNoRunes");
    const bool craftHighIlvl = input.getInt("craftHighIlvl");
    const bool cheatCharms   = input.getInt("cheatCharms");

    auto&     tableSet = output.tableSet;
    TableView view(tableSet.tables[TableId::cubemain], true);
    for (auto& row : view) {
        auto&        description = row["description"];
        auto&        numinputs   = row["numinputs"];
        std::string& input1      = row["input 1"].str;
        std::string& input2      = row["input 2"].str;
        std::string& input3      = row["input 3"].str;
        std::string& input4      = row["input 4"].str;
        const bool   isCrafted   = row["output"].str == "\"usetype,crf\"";

        if (noGemUpgrade && description.contains("->") && description.endsWith(" Rune") && !input2.empty()) {
            numinputs.setInt(numinputs.toInt() - 1);
            input2 = "";
        }
        if (craftHighIlvl && (isCrafted || !row["ilvl"].isEmpty() || !row["plvl"].isEmpty() || !row["lvl"].isEmpty())) {
            row["ilvl"].str = "";
            row["plvl"].str = "";
            row["lvl"].str  = "99";
        }
        if (isCrafted) {
            if (craftNoStrict)
                input1 = mapValue(s_craftedTypeReplace, input1, input1);

            if (craftNoRunes) {
                input2 = input4;
                input3 = input4 = "";
                numinputs.setInt(2);
                input2 = mapValue(s_craftedGemReplace, input2, input2);
            }
        }
    }
    using StringMap = TableView::RowValues;
    if (quickPortals) {
        const StringMap base{
            { "description", "Quick Portal" },
            { "enabled", "1" },
            { "version", "100" },
            { "op", "28" },
            { "*eol", "0" },
            { "input 1", "tbk" },

        };
        StringMap cows    = base;
        cows["numinputs"] = "2";
        cows["input 2"]   = "isc";
        cows["output"]    = "Cow Portal";

        StringMap uber1    = base;
        uber1["numinputs"] = "3";
        uber1["input 2"]   = "\"isc,qty=2\"";
        uber1["output"]    = "Pandemonium Portal";

        StringMap uber2    = base;
        uber2["numinputs"] = "4";
        uber2["input 2"]   = "\"isc,qty=3\"";
        uber2["output"]    = "Pandemonium Finale Portal";

        view.appendRow(cows);
        view.appendRow(uber1);
        view.appendRow(uber2);
    }
    if (quickQuests) {
        const StringMap base{
            { "description", "Quick Quest" },
            { "enabled", "1" },
            { "version", "0" },
            { "op", "28" },
            { "*eol", "0" },
            { "input 1", "key" },
            { "numinputs", "2" },

        };
        StringMap staff  = base;
        staff["input 2"] = "isc";
        staff["output"]  = "hst";

        StringMap khalim  = base;
        khalim["input 2"] = "tsc";
        khalim["output"]  = "qf2";

        view.appendRow(staff);
        view.appendRow(khalim);
    }
    if (newSocketing) {
        const StringMap base{
            { "description", "Quick Sockets" },
            { "enabled", "1" },
            { "version", "100" },
            { "*eol", "0" },
            { "input 1", "\"any,nor,nos\"" },

        };
        StringMap socket3    = base;
        socket3["numinputs"] = "3";
        socket3["input 2"]   = "tsc";
        socket3["input 3"]   = "isc";
        socket3["output"]    = "\"useitem,sock=3\"";

        StringMap socket4    = base;
        socket4["numinputs"] = "4";
        socket4["input 2"]   = "tsc";
        socket4["input 3"]   = "\"isc,qty=2\"";
        socket4["output"]    = "\"useitem,sock=4\"";

        StringMap socket5    = base;
        socket5["numinputs"] = "4";
        socket5["input 2"]   = "\"tsc,qty=2\"";
        socket5["input 3"]   = "isc";
        socket5["output"]    = "\"useitem,sock=5\"";

        StringMap socket6    = base;
        socket6["numinputs"] = "5";
        socket6["input 2"]   = "\"tsc,qty=2\"";
        socket6["input 3"]   = "\"isc,qty=2\"";
        socket6["output"]    = "\"useitem,sock=6\"";

        StringMap socket1    = base;
        socket1["numinputs"] = "3";
        socket1["input 1"]   = "\"any,uni,nos\"";
        socket1["input 2"]   = "tsc";
        socket1["input 3"]   = "isc";
        socket1["output"]    = "\"useitem,sock=1\"";

        StringMap socket1a  = socket1;
        socket1a["input 1"] = "\"any,rar,nos\"";

        StringMap socket1b  = socket1;
        socket1b["input 1"] = "\"any,set,nos\"";

        StringMap socketClear    = base;
        socketClear["numinputs"] = "2";
        socketClear["input 1"]   = "\"any,sock\"";
        socketClear["input 2"]   = "tsc";
        socketClear["output"]    = "\"useitem,uns\"";

        view.appendRow(socket3);
        view.appendRow(socket4);
        view.appendRow(socket5);
        view.appendRow(socket6);
        view.appendRow(socket1);
        view.appendRow(socket1a);
        view.appendRow(socket1b);
        view.appendRow(socketClear);
    }

    if (upgrading) {
        StringMap base{
            { "description", "Quick Upgrading" },
            { "enabled", "1" },
            { "version", "100" },
            { "*eol", "0" },
            { "numinputs", "2" },
            { "ilvl", "100" },
            { "input 2", "yps" },

        };
        if (craftHighIlvl) {
            base["ilvl"] = "";
            base["plvl"] = "";
            base["lvl"]  = "99";
        }
        StringMap armo1  = base;
        armo1["input 1"] = "\"armo,bas\"";
        armo1["output"]  = "\"useitem,mod,exc\"";

        StringMap weap1  = armo1;
        weap1["input 1"] = "\"weap,bas\"";

        StringMap armo2  = base;
        armo2["input 1"] = "\"armo,exc\"";
        armo2["output"]  = "\"useitem,mod,eli\"";

        StringMap weap2  = armo2;
        weap2["input 1"] = "\"weap,exc\"";

        StringMap armoEth      = base;
        armoEth["numinputs"]   = "3";
        armoEth["input 1"]     = "\"armo,noe\"";
        armoEth["input 2"]     = "\"vps,qty=2\"";
        armoEth["output"]      = "\"useitem\"";
        armoEth["mod 1"]       = "ethereal";
        armoEth["mod 1 param"] = "1";
        armoEth["mod 1 min"]   = "1";
        armoEth["mod 1 max"]   = "1";

        armoEth["mod 2"]     = "ac%";
        armoEth["mod 2 min"] = "50";
        armoEth["mod 2 max"] = "50";

        StringMap weapEth  = armoEth;
        weapEth["input 1"] = "\"weap,noe\"";
        weapEth["mod 2"]   = "dmg %";

        StringMap armoNor  = base;
        armoNor["input 1"] = "armo";
        armoNor["input 2"] = "vps";
        armoNor["output"]  = "\"usetype,nor\"";

        StringMap weapNor  = armoNor;
        weapNor["input 1"] = "weap";

        view.appendRow(armo1);
        view.appendRow(weap1);
        view.appendRow(armo2);
        view.appendRow(weap2);
        view.appendRow(armoEth);
        view.appendRow(weapEth);
        view.appendRow(armoNor);
        view.appendRow(weapNor);
    }

    if (cheatCharms) {
        struct Affix {
            std::string m_name;
            std::string m_value;
        };
        struct CraftResult {
            std::string        m_name;
            std::string        m_i1;
            std::string        m_i2;
            std::vector<Affix> m_affixes;
        };
        const std::vector<CraftResult> crs{
            //----------------
            CraftResult{
                .m_name    = "Cheat SC: key + stamina = all skills + all stats",
                .m_i1      = "key",
                .m_i2      = "vps",
                .m_affixes = { { "allskills", "7" }, { "all-stats", "20" }, { "ac", "1" }, { "att", "1" }, { "dmg-max", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + antidote = damage reduction",
                .m_i1      = "key",
                .m_i2      = "yps",
                .m_affixes = { { "red-dmg", "15" }, { "red-mag", "15" }, { "red-dmg%", "10" }, { "ac", "1" }, { "att", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + thawing = absorb",
                .m_i1      = "key",
                .m_i2      = "wms",
                .m_affixes = { { "abs-fire%", "10" }, { "abs-ltng%", "10" }, { "abs-mag%", "10" }, { "abs-cold%", "10" }, { "ac", "1" } },
            },

            CraftResult{
                .m_name    = "Cheat SC: key + any hp = HP and vitality",
                .m_i1      = "key",
                .m_i2      = "hpot",
                .m_affixes = { { "hp", "300" }, { "hp%", "10" }, { "vit", "20" }, { "ac", "1" }, { "att", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: key + any mana = Mana and energy",
                .m_i1      = "key",
                .m_i2      = "mpot",
                .m_affixes = { { "mana", "150" }, { "mana%", "20" }, { "enr", "20" }, { "ac", "1" }, { "att", "1" } },
            },

            //----------------
            CraftResult{
                .m_name    = "Cheat SC: TP + stamina = faster frames",
                .m_i1      = "tsc",
                .m_i2      = "vps",
                .m_affixes = { { "move2", "30" }, { "cast2", "80" }, { "balance2", "80" }, { "block2", "50" }, { "ac", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + antidote = max res + poison len",
                .m_i1      = "tsc",
                .m_i2      = "yps",
                .m_affixes = { { "res-fire-max", "8" }, { "res-ltng-max", "8" }, { "res-cold-max", "8" }, { "res-pois-max", "8" }, { "res-pois-len", "50" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + thawing = all res",
                .m_i1      = "tsc",
                .m_i2      = "wms",
                .m_affixes = { { "res-all", "50" }, { "ac", "1" }, { "att", "1" }, { "dmg-min", "1" }, { "dmg-max", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + any hp = hp regen",
                .m_i1      = "tsc",
                .m_i2      = "hpot",
                .m_affixes = { { "lifesteal", "10" }, { "regen", "30" }, { "heal-kill", "20" }, { "regen-stam", "50" }, { "ac", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: TP + any mana = mana regen",
                .m_i1      = "tsc",
                .m_i2      = "mpot",
                .m_affixes = { { "manasteal", "5" }, { "regen-mana", "50" }, { "mana-kill", "5" }, { "dmg-to-mana", "20" }, { "ac", "1" } },
            },

            //----------------
            CraftResult{
                .m_name    = "Cheat SC: ID + stamina = pierce res",
                .m_i1      = "isc",
                .m_i2      = "vps",
                .m_affixes = { { "pierce-pois", "20" }, { "pierce-fire", "20" }, { "pierce-ltng", "20" }, { "pierce-cold", "20" }, { "ac", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + antidote = +elemental dmg",
                .m_i1      = "isc",
                .m_i2      = "yps",
                .m_affixes = { { "extra-ltng", "50" }, { "extra-cold", "50" }, { "extra-pois", "50" }, { "extra-fire", "50" }, { "ac", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + thawing = OW/CB/DS",
                .m_i1      = "isc",
                .m_i2      = "wms",
                .m_affixes = { { "crush", "20" }, { "openwounds", "20" }, { "deadly", "20" }, { "noheal", "1" }, { "ac", "1" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + any hp = IAS and Enh Dmg",
                .m_i1      = "isc",
                .m_i2      = "hpot",
                .m_affixes = { { "swing2", "30" }, { "dmg-max", "20" }, { "dmg%", "100" }, { "dmg-demon", "100" }, { "dmg-undead", "100" } },
            },
            CraftResult{
                .m_name    = "Cheat SC: ID + any mana = attack rating and defence",
                .m_i1      = "isc",
                .m_i2      = "mpot",
                .m_affixes = { { "att%", "20" }, { "att", "200" }, { "ac", "100" }, { "dmg-min", "1" }, { "dmg-max", "1" } },
            },
        };
        for (const CraftResult& c : crs) {
            StringMap data{
                { "description", c.m_name },
                { "enabled", "1" },
                { "lvl", "1" },
                { "version", "100" },
                { "*eol", "0" },
                { "numinputs", "3" },
                { "input 1", "cm1" },
                { "input 2", c.m_i1 },
                { "input 3", c.m_i2 },
                { "output", "\"usetype,crf\"" },
            };
            for (size_t i = 1; auto&& af : c.m_affixes) {
                data["mod " + std::to_string(i)]             = af.m_name;
                data["mod " + std::to_string(i) + " chance"] = "100";
                data["mod " + std::to_string(i) + " min"]    = af.m_value;
                data["mod " + std::to_string(i) + " max"]    = af.m_value;
                i++;
            }
            view.appendRow(data);
        }
    }
}

}
