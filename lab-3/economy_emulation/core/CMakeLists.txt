add_library(economy_core_lib
        ./Bank.hpp
        ./Bank.cpp
        ./BankAccount.hpp
        ./BaseActor.cpp
        ./BaseActor.hpp
        ./BaseMoneyStorage.cpp
        ./BaseMoneyStorage.hpp
        ./EconomyContext.cpp
        ./EconomyContext.hpp
        ./IActor.hpp
        ./IMoneyStorage.hpp
        ./MoneyTransfer.cpp
        ./MoneyTransfer.hpp
        ./Wallet.hpp
)

target_include_directories(economy_core_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
