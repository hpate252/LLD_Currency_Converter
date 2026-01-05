# Smart Currency Converter (C++ OOP)

A simple, object-oriented **currency converter** written in modern C++ (C++17).  
It converts amounts between multiple currencies (USD, EUR, INR, GBP, JPY, AUD, CAD) and lets you **override exchange rates at runtime** without editing the code.

The project is intentionally structured to showcase **OOP concepts** like encapsulation, inheritance, and polymorphism, while still being easy to run as a console application.

---

## 🔧 Features

- Convert amounts between popular currencies:
  - USD, EUR, INR, GBP, JPY, AUD, CAD
- Menu-driven CLI (console app)
- Basic input validation (invalid numbers, negative amounts, etc.)
- Custom exchange rate overrides:
  - Set your own rate: `1 FROM = X TO`
  - The app will prefer your custom rate for future conversions
- Clean OOP design:
  - `Currency` (data model)
  - `ExchangeRateProvider` (abstract base class / interface)
  - `StaticRateProvider` (concrete implementation)
  - `CurrencyConverter` (business logic)
  - `ConverterApp` (UI & control flow)

---

## 🧱 Tech Stack

- **Language:** C++17
- **Build Tool:** Any C++17-compatible compiler (g++, clang++, MSVC)
- **Libraries:** Only the C++ Standard Library (no external dependencies)

---

## 📁 Project Structure

```text
.
├── main.cpp      # All classes + main() function
└── README.md     # Project documentation