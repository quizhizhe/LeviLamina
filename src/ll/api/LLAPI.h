﻿#pragma once

#include "ll/api/base/Global.h"
#include "ll/api/plugin/Plugin.h"

// Loader APIs
namespace ll {

/**
 * @brief Get the loader version as a Version object
 *
 * @return ll::Version  The loader version
 */
LLNDAPI plugin::Version getLoaderVersion();

/**
 * @brief Register a plugin
 *
 * @param  name     The name of the plugin
 * @param  desc     The descirption(introduction) of the plugin
 * @param  version  The version of the plugin(ll::Version)
 * @param  others   The other information of the plugin(key-value)
 * @return bool     True if the plugin is registered successfully
 *
 * @par Example
 * @code
 * ll::registerPlugin("Test", "A test plugin", Version(0, 0, 1, Version::Alpha), {{"Note","This is Note"}});
 * @endcode
 */
LLAPI bool registerPlugin(
    std::string const&                        name,
    std::string const&                        description,
    plugin::Version const&                    version,
    std::map<std::string, std::string> const& extraInfo = {}
);

/**
 * @brief Find a loaded plugin by name
 *
 * @param  name         The name of the plugin
 */
LLNDAPI optional_ref<plugin::Plugin> findPlugin(std::string const& name);

/**
 * @brief Get the All the loaded plugins
 *
 * @return std::unordered_map<std::string, ll::Plugin>  The loaded plugins(name-plugin)
 */
LLNDAPI std::unordered_map<std::string, plugin::Plugin>& getAllPlugins();

} // namespace ll
