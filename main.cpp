#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <limits>

// ------------------------ Domain Layer ------------------------

class Currency {
    std::string code;    // e.g. "USD"
    std::string name;    // e.g. "US Dollar"
    std::string symbol;  // e.g. "$"

public:
    Currency() = default;

    Currency(std::string c, std::string n, std::string s)
        : code(std::move(c)), name(std::move(n)), symbol(std::move(s)) {}

    const std::string &getCode() const { return code; }
    const std::string &getName() const { return name; }
    const std::string &getSymbol() const { return symbol; }
};

// ------------------------ Exchange Rate Layer ------------------------

class ExchangeRateProvider {
public:
    virtual ~ExchangeRateProvider() = default;

    // returns multiplier for converting from -> to
    virtual double getRate(const std::string &from, const std::string &to) const = 0;

    // allow overriding rates at runtime (default: not supported)
    virtual void setCustomRate(const std::string &from, const std::string &to, double rate) {
        (void)from; (void)to; (void)rate;
        throw std::runtime_error("Custom rates not supported by this provider");
    }
};

class StaticRateProvider : public ExchangeRateProvider {
    std::string baseCurrencyCode;                // all rates are stored relative to this base
    std::map<std::string, double> baseRates;     // code -> rate vs base
    std::map<std::string, double> customRates;   // "FROM->TO" -> rate

    static std::string makeKey(const std::string &from, const std::string &to) {
        return from + "->" + to;
    }

public:
    explicit StaticRateProvider(std::string baseCode = "USD")
        : baseCurrencyCode(std::move(baseCode)) {
        // Hard-coded demo rates (approximate, for example only)
        baseRates[baseCurrencyCode] = 1.0;  // base
        baseRates["EUR"] = 0.92;            // 1 USD ≈ 0.92 EUR
        baseRates["INR"] = 83.10;           // 1 USD ≈ 83.10 INR
        baseRates["GBP"] = 0.79;            // 1 USD ≈ 0.79 GBP
        baseRates["JPY"] = 141.50;          // 1 USD ≈ 141.50 JPY
        baseRates["AUD"] = 1.47;            // 1 USD ≈ 1.47 AUD
        baseRates["CAD"] = 1.34;            // 1 USD ≈ 1.34 CAD
    }

    void registerCurrency(const std::string &code, double rateVsBase) {
        baseRates[code] = rateVsBase;
    }

    std::vector<std::string> getSupportedCodes() const {
        std::vector<std::string> codes;
        codes.reserve(baseRates.size());
        for (const auto &pair : baseRates) {
            codes.push_back(pair.first);
        }
        return codes;
    }

    double getRate(const std::string &from, const std::string &to) const override {
        if (from == to) return 1.0;

        // Check custom override first
        auto customIt = customRates.find(makeKey(from, to));
        if (customIt != customRates.end()) {
            return customIt->second;
        }

        auto itFrom = baseRates.find(from);
        auto itTo   = baseRates.find(to);

        if (itFrom == baseRates.end() || itTo == baseRates.end()) {
            throw std::runtime_error("Unsupported currency code");
        }

        double rateFrom = itFrom->second; // from vs base
        double rateTo   = itTo->second;   // to vs base

        // Convert: from -> base -> to
        return rateTo / rateFrom;
    }

    void setCustomRate(const std::string &from, const std::string &to, double rate) override {
        if (rate <= 0.0) {
            throw std::runtime_error("Rate must be positive");
        }
        customRates[makeKey(from, to)] = rate;
    }
};

// ------------------------ Application Layer ------------------------

class CurrencyConverter {
    ExchangeRateProvider &rateProvider;

public:
    explicit CurrencyConverter(ExchangeRateProvider &provider)
        : rateProvider(provider) {}

    double convert(const std::string &from, const std::string &to, double amount) const {
        if (amount < 0.0) {
            throw std::runtime_error("Amount cannot be negative");
        }
        double rate = rateProvider.getRate(from, to);
        return amount * rate;
    }
};

// ------------------------ Presentation / UI Layer ------------------------

class ConverterApp {
    StaticRateProvider rateProvider;
    CurrencyConverter  converter;
    std::map<std::string, Currency> currencies;

public:
    ConverterApp()
        : rateProvider("USD"), converter(rateProvider) {
        seedCurrencies();
    }

    void run() {
        bool running = true;
        while (running) {
            printMainMenu();
            int choice = readInt("Choose an option: ");
            std::cout << "\n";

            try {
                switch (choice) {
                    case 1:
                        handleConvert();
                        break;
                    case 2:
                        handleListCurrencies();
                        break;
                    case 3:
                        handleCustomRate();
                        break;
                    case 4:
                        printAbout();
                        break;
                    case 0:
                        running = false;
                        break;
                    default:
                        std::cout << "Unknown choice. Try again.\n";
                }
            } catch (const std::exception &ex) {
                std::cout << "Error: " << ex.what() << "\n";
            }

            if (running) {
                std::cout << "\nPress ENTER to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                std::cout << "\n";
            }
        }

        std::cout << "Goodbye!" << std::endl;
    }

private:
    static int readInt(const std::string &prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                return value;
            }
            std::cout << "Invalid number. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static double readDouble(const std::string &prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                return value;
            }
            std::cout << "Invalid number. Try again.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    static std::string readCode(const std::string &prompt) {
        std::string code;
        std::cout << prompt;
        std::cin >> code;
        for (char &c : code) c = static_cast<char>(toupper(c));
        return code;
    }

    void seedCurrencies() {
        // In a larger system this could be loaded from a database or config file
        registerCurrency({"USD", "US Dollar", "$"});
        registerCurrency({"EUR", "Euro", "€"});
        registerCurrency({"INR", "Indian Rupee", "₹"});
        registerCurrency({"GBP", "British Pound", "£"});
        registerCurrency({"JPY", "Japanese Yen", "¥"});
        registerCurrency({"AUD", "Australian Dollar", "$"});
        registerCurrency({"CAD", "Canadian Dollar", "$"});
    }

    void registerCurrency(const Currency &currency) {
        currencies[currency.getCode()] = currency;
    }

    void printMainMenu() {
        std::cout << "==============================\n";
        std::cout << "   Smart Currency Converter\n";
        std::cout << "==============================\n";
        std::cout << "1. Convert amount\n";
        std::cout << "2. List supported currencies\n";
        std::cout << "3. Override custom exchange rate\n";
        std::cout << "4. About this tool\n";
        std::cout << "0. Exit\n";
    }

    void handleConvert() {
        std::cout << "--- Convert Amount ---\n";
        std::string from = readCode("From currency code (e.g. USD): ");
        std::string to   = readCode("To currency code (e.g. INR): ");
        double amount    = readDouble("Amount: ");

        double result = converter.convert(from, to, amount);

        auto itFrom = currencies.find(from);
        auto itTo   = currencies.find(to);

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n" << amount << " "
                  << (itFrom != currencies.end() ? itFrom->second.getCode() : from)
                  << " = " << result << " "
                  << (itTo != currencies.end() ? itTo->second.getCode() : to) << "\n";
    }

    void handleListCurrencies() {
        std::cout << "--- Supported Currencies ---\n";
        std::cout << std::left << std::setw(8)  << "Code"
                  << std::setw(20) << "Name"
                  << "Symbol" << "\n";
        std::cout << "-------------------------------------\n";
        for (const auto &pair : currencies) {
            const Currency &c = pair.second;
            std::cout << std::left << std::setw(8)  << c.getCode()
                      << std::setw(20) << c.getName()
                      << c.getSymbol() << "\n";
        }
    }

    void handleCustomRate() {
        std::cout << "--- Custom Exchange Rate ---\n";
        std::string from = readCode("From currency code: ");
        std::string to   = readCode("To currency code: ");
        double rate      = readDouble("Custom rate (1 " + from + " = ? " + to + "): ");

        rateProvider.setCustomRate(from, to, rate);
        std::cout << "Custom rate updated. Future conversions will use this rate.\n";
    }

    void printAbout() {
        std::cout << "--- About ---\n";
        std::cout << "This demo shows an object-oriented design in C++.\n";
        std::cout << "Key concepts used:\n";
        std::cout << " - Encapsulation via Currency, StaticRateProvider, CurrencyConverter classes\n";
        std::cout << " - Inheritance and polymorphism via ExchangeRateProvider interface\n";
        std::cout << " - Separation of concerns (domain, application, UI layers)\n";
    }
};

int main() {
    ConverterApp app;
    app.run();
    return 0;
}