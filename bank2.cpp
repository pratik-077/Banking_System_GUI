#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <sstream>

struct BankAccount {
    std::string name;
    double balance;
    std::vector<std::string> transactionHistory;
};

BankAccount account; // Using a single account for simplicity

void deposit(double amount) {
    if (amount > 0) {
        account.balance += amount;
        account.transactionHistory.push_back("Deposit: +" + std::to_string(amount));
    }
}

void withdraw(double amount) {
    if (amount > 0 && amount <= account.balance) {
        account.balance -= amount;
        account.transactionHistory.push_back("Withdraw: -" + std::to_string(amount));
    }
}

bool isMouseOverButton(const sf::Vector2i& mousePos, const sf::RectangleShape& button) {
    sf::FloatRect buttonBounds = button.getGlobalBounds();
    return buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Banking System");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Font loading failed." << std::endl;
        return 1;
    }

    account.name = "John Doe";
    account.balance = 1000.0; // Initial balance

    // Text input fields for custom deposit and withdrawal amounts
    sf::Text depositAmountText;
    depositAmountText.setFont(font);
    depositAmountText.setCharacterSize(24);
    depositAmountText.setFillColor(sf::Color::Black);
    depositAmountText.setPosition(20, 20);

    sf::Text withdrawAmountText;
    withdrawAmountText.setFont(font);
    withdrawAmountText.setCharacterSize(24);
    withdrawAmountText.setFillColor(sf::Color::Black);
    withdrawAmountText.setPosition(20, 60);

    sf::RectangleShape depositButton(sf::Vector2f(150, 50));
    depositButton.setFillColor(sf::Color::Green);
    depositButton.setPosition(20, 100);

    sf::Text depositButtonText;
    depositButtonText.setFont(font);
    depositButtonText.setCharacterSize(24);
    depositButtonText.setFillColor(sf::Color::White);
    depositButtonText.setString("Deposit");
    depositButtonText.setPosition(35, 110);

    sf::RectangleShape withdrawButton(sf::Vector2f(150, 50));
    withdrawButton.setFillColor(sf::Color::Red);
    withdrawButton.setPosition(200, 100);

    sf::Text withdrawButtonText;
    withdrawButtonText.setFont(font);
    withdrawButtonText.setCharacterSize(24);
    withdrawButtonText.setFillColor(sf::Color::White);
    withdrawButtonText.setString("Withdraw");
    withdrawButtonText.setPosition(215, 110);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 48 && event.text.unicode <= 57) {
                    // Only allow numeric input (0-9) for deposit and withdrawal amounts
                    depositAmountText.setString(depositAmountText.getString() + static_cast<char>(event.text.unicode));
                    withdrawAmountText.setString(withdrawAmountText.getString() + static_cast<char>(event.text.unicode));
                } else if (event.text.unicode == 8) {
                    // Backspace to delete characters
                    std::string depositText = depositAmountText.getString();
                    if (!depositText.empty()) {
                        depositText.pop_back();
                        depositAmountText.setString(depositText);
                    }

                    std::string withdrawText = withdrawAmountText.getString();
                    if (!withdrawText.empty()) {
                        withdrawText.pop_back();
                        withdrawAmountText.setString(withdrawText);
                    }
                }
            }

            // Handle button clicks
            if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Deposit button clicked
                if (isMouseOverButton(mousePos, depositButton)) {
                    std::istringstream depositInput(depositAmountText.getString());
                    double depositAmount;
                    if (depositInput >> depositAmount) {
                        deposit(depositAmount);
                        depositAmountText.setString(""); // Clear the input field
                    }
                }

                // Withdraw button clicked
                if (isMouseOverButton(mousePos, withdrawButton)) {
                    std::istringstream withdrawInput(withdrawAmountText.getString());
                    double withdrawAmount;
                    if (withdrawInput >> withdrawAmount) {
                        withdraw(withdrawAmount);
                        withdrawAmountText.setString(""); // Clear the input field
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // Display input fields for custom deposit and withdrawal amounts
        window.draw(depositAmountText);
        window.draw(withdrawAmountText);

        // Draw deposit button
        window.draw(depositButton);
        window.draw(depositButtonText);

        // Draw withdraw button
        window.draw(withdrawButton);
        window.draw(withdrawButtonText);

        // Update and display account information
        sf::Text accountInfo;
        accountInfo.setFont(font);
        accountInfo.setCharacterSize(24);
        accountInfo.setFillColor(sf::Color::Black);
        accountInfo.setPosition(20, 200);
        accountInfo.setString("Account Holder: " + account.name);

        sf::Text balanceText;
        balanceText.setFont(font);
        balanceText.setCharacterSize(24);
        balanceText.setFillColor(sf::Color::Black);
        balanceText.setPosition(20, 240);
        balanceText.setString("Balance: $" + std::to_string(account.balance));

        sf::Text historyText;
        historyText.setFont(font);
        historyText.setCharacterSize(18);
        historyText.setFillColor(sf::Color::Black);
        historyText.setPosition(20, 280);
        historyText.setString("Transaction History:\n");
        for (const std::string& transaction : account.transactionHistory) {
            historyText.setString(historyText.getString() + transaction + "\n");
        }

        window.draw(accountInfo);
        window.draw(balanceText);
        window.draw(historyText);

        window.display();
    }

    return 0;
}
