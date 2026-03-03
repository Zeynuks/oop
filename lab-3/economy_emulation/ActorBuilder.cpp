#include "ActorBuilder.hpp"

ActorBuilder::ActorBuilder(Bank& bank)
	: m_bank(bank)
	, m_apuId(1)
	, m_bartId(2)
	, m_bernsId(3)
	, m_chesterId(4)
	, m_homerId(5)
	, m_lizaId(6)
	, m_margeId(7)
	, m_nelsonId(8)
	, m_weylonId(9)
{
}

std::unique_ptr<Apu> ActorBuilder::BuildApu()
{
	return std::make_unique<Apu>(m_apuId, m_bank, m_bernsId);
}

std::unique_ptr<Bart> ActorBuilder::BuildBart()
{
	return std::make_unique<Bart>(m_bartId, m_bank, m_apuId);
}

std::unique_ptr<Berns> ActorBuilder::BuildBerns()
{
	return std::make_unique<Berns>(m_bernsId, m_bank, m_homerId);
}

std::unique_ptr<Chester> ActorBuilder::BuildChester()
{
	return std::make_unique<Chester>(m_chesterId, m_bank, m_homerId, m_apuId);
}

std::unique_ptr<Homer> ActorBuilder::BuildHomer()
{
	return std::make_unique<Homer>(m_homerId, m_bank, m_margeId, m_bernsId, m_bartId, m_lizaId, m_chesterId);
}

std::unique_ptr<Liza> ActorBuilder::BuildLiza()
{
	return std::make_unique<Liza>(m_lizaId, m_bank, m_apuId);
}

std::unique_ptr<Marge> ActorBuilder::BuildMarge()
{
	return std::make_unique<Marge>(m_margeId, m_bank, m_apuId);
}

std::unique_ptr<Nelson> ActorBuilder::BuildNelson()
{
	return std::make_unique<Nelson>(m_nelsonId, m_bank, m_bartId, m_apuId);
}

std::unique_ptr<Weylon> ActorBuilder::BuildWeylon()
{
	return std::make_unique<Weylon>(m_weylonId, m_bank, m_apuId);
}