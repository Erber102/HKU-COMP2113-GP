#include "NightPhase.h"
#include "Customer.h"
#include "Market.h"
#include "Config.h"
#include "item.h"
#include "itemdatabase.h"
#include "../Player.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// Constructor
NightPhase::NightPhase(Player* player, Market* market) 
    : m_player(player), m_market(market) {
}

// Default constructor
NightPhase::NightPhase() 
    : m_player(nullptr), m_market(nullptr) {
}

// Static executeNight method - Note: This signature needs to match Game.cpp usage
// Game.cpp currently passes nullptr for market, so we need to handle that
void NightPhase::executeNight(Player& player, Market* market, int day) {
    if (!market) {
        // If market is nullptr, create a temporary one
        // In actual game, market should be properly initialized
        static Market tempMarket;
        market = &tempMarket;
    }
    
    NightPhase nightPhase(&player, market);
    nightPhase.executeNightPhase(day);
}

// Execute night phase
void NightPhase::executeNightPhase(int day) {
    if (!m_player || !m_market) {
        std::cout << "错误：玩家或市场未初始化 (Error: Player or Market not initialized)" << std::endl;
        return;
    }
    
    std::cout << "\n=== 夜晚阶段 (Night Phase) - 第 " << day << " 天 (Day " << day << ") ===" << std::endl;
    std::cout << "欢迎回到安全屋！(Welcome back to your safe house!)" << std::endl;
    
    // Generate customers
    MarketTrends& trends = m_market->getTrends();
    std::vector<Customer> customers = generateCustomers(trends);
    
    bool nightPhaseActive = true;
    
    while (nightPhaseActive) {
        displayMenu();
        
        int choice;
        std::cout << "\n请选择 (Please choose): ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                // View customers
                displayCustomers(customers);
                break;
                
            case 2:
                // Trade with customer
                if (customers.empty()) {
                    std::cout << "没有顾客 (No customers)" << std::endl;
                } else {
                    std::cout << "\n选择要交易的顾客 (Select customer to trade with):" << std::endl;
                    for (size_t i = 0; i < customers.size(); i++) {
                        std::cout << "[" << (i + 1) << "] " << customers[i].name << std::endl;
                    }
                    std::cout << "[0] 返回 (Return)" << std::endl;
                    
                    int customerChoice;
                    std::cout << "选择 (Choice): ";
                    std::cin >> customerChoice;
                    
                    if (customerChoice > 0 && customerChoice <= static_cast<int>(customers.size())) {
                        handleCustomerTrade(customers[customerChoice - 1]);
                    }
                }
                break;
                
            case 3:
                // Black market
                handleBlackMarket();
                break;
                
            case 4:
                // View inventory
                m_player->displayInventory();
                break;
                
            case 5:
                // End night phase
                if (processEndOfNight(day)) {
                    nightPhaseActive = false;
                } else {
                    std::cout << "游戏结束！钱不够支付费用 (Game Over! Insufficient funds to pay fee)" << std::endl;
                    return;
                }
                break;
                
            default:
                std::cout << "无效选择 (Invalid choice)" << std::endl;
                break;
        }
    }
}

// Display customers
void NightPhase::displayCustomers(const std::vector<Customer>& customers) const {
    std::cout << "\n=== 顾客列表 (Customer List) ===" << std::endl;
    
    if (customers.empty()) {
        std::cout << "没有顾客 (No customers)" << std::endl;
        return;
    }
    
    for (size_t i = 0; i < customers.size(); i++) {
        const Customer& c = customers[i];
        std::cout << "\n[" << (i + 1) << "] " << c.name << std::endl;
        std::cout << "   想要购买 (Wants to buy): " << c.desired_item 
                  << " x" << c.desired_quantity << std::endl;
        std::cout << "   出价 (Offers): " << calculateTradePrice(c, c.desired_quantity) 
                  << " 货币 (Money)" << std::endl;
        std::cout << "   耐心值 (Patience): " << c.patience << std::endl;
    }
}

// Handle customer trade
bool NightPhase::handleCustomerTrade(const Customer& customer) {
    std::cout << "\n=== 交易 (Trade) ===" << std::endl;
    std::cout << "顾客 (Customer): " << customer.name << std::endl;
    std::cout << "想要购买 (Wants to buy): " << customer.desired_item 
              << " x" << customer.desired_quantity << std::endl;
    
    // Check if player has the item
    if (!playerHasItem(customer.desired_item, customer.desired_quantity)) {
        std::cout << "你没有足够的物品！(You don't have enough items!)" << std::endl;
        return false;
    }
    
    int totalPrice = calculateTradePrice(customer, customer.desired_quantity);
    std::cout << "总价 (Total price): " << totalPrice << " 货币 (Money)" << std::endl;
    
    std::cout << "\n是否同意交易？(Agree to trade?) [1] 是 (Yes) [2] 否 (No): ";
    int choice;
    std::cin >> choice;
    
    if (choice != 1) {
        std::cout << "交易取消 (Trade cancelled)" << std::endl;
        return false;
    }
    
    // Remove items from player inventory
    m_player->removeItem(customer.desired_item, customer.desired_quantity);
    
    // Add money to player
    m_player->addMoney(totalPrice);
    
    std::cout << "交易成功！(Trade successful!)" << std::endl;
    std::cout << "获得 (Earned): " << totalPrice << " 货币 (Money)" << std::endl;
    
    return true;
}

// Calculate trade price
int NightPhase::calculateTradePrice(const Customer& customer, int quantity) const {
    // Base price calculation
    int basePrice = customer.base_price * quantity;
    
    // Adjust based on patience (higher patience = better price)
    float patienceMultiplier = 1.0f + (customer.patience - 1) * 0.1f;
    
    return static_cast<int>(basePrice * patienceMultiplier);
}

// Check if player has item
bool NightPhase::playerHasItem(const std::string& itemName, int quantity) const {
    if (!m_player) return false;
    return m_player->hasItem(itemName, quantity);
}

// Display menu
void NightPhase::displayMenu() const {
    std::cout << "\n=== 夜晚菜单 (Night Menu) ===" << std::endl;
    std::cout << "[1] 查看顾客 (View Customers)" << std::endl;
    std::cout << "[2] 与顾客交易 (Trade with Customer)" << std::endl;
    std::cout << "[3] 黑市商人 (Black Market)" << std::endl;
    std::cout << "[4] 查看物品栏 (View Inventory)" << std::endl;
    std::cout << "[5] 结束夜晚 (End Night)" << std::endl;
}

// Handle black market
void NightPhase::handleBlackMarket() {
    if (!m_market) {
        std::cout << "错误：市场未初始化 (Error: Market not initialized)" << std::endl;
        return;
    }
    
    bool inBlackMarket = true;
    
    while (inBlackMarket) {
        m_market->displayBlackMarket();
        
        int choice;
        std::cout << "\n选择 (Choice): ";
        std::cin >> choice;
        
        if (choice == 0) {
            inBlackMarket = false;
            continue;
        }
        
        const std::vector<BlackMarketItem>& stock = m_market->getBlackMarketStock();
        if (choice > 0 && choice <= static_cast<int>(stock.size())) {
            const BlackMarketItem& item = stock[choice - 1];
            
            std::cout << "\n购买 " << item.display_name << " ？(Buy " << item.display_name << "?)" << std::endl;
            std::cout << "价格 (Price): " << item.price << " 货币 (Money)" << std::endl;
            std::cout << "[1] 是 (Yes) [2] 否 (No): ";
            
            int confirm;
            std::cin >> confirm;
            
            if (confirm == 1) {
                if (m_player) {
                    m_market->purchaseFromBlackMarket(*m_player, item.item_name);
                }
            }
        } else {
            std::cout << "无效选择 (Invalid choice)" << std::endl;
        }
    }
}

// Process end of night
bool NightPhase::processEndOfNight(int day) {
    if (!m_market) {
        return false;
    }
    
    std::cout << "\n=== 夜晚结束 (End of Night) ===" << std::endl;
    
    // Update market trends
    m_market->updateTrends();
    MarketTrends& trends = m_market->getTrends();
    
    std::cout << "市场需求更新 (Market demand updated):" << std::endl;
    std::cout << "  食物需求 (Food demand): " << std::fixed << std::setprecision(2) 
              << trends.food_demand << std::endl;
    std::cout << "  医疗需求 (Medicine demand): " << trends.medicine_demand << std::endl;
    std::cout << "  金属需求 (Metal demand): " << trends.metal_demand << std::endl;
    
    // Charge daily fee
    std::cout << "\n需要支付每日费用 (Daily fee required): " << DAILY_FEE << " 货币 (Money)" << std::endl;
    
    // Check if player has enough money
    int playerMoney = m_player->getMoney();
    if (playerMoney < DAILY_FEE) {
        return false;  // Game over
    }
    
    // Deduct fee
    m_player->setMoney(playerMoney - DAILY_FEE);
    std::cout << "已支付费用 (Fee paid). 剩余金钱 (Remaining money): " 
              << m_player->getMoney() << std::endl;
    
    std::cout << "准备进入下一天... (Preparing for next day...)" << std::endl;
    return true;
}

