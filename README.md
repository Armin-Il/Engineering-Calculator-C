# 🧮 Engineering Calculator (C)

A command-line engineering calculator written in **C**, combining a custom expression parser, matrix operations, unit conversion, persistent calculation memory, and ASCII function plotting — all in a single-file application.

---

## 📌 Overview

`main.c` implements a menu-driven engineering calculator with six core modules:

1. **Engineering Calculator** — evaluates math expressions typed as text (`sin(30)+sqrt(16)`, etc.)
2. **Unit Converter** — length, temperature, area, volume, weight
3. **Matrix Operations** — addition, subtraction, multiplication, determinant, inverse
4. **View Memory** — shows saved calculation history from `memory.txt`
5. **Clear Memory** — wipes `memory.txt`
6. **Plot Graph** — ASCII plots of common functions in the terminal

---

## ✨ Features

### 🧮 Expression Calculator
A hand-written recursive-descent parser (`parse_factor` → `parse_power` → `parse_term` → `evaluate_expression`) supports:

- Operators: `+ - * / ^` and parentheses
- Trig: `sin`, `cos`, `tan`, `cot` and their inverses `asin`, `acos`, `atan`, `acot`
- `log(base, value)`, `ln(value)`, `sqrt(value)`, `abs(value)`, `exp(value)`
- `fact(n)` (factorial), `pow(base, exponent)`, `rnd` (random 0–1)
- Descriptive error handling for invalid input, without crashing the program (e.g. division by zero, log of a non-positive number, domain errors for `asin`/`acos`, unmatched parentheses)
- Every successful evaluation is auto-saved to `memory.txt`

> **Note:** the calculator currently runs in **degrees mode by default** each session — `set_angle_mode()` supports radians internally, but there's no menu option yet to switch modes at runtime. Listed under Roadmap below.

### 🔄 Unit Converter
Five conversion categories, each with source/target unit prompts:
- **Length** — meters, feet, inches, miles, kilometers, centimeters
- **Temperature** — Celsius, Fahrenheit, Kelvin
- **Area**
- **Volume**
- **Weight**

### 📐 Matrix Operations
Operates on integer matrices up to `5×5` (`MAX_MATRIX`):
- Addition, subtraction, multiplication (with dimension-compatibility checks)
- Determinant — 2×2 and 3×3
- Inverse — via Gauss-Jordan elimination with partial pivoting, works for any size the determinant step supports, with a singularity check

### 📊 Function Plotting
ASCII plots (100×30 character grid) with axis lines, for:
- Linear: `y = mx`
- Quadratic: `y = ax²`
- Sine / Cosine / Tangent / Cotangent: `y = a·f(x)`
- Logarithmic: `y = a·log(x)`

Parameter values are validated to stay within a sensible plotting range (1–10).

### 💾 Persistent Calculation Memory
- Every evaluated expression is appended to `memory.txt` as `expression = result`
- View or clear the saved history from the main menu

---

## 🛠️ Technologies

| Technology | Purpose |
|---|---|
| **C** | Core application logic |
| **GCC** | Compilation |
| **math.h** | Trigonometric/logarithmic/exponential functions |
| **File I/O** | Persistent memory (`memory.txt`) |

---

## 📂 Project Structure

```text
Engineering-Calculator-C/
│
├── main.c
├── memory.txt        # created automatically on first calculation
├── README.md
├── LICENSE
└── .gitignore
```

The project is currently a single-file implementation (~1200 lines). Modularizing into separate source/header files is planned — see Roadmap.

---

## ⚙️ Requirements

- GCC compiler
- Standard C library
- A terminal / command-line environment

No external frameworks or libraries are required.

---

## 🚀 How to Compile & Run

### Linux / macOS
```bash
git clone https://github.com/Armin-Il/Engineering-Calculator-C.git
cd Engineering-Calculator-C
gcc main.c -o calculator -lm
./calculator
```

### Windows
```bash
git clone https://github.com/Armin-Il/Engineering-Calculator-C.git
cd Engineering-Calculator-C
gcc main.c -o calculator.exe -lm
calculator.exe
```

---

## 🖥️ Example Interface

```
=========================================
   Engineering Calculator - Version 1.1
=========================================
Main Menu:
1. Engineering Calculator
2. Unit Converter
3. Matrix Operations
4. View Memory
5. Clear Memory
6. Plot Graph
7. Exit
Enter your choice:
```

Example expression session:
```
Enter an expression (or 'exit' to quit): sin(30)+sqrt(16)
Evaluating: sin(30)+sqrt(16)
Result: 4.500000
```

---

## 🎯 Learning Objectives

This project strengthens practical understanding of:
- Writing a recursive-descent expression parser from scratch in C
- Numerical methods (Gauss-Jordan elimination for matrix inversion)
- File I/O and persistent data storage
- Defensive input handling (`scanf`/`fgets` buffering issues, invalid input reprompting)
- CLI menu design and function pointers (used for the plotting module)

---
📈 Version History
### v1.1 — Current Version

- Added recursive-descent expression parsing
- Expanded mathematical expression support
- Improved error handling and input validation
- Added matrix addition, subtraction, multiplication, determinant, and inverse
- Expanded unit conversion to five categories
- Added ASCII function plotting
- Improved persistent calculation memory

### v1.0 — Initial Release

- Initial engineering calculator implementation
- Basic mathematical operations
- Unit conversion
- Matrix operations
- Calculation memory using file I/O
---

## 🔮 Roadmap

Phase 1 — Core Calculator ✅
Phase 2 — Engineering Features ✅
- [ ] Expose angle-mode switching (degrees/radians) in the main calculator menu
- [ ] Modularize into separate `.c`/`.h` files
- [ ] Extend determinant/inverse support beyond 2×2/3×3 input validation edge cases
- [ ] Add automated unit tests
- [ ] Add a Makefile / CMake build system
- [ ] Set up CI via GitHub Actions
- [ ] Expand unit conversion categories
- [ ] Improve plot resolution / add axis labels and scaling options

---

## 🤝 Contributing

This project is primarily maintained as a personal engineering/programming project, but suggestions and contributions are welcome:

1. Fork the repository
2. Create a new branch
3. Implement and test your changes
4. Submit a pull request

---

## 📄 License

Licensed under the **MIT License** — see [LICENSE](LICENSE) for details.

---
## 👨‍💻 Author

**Armin Ilat**  
Electrical Engineering Student

**Interests:**  
Electrical Engineering · Scientific Computing · Programming · Digital Communications · Control Systems · Engineering Simulation

### 🔗 Links

- **LinkedIn:** [linkedin.com/in/armin-ilat](https://www.linkedin.com/in/armin-ilat/)
- **GitHub:** [github.com/Armin-Il](https://github.com/Armin-Il)
- **YouTube:** [@VoltVerse-Electrical](https://www.youtube.com/@VoltVerse-Electrical)

---

## ⭐ Support

If you find this project useful, consider giving it a ⭐ on GitHub — thanks for checking it out!
