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
  - `ExchangeRateProvider` (abstract base class/interface)
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

✅ Prerequisites

You need a C++17-capable compiler installed.

On Windows
You can use any of these: MinGW-w64 g++
                          MSYS2 g++

Visual Studio (with Desktop Development with C++)

Make sure g++ is available in your terminal/PowerShell: g++ --version


If this prints a version, you’re good.

On Linux / macOS

Ubuntu/Debian:sudo apt-get update
              sudo apt-get install g++


macOS (via Homebrew): brew install gcc

🚀 How to Build & Run
Windows (PowerShell)

From the project folder: g++ -std=c++17 main.cpp -o currency_converter.exe
                          .\currency_converter.exe


Note: In PowerShell you must use .\ to run an .exe in the current directory.

Linux / macOS g++ -std=c++17 main.cpp -o currency_converter ./currency_converter

🧭 Using the Application When you run the program, you’ll see a menu like:

==============================
   Smart Currency Converter
==============================
1. Convert amount
2. List supported currencies
3. Override custom exchange rate
4. About this tool
0. Exit

Option 1: Convert amount

Choose 1.

Enter a from currency code (e.g. USD, INR, EUR).
Enter a to currency code.
Enter the numeric amount.

Example:
From currency code (e.g. USD): USD
To currency code (e.g. INR): INR
Amount: 10

Output (example): 10.00 USD = 831.00 INR

(Exact values depend on the hard-coded or custom rates.)

Option 2: List supported currencies

Shows a table like:

Code    Name                 Symbol
-------------------------------------
AUD     Australian Dollar    $
CAD     Canadian Dollar      $
EUR     Euro                 €
GBP     British Pound        £
INR     Indian Rupee         ₹
JPY     Japanese Yen         ¥
USD     US Dollar            $

Option 3: Override custom exchange rate
Example:

From currency code: USD
To currency code: INR
Custom rate (1 USD = ? INR): 90

Now, USD → INR conversions will use 1 USD = 90 INR instead of the default static rate.
Option 4: About this tool
Prints a short explanation of the design and the OOP concepts used.
Option 0: Exit
Quits the application.
