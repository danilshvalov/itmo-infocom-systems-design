#include <fstream>
#include <iostream>

class Log {
   public:
    void LogExecution(const std::string& message) {
        std::ofstream file("log.txt", std::ios_base::app);
        Logger(message, file);
    }

    static Log& GetLogger() {
        return logger_;
    }

   private:
    Log() = default;

    static void Logger(const std::string& message, std::ostream& out) {
        out << "Log entry: " << message << std::endl;
    }

    static Log logger_;
};

Log Log::logger_ = Log();

class Operation {
   public:
    static double Run(char code, int operand) {
        Log& log = Log::GetLogger();
        double result = 0;
        switch (code) {
            case '+': {
                result += operand;
                log.LogExecution("Сложение " + std::to_string(operand));
            } break;
            case '-': {
                result -= operand;
                log.LogExecution("Вычитание " + std::to_string(operand));
            } break;
        }
        return result;
    }
};

int main() {
    Log& log = Log::GetLogger();
    log.LogExecution("Метод main()");

    double result = Operation::Run('-', 35);
    result = Operation::Run('+', 30);
}
