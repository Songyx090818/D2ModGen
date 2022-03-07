/*
 * Copyright (C) 2022 Smirnov Vladimir / mapron1@gmail.com
 * SPDX-License-Identifier: MIT
 * See LICENSE file for details.
 */
#pragma once

#include "CommonTypes.hpp"
#include "PropertyTree.hpp"

#include <memory>

namespace D2ModGen {

class DataContext;
class IModule {
public:
    using Ptr        = std::shared_ptr<const IModule>;
    using PtrMap     = std::map<std::string, Ptr>;
    using PresetList = std::vector<PropertyTreeScalarMap>;

    struct ExtraDependencies {
        struct Source {
            StorageType    type   = StorageType::CsvFolder;
            ConflictPolicy policy = ConflictPolicy::RaiseError;
            std::string    srcRoot;
            std::string    modname;
        };
        std::vector<Source> m_sources;
    };

    struct PreGenerationContext {
        StringSet         m_extraJson;
        ExtraDependencies m_preGen;
        ExtraDependencies m_postGen;
    };

    struct InputContext {
        PropertyTree          m_settings;
        PropertyTree          m_mergedSettings;
        GenerationEnvironment m_env;

        bool isAllDefault() const noexcept
        {
            if (!m_settings.isMap())
                return true;
            auto& m = m_settings.getMap();
            return m.empty();
        }
        bool isAllDefault(const StringVector& keys) const noexcept
        {
            if (!m_settings.isMap())
                return true;

            for (const std::string& key : keys) {
                if (m_settings.contains(key))
                    return false;
            }
            return true;
        }
        bool isDefault(const std::string& key) const noexcept
        {
            return !m_settings.contains(key);
        }

        const PropertyTreeScalar& getScalar(const std::string& key) const noexcept(false)
        {
            return m_mergedSettings.getMap().at(key).getScalar();
        }

        int getInt(const std::string& key) const noexcept(false)
        {
            return getScalar(key).toInt();
        }
        std::string getString(const std::string& key) const noexcept(false)
        {
            return getScalar(key).toString();
        }
    };

    struct UiControlHint {
        enum Control
        {
            Auto,
            SliderMinMax,
            Slider,
            CheckBox,
            LineEdit
        };
        Control m_control = Control::Auto;
        int     m_min     = 0;
        int     m_max     = 0;
        double  m_num     = 0.;
        double  m_denom   = 0.;
        bool    m_compact = true;
    };
    struct UiControlHintSlider : public UiControlHint {
        UiControlHintSlider() = default;
        UiControlHintSlider(double num, double denom, bool compact = false)
        {
            m_control = Control::Slider;
            m_num     = num;
            m_denom   = denom;
            m_compact = compact;
        }
    };
    struct UiControlHintSliderMinMax : public UiControlHint {
        UiControlHintSliderMinMax() = default;
        UiControlHintSliderMinMax(int min, int max, bool compact = false)
        {
            m_control = Control::SliderMinMax;
            m_min     = min;
            m_max     = max;
            m_compact = compact;
        }
    };
    using UiControlHintMap = std::map<std::string, UiControlHint>;

public:
    virtual std::string settingKey() const = 0;

    virtual PropertyTree     pluginInfo() const    = 0;
    virtual PresetList       presets() const       = 0;
    virtual PropertyTree     defaultValues() const = 0;
    virtual UiControlHintMap uiHints() const       = 0;

    virtual void gatherInfo(PreGenerationContext& output, const InputContext& input) const            = 0;
    virtual void generate(DataContext& output, RandomGenerator& rng, const InputContext& input) const = 0;

public:
    struct Key {
        static constexpr const std::string_view challenge{ "challenge" };
        static constexpr const std::string_view character{ "character" };
        static constexpr const std::string_view horadricCube{ "cube" };
        static constexpr const std::string_view dropFiltering{ "drop_filter" };
        static constexpr const std::string_view gambling{ "gambling" };
        static constexpr const std::string_view itemDrops{ "item_drops" };
        static constexpr const std::string_view itemRandomizer{ "randomizer" };
        static constexpr const std::string_view mergePregen{ "mergePregen" };
        static constexpr const std::string_view mergePostgen{ "mergePostgen" };
        static constexpr const std::string_view monsterDensity{ "density" };
        static constexpr const std::string_view monsterRandomizer{ "monRandomizer" };
        static constexpr const std::string_view monsterStats{ "monster_stats" };
        static constexpr const std::string_view perfectRolls{ "perfect_rolls" };
        static constexpr const std::string_view qualityOfLife{ "qol" };
        static constexpr const std::string_view runeDrops{ "rune_drops" };
        static constexpr const std::string_view skillRandomizer{ "skillRandomizer" };

        static constexpr const std::string_view testConfig{ "testConfig" };
        static constexpr const std::string_view main{ "main" };
    };
};

}
