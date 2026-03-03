#pragma once

#include "actors/Apu.hpp"
#include "actors/Bart.hpp"
#include "actors/Berns.hpp"
#include "actors/Chester.hpp"
#include "actors/Homer.hpp"
#include "actors/Liza.hpp"
#include "actors/Marge.hpp"
#include "actors/Nelson.hpp"
#include "actors/Weylon.hpp"

#include <memory>

class ActorBuilder
{
public:
	explicit ActorBuilder(Bank& bank);

	std::unique_ptr<Apu> BuildApu();
	std::unique_ptr<Bart> BuildBart();
	std::unique_ptr<Berns> BuildBerns();
	std::unique_ptr<Chester> BuildChester();
	std::unique_ptr<Homer> BuildHomer();
	std::unique_ptr<Liza> BuildLiza();
	std::unique_ptr<Marge> BuildMarge();
	std::unique_ptr<Nelson> BuildNelson();
	std::unique_ptr<Weylon> BuildWeylon();

private:
	Bank& m_bank;

	ActorId m_apuId;
	ActorId m_bartId;
	ActorId m_bernsId;
	ActorId m_chesterId;
	ActorId m_homerId;
	ActorId m_lizaId;
	ActorId m_margeId;
	ActorId m_nelsonId;
	ActorId m_weylonId;
};