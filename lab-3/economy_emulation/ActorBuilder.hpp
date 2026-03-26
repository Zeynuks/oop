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
	{
	}

	std::unique_ptr<Apu> BuildApu() const
	{
		auto actor = std::make_unique<Apu>(IDs::Apu, m_bank, IDs::Berns);
		actor.get()->ReceiveBankTransfer(m_account, 200);
		return actor;
	}

	std::unique_ptr<Bart> BuildBart() const
	{
		auto actor = std::make_unique<Bart>(IDs::Bart, m_bank, IDs::Apu);
		return actor;
	}

	std::unique_ptr<Berns> BuildBerns() const
	{
		auto actor = std::make_unique<Berns>(IDs::Berns, m_bank, IDs::Homer, IDs::Weylon);
		actor.get()->ReceiveBankTransfer(m_account, 2000);
		return actor;
	}

	std::unique_ptr<Chester> BuildChester() const
	{
		auto actor = std::make_unique<Chester>(IDs::Chester, m_bank, IDs::Homer, IDs::Apu);
		return actor;
	}

	std::unique_ptr<Homer> BuildHomer() const
	{
		auto actor = std::make_unique<Homer>(IDs::Homer, m_bank, IDs::Marge, IDs::Berns, IDs::Bart, IDs::Liza, IDs::Chester);
		return actor;
	}

	std::unique_ptr<Liza> BuildLiza() const
	{
		auto actor = std::make_unique<Liza>(IDs::Liza, m_bank, IDs::Apu);
		return actor;
	}

	std::unique_ptr<Marge> BuildMarge() const
	{
		auto actor = std::make_unique<Marge>(IDs::Marge, m_bank, IDs::Apu);
		return actor;
	}

	std::unique_ptr<Nelson> BuildNelson() const
	{
		auto actor = std::make_unique<Nelson>(IDs::Nelson, m_bank, IDs::Bart, IDs::Apu);
		return actor;
	}

	std::unique_ptr<Weylon> BuildWeylon() const
	{
		auto actor = std::make_unique<Weylon>(IDs::Weylon, m_bank, IDs::Apu);
		actor.get()->ReceiveBankTransfer(m_account, 150);
		return actor;
	}

private:
	Bank& m_bank;
	BankAccount& m_account;

	struct IDs
	{
		static constexpr ActorId Apu = 1;
		static constexpr ActorId Bart = 2;
		static constexpr ActorId Berns = 3;
		static constexpr ActorId Chester = 4;
		static constexpr ActorId Homer = 5;
		static constexpr ActorId Liza = 6;
		static constexpr ActorId Marge = 7;
		static constexpr ActorId Nelson = 8;
		static constexpr ActorId Weylon = 9;
	};
};