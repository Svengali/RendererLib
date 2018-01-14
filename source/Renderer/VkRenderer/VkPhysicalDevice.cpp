/*
This file belongs to VkLib.
See LICENSE file in root folder.
*/
#include "VkPhysicalDevice.hpp"

#include "VkRenderer.hpp"
#include "VkDevice.hpp"

namespace vk_renderer
{
	PhysicalDevice::PhysicalDevice( Renderer & renderer
		, VkPhysicalDevice gpu )
		: m_gpu{ gpu }
		, m_renderer{ renderer }
	{
		// On récupère les extensions supportées par le GPU.
		uint32_t extensionCount{ 0 };
		auto res = EnumerateDeviceExtensionProperties( m_gpu
			, nullptr
			, &extensionCount
			, nullptr );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "GPU's extensions enumeration failed: " + getLastError() };
		}

		std::vector< VkExtensionProperties > extensions( extensionCount );
		res = EnumerateDeviceExtensionProperties( m_gpu
			, nullptr
			, &extensionCount
			, extensions.data() );

		if ( !checkError( res ) )
		{
			throw std::runtime_error{ "GPU's extensions enumeration failed: " + getLastError() };
		}

		m_renderer.completeLayerNames( m_deviceLayerNames );
		m_deviceExtensionNames.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
		checkExtensionsAvailability( extensions, m_deviceExtensionNames );

		// Puis les capacités du GPU.
		GetPhysicalDeviceMemoryProperties( m_gpu, &m_memoryProperties );
		GetPhysicalDeviceProperties( m_gpu, &m_properties );
		GetPhysicalDeviceFeatures( m_gpu, &m_features );

		// Et enfin les propriétés des familles de files du GPU.
		uint32_t queueCount{ 0 };
		GetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, nullptr );
		assert( queueCount >= 1 );

		m_queueProperties.resize( queueCount );
		GetPhysicalDeviceQueueFamilyProperties( m_gpu, &queueCount, m_queueProperties.data() );
		assert( queueCount >= 1 );
	}

	bool PhysicalDevice::deduceMemoryType( uint32_t typeBits
		, VkMemoryPropertyFlags requirements
		, uint32_t & typeIndex )const
	{
		bool result{ false };

		// Recherche parmi les types de mémoire de la première ayant les propriétés voulues.
		uint32_t i{ 0 };
		while ( i < m_memoryProperties.memoryTypeCount && !result )
		{
			if ( ( typeBits & 1 ) == 1 )
			{
				// Le type de mémoire est disponible, a-t-il les propriétés demandées?
				if ( ( m_memoryProperties.memoryTypes[i].propertyFlags & requirements ) == requirements )
				{
					typeIndex = i;
					result = true;
				}
			}

			typeBits >>= 1;
			++i;
		}

		return result;
	}
}