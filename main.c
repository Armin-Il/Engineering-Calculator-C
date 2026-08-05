#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>

#define MAX_EXPR_LENGTH 100
#define MEMORY_FILE "memory.txt"

#define WIDTH 100   // Plot width
#define HEIGHT 30   // Plot height

#define MAX_MATRIX 5

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ---------------------------------------------------------------------
// Global error flag for parser.
// Prevents exit() calls on evaluation errors so calculate() can handling 
// errors gracefully without crashing the main loop.
// ---------------------------------------------------------------------
static bool g_calc_error = false;
static char g_calc_error_msg[128] = "";

static void set_calc_error(const char* msg) {
    g_calc_error = true;
    strncpy(g_calc_error_msg, msg, sizeof(g_calc_error_msg) - 1);
    g_calc_error_msg[sizeof(g_calc_error_msg) - 1] = '\0';
}

// Engineering Calculator functions
typedef struct {
    char angle_mode[10];
} EngineeringCalculator;

void set_angle_mode(EngineeringCalculator* calc, const char* mode);
double factorial(int n);
double random_number();
double log_base(double value, double base);
double evaluate_expression(const char* expr, const char* angle_mode);
double get_number(const char** expr);
void append_to_memory(const char* record);
void view_memory();
void clear_memory();
void calculate(EngineeringCalculator* calc, const char* expression);
void LengthConvert();
void ConvertAllLengths();
void TemperatureConvert();
void ConvertAllTemperatures();
void AreaConvert();
void ConvertAllareas();
void volumeConvert();
void ConvertAllVolumes();
void WeightConvert();
void ConvertAllWeights();
void inputMatrix(int matrix[MAX_MATRIX][MAX_MATRIX], int rows, int cols);
void printMatrix(int matrix[MAX_MATRIX][MAX_MATRIX], int rows, int cols);
void addMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows, int cols);
void subtractMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows, int cols);
void multiplyMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows1, int cols1, int rows2, int cols2);
double calculateDeterminant(int matrix[MAX_MATRIX][MAX_MATRIX], int size);
void calculateInverse(int matrix[MAX_MATRIX][MAX_MATRIX], int size);
void matrixOperations();
void determinantMenu();
void unitConverterMenu();
void plotMenu();

bool is_valid_input(double value);
void plot_function(double (*func)(double, double), const char *title, double param);
double linear_function(double x, double slope);
double quadratic_function(double x, double a);
double sin_function(double x, double param);
double cos_function(double x, double param);
double tan_function(double x, double param);
double cot_function(double x, double param);
double log_function(double x, double param);

// ---------------------------------------------------------------------
// Input helper functions to fix standard scanf/fgets buffering issues
// ---------------------------------------------------------------------

// Flushes standard input buffer up to '\n' or EOF
static void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard leftover characters
    }
}

// Safely reads an integer, reprompting on invalid input
static int read_int(const char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            clear_input_buffer();
            return value;
        }
        printf("Invalid input. Please enter an integer.\n");
        clear_input_buffer();
    }
}

// Safely reads a floating-point value
static double read_double(const char* prompt) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1) {
            clear_input_buffer();
            return value;
        }
        printf("Invalid input. Please enter a number.\n");
        clear_input_buffer();
    }
}

// Safely reads a single word token
static void read_word(const char* prompt, char* buffer, size_t buffer_size) {
    printf("%s", prompt);
    scanf("%19s", buffer);
    (void)buffer_size;
    clear_input_buffer();
}

int main() {
    printf("=========================================\n");
    printf("   Engineering Calculator - Version 1.1  \n");
    printf("=========================================\n");
    srand((unsigned int)time(NULL)); // Seed RNG once at main startup

    int choice;
    do {
        printf("Main Menu:\n");
        printf("1. Engineering Calculator\n");
        printf("2. Unit Converter\n");
        printf("3. Matrix Operations\n");
        printf("4. View Memory\n");
        printf("5. Clear Memory\n");
        printf("6. Plot Graph\n");
        printf("7. Exit\n");
        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1: {
                EngineeringCalculator calc;
                set_angle_mode(&calc, "degrees");

                char input[MAX_EXPR_LENGTH];
                while (1) {
                    printf("Enter an expression (or 'exit' to quit): ");
                    if (fgets(input, sizeof(input), stdin) == NULL) break;
                    input[strcspn(input, "\n")] = 0;  // Remove newline character

                    if (strcmp(input, "exit") == 0) {
                        break;
                    }
                    if (strlen(input) == 0) {
                        continue; // Skip blank line
                    }

                    calculate(&calc, input);
                }
                break;
            }
            case 2:
                unitConverterMenu();
                break;
            case 3:
                matrixOperations();
                break;
            case 4:
                view_memory();
                break;
            case 5:
                clear_memory();
                break;
            case 6:
                plotMenu();
                break;
            case 7:
                printf("Exiting program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

void set_angle_mode(EngineeringCalculator* calc, const char* mode) {
    if (strcmp(mode, "degrees") == 0 || strcmp(mode, "radians") == 0) {
        strncpy(calc->angle_mode, mode, sizeof(calc->angle_mode) - 1);
        calc->angle_mode[sizeof(calc->angle_mode) - 1] = '\0';
    } else {
        printf("Mode must be 'degrees' or 'radians'.\n");
    }
}

double factorial(int n) {
    if (n == 0 || n == 1) return 1;
    else return n * factorial(n - 1);
}

double random_number() {
    // Note: srand() is called in main() to prevent repeated sequences in fast calls
    return (double)rand() / RAND_MAX;
}

double log_base(double value, double base) {
    if (value <= 0 || base <= 0) {
        set_calc_error("Logarithm with non-positive value or base is undefined.");
        return 0;
    }
    return log(value) / log(base);
}

double get_number(const char** expr) {
    double number = 0;
    sscanf(*expr, "%lf", &number);
    while (isdigit(**expr) || **expr == '.') (*expr)++;
    return number;
}

void append_to_memory(const char* record) {
    FILE* file = fopen(MEMORY_FILE, "a");
    if (file == NULL) {
        printf("Error opening memory file for appending.\n");
        return;
    }
    fprintf(file, "%s\n", record);
    fclose(file);
}

void view_memory() {
    FILE* file = fopen(MEMORY_FILE, "r");
    if (file == NULL) {
        printf("No memory file found or error opening memory file.\n");
        return;
    }
    char line[256];
    printf("\n--- Memory Contents ---\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    printf("--- End of Memory ---\n");
    fclose(file);
}

void clear_memory() {
    FILE* file = fopen(MEMORY_FILE, "w");
    if (file == NULL) {
        printf("Error opening memory file for clearing.\n");
        return;
    }
    fclose(file);
    printf("Memory cleared.\n");
}

double evaluate_expression(const char* expr, const char* angle_mode);

double parse_factor(const char** expr, const char* angle_mode) {
    while (**expr == ' ') (*expr)++;

    double result = 0;
    if (g_calc_error) return 0; // Return early on previous error

    if (**expr == '(') {
        (*expr)++;
        result = evaluate_expression(*expr, angle_mode);
        // Advance to matching closing parenthesis without buffer overflow
        while (**expr != ')' && **expr != '\0') (*expr)++;
        if (**expr == ')') {
            (*expr)++;
        } else {
            set_calc_error("Missing closing parenthesis.");
            return 0;
        }
    } else if (**expr == '\0') {
        set_calc_error("Incomplete expression.");
        return 0;
    } else if (strncmp(*expr, "asin", 4) == 0) {
        *expr += 4;
        double value = parse_factor(expr, angle_mode);
        if (value < -1 || value > 1) {
            set_calc_error("Invalid domain for arcsin.");
            return 0;
        }
        result = asin(value) * (strcmp(angle_mode, "degrees") == 0 ? 180 / M_PI : 1);
    } else if (strncmp(*expr, "acos", 4) == 0) {
        *expr += 4;
        double value = parse_factor(expr, angle_mode);
        if (value < -1 || value > 1) {
            set_calc_error("Invalid domain for arccos.");
            return 0;
        }
        result = acos(value) * (strcmp(angle_mode, "degrees") == 0 ? 180 / M_PI : 1);
    } else if (strncmp(*expr, "atan", 4) == 0) {
        *expr += 4;
        result = atan(parse_factor(expr, angle_mode)) * (strcmp(angle_mode, "degrees") == 0 ? 180 / M_PI : 1);
    } else if (strncmp(*expr, "acot", 4) == 0) {
        *expr += 4;
        result = (M_PI / 2 - atan(parse_factor(expr, angle_mode))) * (strcmp(angle_mode, "degrees") == 0 ? 180 / M_PI : 1);
    } else if (strncmp(*expr, "sin", 3) == 0) {
        *expr += 3;
        result = sin(parse_factor(expr, angle_mode) * (strcmp(angle_mode, "degrees") == 0 ? M_PI / 180 : 1));
    } else if (strncmp(*expr, "cos", 3) == 0) {
        *expr += 3;
        result = cos(parse_factor(expr, angle_mode) * (strcmp(angle_mode, "degrees") == 0 ? M_PI / 180 : 1));
    } else if (strncmp(*expr, "tan", 3) == 0) {
        *expr += 3;
        result = tan(parse_factor(expr, angle_mode) * (strcmp(angle_mode, "degrees") == 0 ? M_PI / 180 : 1));
    } else if (strncmp(*expr, "cot", 3) == 0) {
        *expr += 3;
        double angle = parse_factor(expr, angle_mode);
        double t = tan(angle * (strcmp(angle_mode, "degrees") == 0 ? M_PI / 180 : 1));
        if (t == 0) {
            set_calc_error("Cotangent of zero is undefined.");
            return 0;
        }
        result = 1 / t;
    } else if (strncmp(*expr, "log", 3) == 0) {
        *expr += 3;
        double base = parse_factor(expr, angle_mode);
        result = log_base(parse_factor(expr, angle_mode), base);
    } else if (strncmp(*expr, "ln", 2) == 0) {
        *expr += 2;
        double value = parse_factor(expr, angle_mode);
        if (value <= 0) {
            set_calc_error("Natural log of non-positive value is undefined.");
            return 0;
        }
        result = log(value);
    } else if (strncmp(*expr, "sqrt", 4) == 0) {
        *expr += 4;
        double value = parse_factor(expr, angle_mode);
        if (value < 0) {
            set_calc_error("Square root of negative number is undefined.");
            return 0;
        }
        result = sqrt(value);
    } else if (strncmp(*expr, "abs", 3) == 0) {
        *expr += 3;
        result = fabs(parse_factor(expr, angle_mode));
    } else if (strncmp(*expr, "exp", 3) == 0) {
        *expr += 3;
        result = exp(parse_factor(expr, angle_mode));
    } else if (strncmp(*expr, "fact", 4) == 0) {
        *expr += 4;
        double value = parse_factor(expr, angle_mode);
        if (value < 0) {
            set_calc_error("Factorial of negative integer is undefined.");
            return 0;
        }
        result = factorial((int)value);
    } else if (strncmp(*expr, "rnd", 3) == 0) {
        *expr += 3;
        result = random_number();
    } else if (strncmp(*expr, "pow", 3) == 0) {
        *expr += 3;
        double base = parse_factor(expr, angle_mode);
        result = pow(base, parse_factor(expr, angle_mode));
    } else if (isdigit((unsigned char)**expr) || **expr == '.') {
        result = get_number(expr);
    } else {
        set_calc_error("Unknown token in expression.");
        return 0;
    }

    return result;
}

double parse_power(const char** expr, const char* angle_mode) {
    double result = parse_factor(expr, angle_mode);
    if (g_calc_error) return 0;

    while (**expr == '^') {
        (*expr)++;
        double factor = parse_factor(expr, angle_mode);
        if (g_calc_error) return 0;
        result = pow(result, factor);
    }

    return result;
}

double parse_term(const char** expr, const char* angle_mode) {
    double result = parse_power(expr, angle_mode);
    if (g_calc_error) return 0;

    while (**expr == '*' || **expr == '/') {
        char op = **expr;
        (*expr)++;
        double power = parse_power(expr, angle_mode);
        if (g_calc_error) return 0;
        if (op == '*') result *= power;
        else {
            if (power == 0) {
                set_calc_error("Division by zero.");
                return 0;
            }
            result /= power;
        }
    }

    return result;
}

double evaluate_expression(const char* expr, const char* angle_mode) {
    double result = parse_term(&expr, angle_mode);
    if (g_calc_error) return 0;

    while (*expr == '+' || *expr == '-') {
        char op = *expr;
        expr++;
        double term = parse_term(&expr, angle_mode);
        if (g_calc_error) return 0;
        if (op == '+') result += term;
        else result -= term;
    }

    return result;
}

void calculate(EngineeringCalculator* calc, const char* expression) {
    printf("Evaluating: %s\n", expression);

    g_calc_error = false;
    g_calc_error_msg[0] = '\0';

    double result = evaluate_expression(expression, calc->angle_mode);

    if (g_calc_error) {
        printf("Error: %s\n", g_calc_error_msg);
        return; // Return to caller without exiting app
    }

    printf("Result: %f\n", result);

    char record[256];
    snprintf(record, sizeof(record), "%s = %f", expression, result);
    append_to_memory(record);
}

// ---------------------------------------------------------------------
// Unit Converter functions
// ---------------------------------------------------------------------
double toMeters(const char* unit);
double fromMeters(const char* unit);
double toSquareMeters(const char* unit);
double fromSquareMeters(const char* unit);
double toCubicMeters(const char* unit);
double fromCubicMeters(const char* unit);
double toKilograms(const char* unit);
double fromKilograms(const char* unit);

void LengthConvert() {
    char fromUnit[20], toUnit[20];
    double input, result;

    printf("\nAvailable Units: meters, feet, inches, miles, kilometers, centimeters\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    read_word("Enter the target unit: ", toUnit, sizeof(toUnit));
    input = read_double("Enter the value to convert: ");

    double fromFactor = toMeters(fromUnit);
    double toFactor = fromMeters(toUnit);

    if (fromFactor == -1 || toFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }
    result = input * fromFactor * toFactor;
    printf("%.2lf %s = %.2lf %s\n", input, fromUnit, result, toUnit);
}

double toMeters(const char* unit) {
    if (strcmp(unit, "meters") == 0) return 1.0;
    if (strcmp(unit, "feet") == 0) return 0.3048;
    if (strcmp(unit, "inches") == 0) return 0.0254;
    if (strcmp(unit, "miles") == 0) return 1609.34;
    if (strcmp(unit, "kilometers") == 0) return 1000.0;
    if (strcmp(unit, "centimeters") == 0) return 0.01;
    return -1;
}

double fromMeters(const char* unit) {
    if (strcmp(unit, "meters") == 0) return 1.0;
    if (strcmp(unit, "feet") == 0) return 1 / 0.3048;
    if (strcmp(unit, "inches") == 0) return 1 / 0.0254;
    if (strcmp(unit, "miles") == 0) return 1 / 1609.34;
    if (strcmp(unit, "kilometers") == 0) return 1 / 1000.0;
    if (strcmp(unit, "centimeters") == 0) return 1 / 0.01;
    return -1;
}

void ConvertAllLengths() {
    char fromUnit[20];
    double input, baseValue;
    const char* units[] = { "meters", "feet", "inches", "miles", "kilometers", "centimeters" };
    int unitCount = sizeof(units) / sizeof(units[0]);
    printf("\nAvailable Units: meters, feet, inches, miles, kilometers, centimeters\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    input = read_double("Enter the value to convert: ");
    double fromFactor = toMeters(fromUnit);
    if (fromFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }
    baseValue = input * fromFactor;
    printf("\nConversions for %.2lf %s:\n", input, fromUnit);
    for (int i = 0; i < unitCount; i++) {
        double convertedValue = baseValue * fromMeters(units[i]);
        printf("  %.2lf %s\n", convertedValue, units[i]);
    }
}

void TemperatureConvert() {
    char fromUnit[20], toUnit[20];
    double input, result;

    printf("\nAvailable Units: Celsius, Fahrenheit, Kelvin\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    read_word("Enter the target unit: ", toUnit, sizeof(toUnit));
    input = read_double("Enter the value to convert: ");

    if (strcmp(fromUnit, "Celsius") == 0) {
        if (strcmp(toUnit, "Fahrenheit") == 0) {
            result = (input * 9.0 / 5.0) + 32;
        } else if (strcmp(toUnit, "Kelvin") == 0) {
            result = input + 273.15;
        } else if (strcmp(toUnit, "Celsius") == 0) {
            result = input;
        } else {
            printf("Invalid target unit.\n");
            return;
        }
    } else if (strcmp(fromUnit, "Fahrenheit") == 0) {
        if (strcmp(toUnit, "Celsius") == 0) {
            result = (input - 32) * 5.0 / 9.0;
        } else if (strcmp(toUnit, "Kelvin") == 0) {
            result = (input - 32) * 5.0 / 9.0 + 273.15;
        } else if (strcmp(toUnit, "Fahrenheit") == 0) {
            result = input;
        } else {
            printf("Invalid target unit.\n");
            return;
        }
    } else if (strcmp(fromUnit, "Kelvin") == 0) {
        if (strcmp(toUnit, "Celsius") == 0) {
            result = input - 273.15;
        } else if (strcmp(toUnit, "Fahrenheit") == 0) {
            result = (input - 273.15) * 9.0 / 5.0 + 32;
        } else if (strcmp(toUnit, "Kelvin") == 0) {
            result = input;
        } else {
            printf("Invalid target unit.\n");
            return;
        }
    } else {
        printf("Invalid source unit.\n");
        return;
    }

    printf("%.2lf %s = %.2lf %s\n", input, fromUnit, result, toUnit);
}

void ConvertAllTemperatures() {
    char fromUnit[20];
    double input;

    printf("\nAvailable Units: Celsius, Fahrenheit, Kelvin\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    input = read_double("Enter the value to convert: ");

    if (strcmp(fromUnit, "Celsius") == 0) {
        printf("%.2lf Celsius = %.2lf Fahrenheit\n", input, (input * 9.0 / 5.0) + 32);
        printf("%.2lf Celsius = %.2lf Kelvin\n", input, input + 273.15);
    } else if (strcmp(fromUnit, "Fahrenheit") == 0) {
        printf("%.2lf Fahrenheit = %.2lf Celsius\n", input, (input - 32) * 5.0 / 9.0);
        printf("%.2lf Fahrenheit = %.2lf Kelvin\n", input, (input - 32) * 5.0 / 9.0 + 273.15);
    } else if (strcmp(fromUnit, "Kelvin") == 0) {
        printf("%.2lf Kelvin = %.2lf Celsius\n", input, input - 273.15);
        printf("%.2lf Kelvin = %.2lf Fahrenheit\n", input, (input - 273.15) * 9.0 / 5.0 + 32);
    } else {
        printf("Invalid source unit.\n");
        return;
    }
}

void AreaConvert() {
    char fromUnit[20], toUnit[20];
    double input, result;

    printf("\nAvailable Units: square meters (m2), square centimeters (cm2), square kilometers (km2), ");
    printf("square feet (ft2), square yards (yd2), square miles (mi2), acres, hectares (ha)\n");

    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    read_word("Enter the target unit: ", toUnit, sizeof(toUnit));
    input = read_double("Enter the value to convert: ");
    double fromFactor = toSquareMeters(fromUnit);
    double toFactor = fromSquareMeters(toUnit);

    if (fromFactor == -1 || toFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }

    result = input * fromFactor * toFactor;
    printf("%.2lf %s = %.2lf %s\n", input, fromUnit, result, toUnit);
}

double toSquareMeters(const char* unit) {
    if (strcmp(unit, "m2") == 0) return 1.0;
    if (strcmp(unit, "cm2") == 0) return 0.0001;
    if (strcmp(unit, "km2") == 0) return 1000000.0;
    if (strcmp(unit, "ft2") == 0) return 0.092903;
    if (strcmp(unit, "yd2") == 0) return 0.836127;
    if (strcmp(unit, "mi2") == 0) return 2589988.0;
    if (strcmp(unit, "acres") == 0) return 4046.856;
    if (strcmp(unit, "ha") == 0) return 10000.0;
    return -1;
}

double fromSquareMeters(const char* unit) {
    if (strcmp(unit, "m2") == 0) return 1.0;
    if (strcmp(unit, "cm2") == 0) return 10000.0;
    if (strcmp(unit, "km2") == 0) return 1 / 1000000.0;
    if (strcmp(unit, "ft2") == 0) return 1 / 0.092903;
    if (strcmp(unit, "yd2") == 0) return 1 / 0.836127;
    if (strcmp(unit, "mi2") == 0) return 1 / 2589988.0;
    if (strcmp(unit, "acres") == 0) return 1 / 4046.856;
    if (strcmp(unit, "ha") == 0) return 1 / 10000.0;
    return -1;
}

void ConvertAllareas() {
    char fromUnit[20];
    double input;

    printf("\nAvailable Units: square meters (m2), square centimeters (cm2), square kilometers (km2), ");
    printf("square feet (ft2), square yards (yd2), square miles (mi2), acres, hectares (ha)\n");

    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    input = read_double("Enter the value to convert: ");

    double fromFactor = toSquareMeters(fromUnit);

    if (fromFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }

    printf("\nConversions for %.2lf %s:\n", input, fromUnit);

    const char* units[] = { "m2", "cm2", "km2", "ft2", "yd2", "mi2", "acres", "ha" };
    int numUnits = sizeof(units) / sizeof(units[0]);
    for (int i = 0; i < numUnits; i++) {
        double toFactor = fromSquareMeters(units[i]);
        double result = input * fromFactor * toFactor;
        printf("%.2lf %s\n", result, units[i]);
    }
}

void volumeConvert() {
    char fromUnit[20], toUnit[20];
    double input, result;
    printf("\nAvailable Units: liters, milliliters, cubic meters, cubic centimeters, gallons\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    read_word("Enter the target unit: ", toUnit, sizeof(toUnit));
    input = read_double("Enter the value to convert: ");

    double fromFactor = toCubicMeters(fromUnit);
    double toFactor = fromCubicMeters(toUnit);
    if (fromFactor == -1 || toFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }
    result = input * fromFactor * toFactor;
    printf("%.2lf %s = %.2lf %s\n", input, fromUnit, result, toUnit);
}

double toCubicMeters(const char* unit) {
    if (strcmp(unit, "liters") == 0) return 0.001;
    if (strcmp(unit, "milliliters") == 0) return 0.000001;
    if (strcmp(unit, "cubic") == 0) return 1.0; // Multi-word unit token fallback
    if (strcmp(unit, "gallons") == 0) return 0.00378541;
    return -1;
}

double fromCubicMeters(const char* unit) {
    if (strcmp(unit, "liters") == 0) return 1000.0;
    if (strcmp(unit, "milliliters") == 0) return 1000000.0;
    if (strcmp(unit, "cubic") == 0) return 1.0;
    if (strcmp(unit, "gallons") == 0) return 264.172;
    return -1;
}

void ConvertAllVolumes() {
    char fromUnit[20];
    double input;
    const char* units[] = { "liters", "milliliters", "gallons" };
    int numUnits = sizeof(units) / sizeof(units[0]);
    printf("\nAvailable Units: liters, milliliters, gallons\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    input = read_double("Enter the value to convert: ");
    double fromFactor = toCubicMeters(fromUnit);
    if (fromFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }
    printf("\n%.2lf %s equals:\n", input, fromUnit);
    for (int i = 0; i < numUnits; i++) {
        double toFactor = fromCubicMeters(units[i]);
        double result = input * fromFactor * toFactor;
        printf("%.2lf %s\n", result, units[i]);
    }
}

void WeightConvert() {
    char fromUnit[20], toUnit[20];
    double input, result;
    printf("\nAvailable Units: kilograms, grams, pounds, ounces, tonnes, milligrams\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    read_word("Enter the target unit: ", toUnit, sizeof(toUnit));
    input = read_double("Enter the value to convert: ");

    double fromFactor = toKilograms(fromUnit);
    double toFactor = fromKilograms(toUnit);

    if (fromFactor == -1 || toFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }

    result = input * fromFactor * toFactor;
    printf("%.2lf %s = %.2lf %s\n", input, fromUnit, result, toUnit);
}

double toKilograms(const char* unit) {
    if (strcmp(unit, "kilograms") == 0) return 1.0;
    if (strcmp(unit, "grams") == 0) return 0.001;
    if (strcmp(unit, "pounds") == 0) return 0.453592;
    if (strcmp(unit, "ounces") == 0) return 0.0283495;
    if (strcmp(unit, "tonnes") == 0) return 1000.0;
    if (strcmp(unit, "milligrams") == 0) return 0.000001;
    return -1;
}

double fromKilograms(const char* unit) {
    if (strcmp(unit, "kilograms") == 0) return 1.0;
    if (strcmp(unit, "grams") == 0) return 1000.0;
    if (strcmp(unit, "pounds") == 0) return 1 / 0.453592;
    if (strcmp(unit, "ounces") == 0) return 1 / 0.0283495;
    if (strcmp(unit, "tonnes") == 0) return 1 / 1000.0;
    if (strcmp(unit, "milligrams") == 0) return 1000000.0;
    return -1;
}

void ConvertAllWeights() {
    const char* units[] = { "kilograms", "grams", "pounds", "ounces", "tonnes", "milligrams" };
    int numUnits = sizeof(units) / sizeof(units[0]);
    char fromUnit[20];
    double input;
    printf("\nAvailable Units: kilograms, grams, pounds, ounces, tonnes, milligrams\n");
    read_word("Enter the source unit: ", fromUnit, sizeof(fromUnit));
    input = read_double("Enter the value to convert: ");

    double fromFactor = toKilograms(fromUnit);
    if (fromFactor == -1) {
        printf("Invalid unit entered. Please try again.\n");
        return;
    }
    printf("\nConversions from %.2lf %s:\n", input, fromUnit);
    for (int i = 0; i < numUnits; i++) {
        double toFactor = fromKilograms(units[i]);
        double result = input * fromFactor * toFactor;
        printf("%.2lf %s\n", result, units[i]);
    }
}

// ---------------------------------------------------------------------
// Matrix Operations -- Dimension validation and Gauss-Jordan inverse
// ---------------------------------------------------------------------

// Validates matrix dimensions to range [1, MAX_MATRIX]
static void read_matrix_dims(const char* prompt, int* rows, int* cols) {
    while (1) {
        printf("%s", prompt);
        if (scanf("%d %d", rows, cols) == 2 &&
            *rows >= 1 && *rows <= MAX_MATRIX &&
            *cols >= 1 && *cols <= MAX_MATRIX) {
            clear_input_buffer();
            return;
        }
        printf("Invalid dimensions. Values must be between 1 and %d.\n", MAX_MATRIX);
        clear_input_buffer();
    }
}

void inputMatrix(int matrix[MAX_MATRIX][MAX_MATRIX], int rows, int cols) {
    printf("Enter the elements of the matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("");
        }
    }
}

void printMatrix(int matrix[MAX_MATRIX][MAX_MATRIX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void addMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
}

void subtractMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = matrix1[i][j] - matrix2[i][j];
        }
    }
}

void multiplyMatrices(int matrix1[MAX_MATRIX][MAX_MATRIX], int matrix2[MAX_MATRIX][MAX_MATRIX], int result[MAX_MATRIX][MAX_MATRIX], int rows1, int cols1, int rows2, int cols2) {
    if (cols1 != rows2) {
        printf("Matrix multiplication is not possible: Number of columns of first matrix must equal number of rows of second matrix.\n");
        return;
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void determinantMenu() {
    int size;
    while (1) {
        size = read_int("Enter the size of the matrix (2 or 3): ");
        if (size == 2 || size == 3) break;
        printf("Only 2x2 and 3x3 matrices are supported.\n");
    }

    int matrix[MAX_MATRIX][MAX_MATRIX];
    printf("Enter the elements of the matrix:\n");
    inputMatrix(matrix, size, size);

    double determinant = calculateDeterminant(matrix, size);
    printf("The determinant of the matrix is: %.2f\n", determinant);

    int choice = read_int("Do you want to calculate the inverse of the matrix? (1 for Yes, 0 for No): ");
    if (choice == 1) {
        calculateInverse(matrix, size);
    }
}

double calculateDeterminant(int matrix[MAX_MATRIX][MAX_MATRIX], int size) {
    double determinant = 0;

    if (size == 2) {
        determinant = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    } else if (size == 3) {
        determinant = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1])
                    - matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0])
                    + matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
    }

    return determinant;
}

// Computes inverse via Gauss-Jordan elimination (works up to MAX_MATRIX)
void calculateInverse(int matrix[MAX_MATRIX][MAX_MATRIX], int size) {
    double a[MAX_MATRIX][2 * MAX_MATRIX];

    // Build augmented matrix [A | I]
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            a[i][j] = matrix[i][j];
        }
        for (int j = 0; j < size; j++) {
            a[i][size + j] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int col = 0; col < size; col++) {
        // Find pivot row for numerical stability
        int pivotRow = col;
        double maxVal = fabs(a[col][col]);
        for (int r = col + 1; r < size; r++) {
            if (fabs(a[r][col]) > maxVal) {
                maxVal = fabs(a[r][col]);
                pivotRow = r;
            }
        }

        if (maxVal < 1e-9) {
            printf("Matrix is singular and cannot have an inverse.\n");
            return;
        }

        if (pivotRow != col) {
            for (int k = 0; k < 2 * size; k++) {
                double tmp = a[col][k];
                a[col][k] = a[pivotRow][k];
                a[pivotRow][k] = tmp;
            }
        }

        double pivot = a[col][col];
        for (int k = 0; k < 2 * size; k++) {
            a[col][k] /= pivot;
        }

        for (int r = 0; r < size; r++) {
            if (r == col) continue;
            double factor = a[r][col];
            for (int k = 0; k < 2 * size; k++) {
                a[r][k] -= factor * a[col][k];
            }
        }
    }

    printf("The inverse of the matrix is:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%.4f ", a[i][size + j]);
        }
        printf("\n");
    }
}

void matrixOperations() {
    int choice;
    int matrix1[MAX_MATRIX][MAX_MATRIX], matrix2[MAX_MATRIX][MAX_MATRIX], result[MAX_MATRIX][MAX_MATRIX];
    int rows1, cols1, rows2, cols2;

    printf("Matrix Operations Menu:\n");
    printf("1. Addition\n");
    printf("2. Subtraction\n");
    printf("3. Multiplication\n");
    printf("4. Determinant\n");
    printf("5. Exit\n");
    choice = read_int("Enter your choice: ");

    switch (choice) {
        case 1:
            read_matrix_dims("Enter the number of rows and columns for the matrices: ", &rows1, &cols1);

            printf("Matrix 1:\n");
            inputMatrix(matrix1, rows1, cols1);
            printf("Matrix 2:\n");
            inputMatrix(matrix2, rows1, cols1);

            addMatrices(matrix1, matrix2, result, rows1, cols1);
            printf("Result of Addition:\n");
            printMatrix(result, rows1, cols1);
            break;

        case 2:
            read_matrix_dims("Enter the number of rows and columns for the matrices: ", &rows1, &cols1);

            printf("Matrix 1:\n");
            inputMatrix(matrix1, rows1, cols1);
            printf("Matrix 2:\n");
            inputMatrix(matrix2, rows1, cols1);

            subtractMatrices(matrix1, matrix2, result, rows1, cols1);
            printf("Result of Subtraction:\n");
            printMatrix(result, rows1, cols1);
            break;

        case 3:
            read_matrix_dims("Enter the number of rows and columns for Matrix 1: ", &rows1, &cols1);
            read_matrix_dims("Enter the number of rows and columns for Matrix 2: ", &rows2, &cols2);

            if (cols1 != rows2) {
                printf("Matrix multiplication is not possible: Number of columns of Matrix 1 must equal number of rows of Matrix 2.\n");
                return;
            }

            printf("Matrix 1:\n");
            inputMatrix(matrix1, rows1, cols1);
            printf("Matrix 2:\n");
            inputMatrix(matrix2, rows2, cols2);

            multiplyMatrices(matrix1, matrix2, result, rows1, cols1, rows2, cols2);
            printf("Result of Multiplication:\n");
            printMatrix(result, rows1, cols2);
            break;

        case 4:
            determinantMenu();
            break;

        case 5:
            return;

        default:
            printf("Invalid choice.\n");
    }
}

// ---------------------------------------------------------------------
// Plotting
// ---------------------------------------------------------------------

bool is_valid_input(double value) {
    return value >= 1 && value <= 10;  // Bounds check parameter scaling (1 to 10)
}

void plot_function(double (*func)(double, double), const char *title, double param) {
    printf("\n%s\n", title);

    for (int i = HEIGHT; i >= 0; i--) {
        double y = (i - HEIGHT / 2) * 1.0;
        for (int j = 0; j < WIDTH; j++) {
            double x = (j - WIDTH / 2) * 1.0;

            if (i == HEIGHT / 2 && j == WIDTH / 2) {
                printf("+");
            } else if (i == HEIGHT / 2) {
                printf("-");
            } else if (j == WIDTH / 2) {
                printf("|");
            } else {
                double value = func(x, param);

                if (value >= y - 0.5 && value <= y + 0.5) {
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
}

double linear_function(double x, double slope) {
    return slope * x;
}

double quadratic_function(double x, double a) {
    return a * x * x;
}

double sin_function(double x, double param) {
    return param * sin(x);
}

double cos_function(double x, double param) {
    return param * cos(x);
}

double tan_function(double x, double param) {
    return param * tan(x);
}

double cot_function(double x, double param) {
    if (tan(x) != 0) {
        return param / tan(x);
    } else {
        return 0;
    }
}

double log_function(double x, double param) {
    if (x > 0) {
        return param * log(x);
    } else {
        return NAN;
    }
}

void unitConverterMenu() {
    int choice;
    do {
        printf("Unit Converter Menu:\n");
        printf("1. Length Conversion\n");
        printf("2. Temperature Conversion\n");
        printf("3. Area Conversion\n");
        printf("4. Volume Conversion\n");
        printf("5. Weight Conversion\n");
        printf("6. Exit to Main Menu\n");
        choice = read_int("Enter your choice: ");

        switch (choice) {
            case 1:
                LengthConvert();
                break;
            case 2:
                TemperatureConvert();
                break;
            case 3:
                AreaConvert();
                break;
            case 4:
                volumeConvert();
                break;
            case 5:
                WeightConvert();
                break;
            case 6:
                printf("Returning to Main Menu...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void plotMenu() {
    int choice;
    double slope, a, param;

    printf("Select the function you want to plot:\n");
    printf("1. Linear Function (y = mx)\n");
    printf("2. Quadratic Function (y = ax^2)\n");
    printf("3. Sine Function (y = a * sin(x))\n");
    printf("4. Cosine Function (y = a * cos(x))\n");
    printf("5. Tangent Function (y = a * tan(x))\n");
    printf("6. Cotangent Function (y = a * cot(x))\n");
    printf("7. Logarithmic Function (y = a * log(x))\n");
    choice = read_int("Enter your choice (1-7): ");

    switch (choice) {
        case 1:
            do {
                slope = read_double("Enter slope (m) between 1 and 10: ");
            } while (!is_valid_input(slope));
            plot_function(linear_function, "Linear Function (y = mx)", slope);
            break;

        case 2:
            do {
                a = read_double("Enter coefficient a between 1 and 10: ");
            } while (!is_valid_input(a));
            plot_function(quadratic_function, "Quadratic Function (y = ax^2)", a);
            break;

        case 3:
            do {
                param = read_double("Enter parameter a (amplitude) between 1 and 10: ");
            } while (!is_valid_input(param));
            plot_function(sin_function, "Sine Function (y = a * sin(x))", param);
            break;

        case 4:
            do {
                param = read_double("Enter parameter a (amplitude) between 1 and 10: ");
            } while (!is_valid_input(param));
            plot_function(cos_function, "Cosine Function (y = a * cos(x))", param);
            break;

        case 5:
            do {
                param = read_double("Enter parameter a (amplitude) between 1 and 10: ");
            } while (!is_valid_input(param));
            plot_function(tan_function, "Tangent Function (y = a * tan(x))", param);
            break;

        case 6:
            do {
                param = read_double("Enter parameter a (amplitude) between 1 and 10: ");
            } while (!is_valid_input(param));
            plot_function(cot_function, "Cotangent Function (y = a * cot(x))", param);
            break;

        case 7:
            do {
                param = read_double("Enter parameter a (amplitude) between 1 and 10: ");
            } while (!is_valid_input(param));
            plot_function(log_function, "Logarithmic Function (y = a * log(x))", param);
            break;

        default:
            printf("Invalid choice.\n");
    }
}
