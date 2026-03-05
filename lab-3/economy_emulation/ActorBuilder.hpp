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
	explicit ActorBuilder(Bank& bank, BankAccount& account)
		: m_bank(bank)
		, m_account(account)
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

	std::unique_ptr<Apu> BuildApu()
	{
		auto actor = std::make_unique<Apu>(m_apuId, m_bank, m_bernsId);
		actor.get()->ReceiveBankTransfer(m_account, 200);
		return actor;
	}

	std::unique_ptr<Bart> BuildBart()
	{
		auto actor = std::make_unique<Bart>(m_bartId, m_bank, m_apuId);
		return actor;
	}

	std::unique_ptr<Berns> BuildBerns()
	{
		auto actor = std::make_unique<Berns>(m_bernsId, m_bank, m_homerId, m_weylonId);
		actor.get()->ReceiveBankTransfer(m_account, 2000);
		return actor;
	}

	std::unique_ptr<Chester> BuildChester()
	{
		auto actor = std::make_unique<Chester>(m_chesterId, m_bank, m_homerId, m_apuId);
		return actor;
	}

	std::unique_ptr<Homer> BuildHomer()
	{
		auto actor = std::make_unique<Homer>(m_homerId, m_bank, m_margeId, m_bernsId, m_bartId, m_lizaId, m_chesterId);
		return actor;
	}

	std::unique_ptr<Liza> BuildLiza()
	{
		auto actor = std::make_unique<Liza>(m_lizaId, m_bank, m_apuId);
		return actor;
	}

	std::unique_ptr<Marge> BuildMarge()
	{
		auto actor = std::make_unique<Marge>(m_margeId, m_bank, m_apuId);
		return actor;
	}

	std::unique_ptr<Nelson> BuildNelson()
	{
		auto actor = std::make_unique<Nelson>(m_nelsonId, m_bank, m_bartId, m_apuId);
		return actor;
	}

	std::unique_ptr<Weylon> BuildWeylon()
	{
		auto actor = std::make_unique<Weylon>(m_weylonId, m_bank, m_apuId);
		actor.get()->ReceiveBankTransfer(m_account, 150);
		return actor;
	}

private:
	Bank& m_bank;
	BankAccount& m_account;

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