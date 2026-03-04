add_library(economy_core_lib
        ./Bank.hpp
        ./Bank.cpp
        ./BaseActor.cpp
        ./BaseActor.hpp
        ./BaseBankAccountHolder.hpp
        ./BaseBankAccountHolder.cpp
        ./BaseWalletHolder.hpp
        ./BaseWalletHolder.cpp
        ./Person.cpp
        ./Person.hpp
        ./EconomyContext.cpp
        ./EconomyContext.hpp
        ./IActor.hpp
        ./money_storage/BankAccount.hpp
        ./money_storage/BaseMoneyStorage.cpp
        ./money_storage/BaseMoneyStorage.hpp
        ./money_storage/IMoneyStorage.hpp
        ./money_storage/MoneyTransfer.cpp
        ./money_storage/MoneyTransfer.hpp
        ./money_storage/Wallet.hpp
)

target_include_directories(economy_core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
