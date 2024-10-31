#include "Precompiled.h"
#include "WaveComponent.h"

using namespace EngineD;
using namespace EngineD::Graphics;

void WaveLoaderComponent::Initialize()
{
	mTemplateFilePath = L"../../Assets/Templates/waveLoader.json";
	FILE* file = nullptr;
	auto err = fopen_s(&file, mTemplateFilePath.u8string().c_str(), "r");
	ASSERT(err == 0, "Wave Component: failed to open file %s", mTemplateFilePath.u8string().c_str());

	char readBuffer[65536];
	rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rapidjson::Document doc;
	doc.ParseStream(readStream);
	auto component = doc["Components"].GetObj().begin();
	Deserialize(component->value);
}

void WaveLoaderComponent::Terminate()
{
	mEffect = nullptr;
}

void WaveLoaderComponent::DebugUI()
{
	if (ImGui::Button("Save"))
	{
		if (mTemplateFilePath.empty())
		{
			return;
		}

		mData = mEffect->GetOceanData();
		rapidjson::Document doc;
		doc.SetObject();

		rapidjson::Value components(rapidjson::kObjectType);
		Serialize(doc, components);
		doc.AddMember("Components", components, doc.GetAllocator());

		FILE* file = nullptr;
		auto err = fopen_s(&file, mTemplateFilePath.u8string().c_str(), "w");
		ASSERT(err == 0, "Wave Component: failed to open template file %s", mTemplateFilePath.u8string().c_str());

		char writeBuffer[655536];
		rapidjson::FileWriteStream writeStream(file, writeBuffer, sizeof(writeBuffer));
		rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(writeStream);
		writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
		doc.Accept(writer);
		fclose(file);
	}
}

void WaveLoaderComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveInt("WaveCount", mData.waveCount, doc, componentValue);
	SaveUtil::SaveInt("PixelWaveCount", mData.pixelWaveCount, doc, componentValue);
	SaveUtil::SaveFloat("VertexFrequency", mData.vertexFrequency, doc, componentValue);
	SaveUtil::SaveFloat("VertexAmplitude", mData.vertexAmplitude, doc, componentValue);
	SaveUtil::SaveFloat("VertexInitialSpeed", mData.vertexInitialSpeed, doc, componentValue);
	SaveUtil::SaveFloat("VertexSeed", mData.vertexSeed, doc, componentValue);
	SaveUtil::SaveFloat("VertexMaxPeak", mData.vertexMaxPeak, doc, componentValue);
	SaveUtil::SaveFloat("VertexPeakOffset", mData.vertexPeakOffset, doc, componentValue);
	SaveUtil::SaveFloat("VertexFrequencyMult", mData.vertexFrequencyMult, doc, componentValue);
	SaveUtil::SaveFloat("VertexAmplitudeMult", mData.vertexAmplitudeMult, doc, componentValue);
	SaveUtil::SaveFloat("VertexSpeedRamp", mData.vertexSpeedRamp, doc, componentValue);
	SaveUtil::SaveFloat("VertexSeedIter", mData.vertexSeedIter, doc, componentValue);
	SaveUtil::SaveFloat("VertexHeight", mData.vertexHeight, doc, componentValue);
	SaveUtil::SaveFloat("VertexDrag", mData.vertexDrag, doc, componentValue);

	SaveUtil::SaveFloat("NormalStrength", mLightData.normalStrength, doc, componentValue);
	SaveUtil::SaveFloat("SpecularNormalStrength", mLightData.specNormalStrength, doc, componentValue);
	SaveUtil::SaveFloat("Shininess", mLightData.shininess, doc, componentValue);
	SaveUtil::SaveFloat("TipAttenuation", mLightData.tipAttenuation, doc, componentValue);
	SaveUtil::SaveVector3("DiffuseReflectance", mLightData.diffuseReflectance, doc, componentValue);
	SaveUtil::SaveVector3("SpecularReflectance", mLightData.specularReflectance, doc, componentValue);
	SaveUtil::SaveVector3("AmbientColor", mLightData.ambientColor, doc, componentValue);
	SaveUtil::SaveVector3("TipColor", mLightData.tipColor, doc, componentValue);
	SaveUtil::SaveVector4("SpecularColor", mLightData.specularColor, doc, componentValue);
	
	value.AddMember("WaveComponent", componentValue, doc.GetAllocator());
}

void WaveLoaderComponent::Deserialize(const rapidjson::Value& value)
{
	if (value.HasMember("WaveCount"))
	{
		int count = value["WaveCount"].GetInt();
		mData.waveCount = count;
	}
	if (value.HasMember("PixelWaveCount"))
	{
		int count = value["PixelWaveCount"].GetInt();
		mData.pixelWaveCount = count;
	}

	if (value.HasMember("VertexFrequency"))
	{	
		float data = value["VertexFrequency"].GetFloat();
		mData.vertexFrequency = data;
	}
	if (value.HasMember("VertexAmplitude"))
	{	
		float data = value["VertexAmplitude"].GetFloat();
		mData.vertexAmplitude = data;
	}
	if (value.HasMember("VertexInitialSpeed"))
	{	
		float data = value["VertexInitialSpeed"].GetFloat();
		mData.vertexInitialSpeed = data;
	}
	if (value.HasMember("VertexSeed"))
	{	
		float data = value["VertexSeed"].GetFloat();
		mData.vertexSeed = data;
	}
	if (value.HasMember("VertexMaxPeak"))
	{	
		float data = value["VertexMaxPeak"].GetFloat();
		mData.vertexMaxPeak = data;
	}
	if (value.HasMember("VertexPeakOffset"))
	{	
		float data = value["VertexPeakOffset"].GetFloat();
		mData.vertexPeakOffset = data;
	}
	if (value.HasMember("VertexFrequencyMult"))
	{	
		float data = value["VertexFrequencyMult"].GetFloat();
		mData.vertexFrequencyMult = data;
	}
	if (value.HasMember("VertexAmplitudeMult"))
	{	
		float data = value["VertexAmplitudeMult"].GetFloat();
		mData.vertexAmplitudeMult = data;
	}
	if (value.HasMember("VertexSpeedRamp"))
	{	
		float data = value["VertexSpeedRamp"].GetFloat();
		mData.vertexSpeedRamp = data;
	}
	if (value.HasMember("VertexSeedIter"))
	{	
		float data = value["VertexSeedIter"].GetFloat();
		mData.vertexSeedIter = data;
	}
	if (value.HasMember("VertexHeight"))
	{	
		float data = value["VertexHeight"].GetFloat();
		mData.vertexHeight = data;
	}
	if (value.HasMember("VertexDrag"))
	{	
		float data = value["VertexDrag"].GetFloat();
		mData.vertexDrag = data;
	}

	if (value.HasMember("VertexDrag"))
	{
		float data = value["VertexDrag"].GetFloat();
		mData.vertexDrag = data;
	}


	if (value.HasMember("NormalStrength"))
	{
		float data = value["NormalStrength"].GetFloat();
		mLightData.normalStrength = data;
	}
	if (value.HasMember("SpecularNormalStrength"))
	{
		float data = value["SpecularNormalStrength"].GetFloat();
		mLightData.specNormalStrength = data;
	}
	if (value.HasMember("Shininess"))
	{
		float data = value["Shininess"].GetFloat();
		mLightData.shininess = data;
	}
	if (value.HasMember("TipAttenuation"))
	{
		float data = value["TipAttenuation"].GetFloat();
		mLightData.tipAttenuation = data;
	}

	if (value.HasMember("DiffuseReflectance"))
	{
		const auto& data = value["DiffuseReflectance"].GetArray();
		float x = data[0].GetFloat();
		float y = data[1].GetFloat();
		float z = data[2].GetFloat();
		mLightData.diffuseReflectance = { x, y, z };
	}
	if (value.HasMember("SpecularReflectance"))
	{
		const auto& data = value["SpecularReflectance"].GetArray();
		float x = data[0].GetFloat();
		float y = data[1].GetFloat();
		float z = data[2].GetFloat();
		mLightData.specularReflectance = { x, y, z };
	}
	if (value.HasMember("AmbientColor"))
	{
		const auto& data = value["AmbientColor"].GetArray();
		float x = data[0].GetFloat();
		float y = data[1].GetFloat();
		float z = data[2].GetFloat();
		mLightData.ambientColor = { x, y, z };
	}
	if (value.HasMember("TipColor"))
	{
		const auto& data = value["TipColor"].GetArray();
		float x = data[0].GetFloat();
		float y = data[1].GetFloat();
		float z = data[2].GetFloat();
		mLightData.tipColor = { x, y, z };
	}

	if (value.HasMember("SpecularColor"))
	{
		const auto& data = value["SpecularColor"].GetArray();
		float x = data[0].GetFloat();
		float y = data[1].GetFloat();
		float z = data[2].GetFloat();
		float w = data[3].GetFloat();
		mLightData.specularColor = { x, y, z, w };
	}
}

WaveEffect::OceanData WaveLoaderComponent::GetData()
{
	return mData;
}

Graphics::WaveEffect::LightData EngineD::WaveLoaderComponent::GetLightData()
{
	return mLightData;
}

void WaveLoaderComponent::SetEffect(EngineD::Graphics::WaveEffect* waveEffect)
{
	mEffect = waveEffect;
}
