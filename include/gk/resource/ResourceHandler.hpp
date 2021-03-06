/*
 * =====================================================================================
 *
 *  GameKit
 *  Copyright (C) 2018-2020 Unarelith, Quentin Bazin <openminer@unarelith.net>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * =====================================================================================
 */
#ifndef GK_RESOURCEHANDLER_HPP_
#define GK_RESOURCEHANDLER_HPP_

#include <map>
#include <memory>

#include "gk/core/XMLFile.hpp"
#include "gk/resource/IResourceLoader.hpp"
#include "gk/core/Exception.hpp"

namespace gk {

class ResourceHandler {
	public:
		template<typename T, typename... Args>
		T &add(const std::string &name, Args &&...args) {
			if(has(name)) {
				throw EXCEPTION("A resource of type", typeid(T).name(), "already exists with name:", name);
			}

			m_resources.emplace(name, std::make_shared<T>(std::forward<Args>(args)...));

			return get<T>(name);
		}

		bool has(const std::string &name) {
			return m_resources.count(name) == 1;
		}

		template<typename T>
		T &get(const std::string &name) {
			auto it = m_resources.find(name);
			if(it == m_resources.end()) {
			 	throw EXCEPTION("Unable to find resource with name:", name);
			}

			return *std::static_pointer_cast<T>(it->second);
		}

		template<typename ResourceLoader>
		static auto loadConfigFile(const char *xmlFilename) -> typename std::enable_if<std::is_base_of<IResourceLoader, ResourceLoader>::value>::type {
			ResourceLoader loader;
			loader.load(xmlFilename, getInstance());
		}

		static ResourceHandler &getInstance();

		static void setInstance(ResourceHandler &handler);

	private:
		static ResourceHandler *instance;

		std::map<std::string, std::shared_ptr<void>> m_resources;
};

} // namespace gk

#endif // GK_RESOURCEHANDLER_HPP_
