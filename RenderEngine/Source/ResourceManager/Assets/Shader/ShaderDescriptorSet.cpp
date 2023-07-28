#include "ResourceManager/Assets/Shader/ShaderDescriptorSet.hpp"

using namespace RenderEngine::Assets;

ShaderDescriptorSet ShaderDescriptorSet::GenerateDefaultVertexShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor uniformBufferDescriptor{
		ShaderStage::VERTEX,
		DescriptorType::UNIFORM_BUFFER,
		0u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(uniformBufferDescriptor);

	ShaderDescriptor cameraBufferDescriptor{
		ShaderStage::VERTEX,
		DescriptorType::CAMERA_BUFFER,
		0u,
		1u
	};
	descriptorSet.shaderDescriptors.push_front(cameraBufferDescriptor);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateSkyboxVertexShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor cameraBufferDescriptor{
		ShaderStage::VERTEX,
		DescriptorType::CAMERA_BUFFER,
		0u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(cameraBufferDescriptor);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateFilterCubeVertexShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	PushConstant constant{
		ShaderStage::VERTEX,
		128u,
		0u
	};
	descriptorSet.pushConstants.push_front(constant);

	return descriptorSet;
}


ShaderDescriptorSet ShaderDescriptorSet::GenerateDefaultFragmentShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor albedo{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_ALBEDO,
		1u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(albedo);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateSkyboxFragmentShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor skybox{
		ShaderStage::FRAGMENT,
		DescriptorType::SKYBOX_CUBEMAP,
		1u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(skybox);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateTextureToCubemapFragmentShaderDescriptor(Texture* _inputTexture)
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor textureDescriptor{
		ShaderStage::FRAGMENT,
		DescriptorType::CUSTOM_TEXTURE,
		0u,
		0u
	};
	textureDescriptor.texture = _inputTexture;
	descriptorSet.shaderDescriptors.push_front(textureDescriptor);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GeneratePhongFragmentShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor albedo{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_ALBEDO,
		1u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(albedo);

	ShaderDescriptor metalness{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_METALNESS,
		1u,
		1u
	};
	descriptorSet.shaderDescriptors.push_front(metalness);

	ShaderDescriptor normal{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_NORMALMAP,
		1u,
		2u
	};
	descriptorSet.shaderDescriptors.push_front(normal);

	ShaderDescriptor materialBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_BUFFER,
		1u,
		3u
	};
	descriptorSet.shaderDescriptors.push_front(materialBuffer);

	ShaderDescriptor pointLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::POINT_LIGHT_BUFFER,
		1u,
		4u
	};
	descriptorSet.shaderDescriptors.push_front(pointLightBuffer);

	ShaderDescriptor directionalLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::DIRECTIONAL_LIGHT_BUFFER,
		1u,
		5u
	};
	descriptorSet.shaderDescriptors.push_front(directionalLightBuffer);

	ShaderDescriptor spotLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::SPOT_LIGHT_BUFFER,
		1u,
		6u
	};
	descriptorSet.shaderDescriptors.push_front(spotLightBuffer);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateBlinnPhongFragmentShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor albedo{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_ALBEDO,
		1u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(albedo);

	ShaderDescriptor metalness{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_METALNESS,
		1u,
		1u
	};
	descriptorSet.shaderDescriptors.push_front(metalness);

	ShaderDescriptor normal{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_NORMALMAP,
		1u,
		2u
	};
	descriptorSet.shaderDescriptors.push_front(normal);

	ShaderDescriptor materialBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_BUFFER,
		1u,
		3u
	};
	descriptorSet.shaderDescriptors.push_front(materialBuffer);

	ShaderDescriptor pointLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::POINT_LIGHT_BUFFER,
		1u,
		4u
	};
	descriptorSet.shaderDescriptors.push_front(pointLightBuffer);

	ShaderDescriptor directionalLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::DIRECTIONAL_LIGHT_BUFFER,
		1u,
		5u
	};
	descriptorSet.shaderDescriptors.push_front(directionalLightBuffer);

	ShaderDescriptor spotLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::SPOT_LIGHT_BUFFER,
		1u,
		6u
	};
	descriptorSet.shaderDescriptors.push_front(spotLightBuffer);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GenerateIrradianceConvolutionFragmentShaderDescriptor(Texture* _inputTexture)
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor textureDescriptor{
		ShaderStage::FRAGMENT,
		DescriptorType::CUSTOM_TEXTURE,
		0u,
		0u
	};
	textureDescriptor.texture = _inputTexture;
	descriptorSet.shaderDescriptors.push_front(textureDescriptor);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GeneratePrefilterEnvmapFragmentShaderDescriptor(Texture* _inputTexture)
{
	ShaderDescriptorSet descriptorSet{};

	PushConstant constant{
		ShaderStage::FRAGMENT,
		8u,
		128u
	};
	descriptorSet.pushConstants.push_front(constant);

	ShaderDescriptor textureDescriptor{
		ShaderStage::FRAGMENT,
		DescriptorType::CUSTOM_TEXTURE,
		0u,
		0u
	};
	textureDescriptor.texture = _inputTexture;
	descriptorSet.shaderDescriptors.push_front(textureDescriptor);

	return descriptorSet;
}

ShaderDescriptorSet ShaderDescriptorSet::GeneratePBRFragmentShaderDescriptor()
{
	ShaderDescriptorSet descriptorSet{};

	ShaderDescriptor albedo{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_ALBEDO,
		1u,
		0u
	};
	descriptorSet.shaderDescriptors.push_front(albedo);

	ShaderDescriptor metalness{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_METALNESS,
		1u,
		1u
	};
	descriptorSet.shaderDescriptors.push_front(metalness);

	ShaderDescriptor roughness{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_ROUGHNESS,
		1u,
		2u
	};
	descriptorSet.shaderDescriptors.push_front(roughness);

	ShaderDescriptor normal{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_NORMALMAP,
		1u,
		3u
	};
	descriptorSet.shaderDescriptors.push_front(normal);

	ShaderDescriptor aoMap{
		ShaderStage::FRAGMENT,
		DescriptorType::MESHRENDERER_MATERIAL_AOMAP,
		1u,
		4u
	};
	descriptorSet.shaderDescriptors.push_front(aoMap);

	ShaderDescriptor pointLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::POINT_LIGHT_BUFFER,
		1u,
		5u
	};
	descriptorSet.shaderDescriptors.push_front(pointLightBuffer);

	ShaderDescriptor directionalLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::DIRECTIONAL_LIGHT_BUFFER,
		1u,
		6u
	};
	descriptorSet.shaderDescriptors.push_front(directionalLightBuffer);

	ShaderDescriptor spotLightBuffer{
		ShaderStage::FRAGMENT,
		DescriptorType::SPOT_LIGHT_BUFFER,
		1u,
		7u
	};
	descriptorSet.shaderDescriptors.push_front(spotLightBuffer);

	ShaderDescriptor irradianceMap{
		ShaderStage::FRAGMENT,
		DescriptorType::SKYBOX_IRRADIANCE,
		1u,
		8u
	};
	descriptorSet.shaderDescriptors.push_front(irradianceMap);

	ShaderDescriptor prefilteredMap{
		ShaderStage::FRAGMENT,
		DescriptorType::SKYBOX_PREFILTERED,
		1u,
		9u
	};
	descriptorSet.shaderDescriptors.push_front(prefilteredMap);

	ShaderDescriptor BRDFLut{
		ShaderStage::FRAGMENT,
		DescriptorType::SKYBOX_BRDF_LUT,
		1u,
		10u
	};
	descriptorSet.shaderDescriptors.push_front(BRDFLut);

	return descriptorSet;
}