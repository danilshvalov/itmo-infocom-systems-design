#include <iostream>
#include <memory>
#include <vector>

class ArithmeticUnit {
   public:
    double GetRegister() const {
        return register_;
    }

    void Run(char operation, double operand) {
        switch (operation) {
            case '+': {
                register_ += operand;
            } break;
            case '-': {
                register_ -= operand;
            } break;
            case '*': {
                register_ *= operand;
            } break;
            case '/': {
                register_ /= operand;
            } break;
            default: {
                std::cout << "Incorrect operation " << '"' << operation << '"'
                          << std::endl;
            } break;
        }
    }

   private:
    double register_;
};

class Command {
   public:
    Command(ArithmeticUnit& unit, double operand)
        : unit_(unit), operand_(operand) {}

    virtual void Execute() = 0;
    virtual void UnExecute() = 0;

    virtual ~Command() = default;

   protected:
    ArithmeticUnit& unit_;
    double operand_;
};

class ControlUnit {
   public:
    void AddCommand(std::unique_ptr<Command> command) {
        commands_.push_back(std::move(command));
    }

    void ExecuteCommand() {
        if (command_index_ < commands_.size()) {
            commands_[command_index_++]->Execute();
        }
    }

    void Undo(size_t steps_backward = 1) {
        size_t index = command_index_;
        while (steps_backward > 0 && index > 0 && index - 1 < commands_.size()) {
            commands_[--index]->UnExecute();
            --steps_backward;
        }
    }

    void Redo(size_t steps_backward = 1) {
        size_t index = command_index_;
        while (steps_backward > 0 && index > 0 && index - 1 < commands_.size()) {
            commands_[--index]->Execute();
            --steps_backward;
        }
    }

   private:
    std::vector<std::unique_ptr<Command>> commands_;
    size_t command_index_ = 0;
};

class AddCommand : public Command {
    using Command::Command;

    void Execute() override {
        unit_.Run('+', operand_);
    }

    void UnExecute() override {
        unit_.Run('-', operand_);
    }
};

class SubCommand : public Command {
    using Command::Command;

    void Execute() override {
        unit_.Run('-', operand_);
    }

    void UnExecute() override {
        unit_.Run('+', operand_);
    }
};

class MulCommand : public Command {
    using Command::Command;

    void Execute() override {
        unit_.Run('*', operand_);
    }

    void UnExecute() override {
        unit_.Run('/', operand_);
    }
};

class DivCommand : public Command {
    using Command::Command;

    void Execute() override {
        unit_.Run('/', operand_);
    }

    void UnExecute() override {
        unit_.Run('*', operand_);
    }
};

class Calculator {
   public:
    double Add(double operand) {
        return Run(std::make_unique<AddCommand>(arithmetic_unit_, operand));
    }

    double Sub(double operand) {
        return Run(std::make_unique<SubCommand>(arithmetic_unit_, operand));
    }

    double Mul(double operand) {
        return Run(std::make_unique<MulCommand>(arithmetic_unit_, operand));
    }

    double Div(double operand) {
        return Run(std::make_unique<DivCommand>(arithmetic_unit_, operand));
    }

    double Undo(size_t steps_backward = 1) {
        control_unit_.Undo(steps_backward);
        return arithmetic_unit_.GetRegister();
    }

    double Redo(size_t steps_backward = 1) {
        control_unit_.Redo(steps_backward);
        return arithmetic_unit_.GetRegister();
    }

   private:
    double Run(std::unique_ptr<Command> command) {
        control_unit_.AddCommand(std::move(command));
        control_unit_.ExecuteCommand();
        return arithmetic_unit_.GetRegister();
    }

    ArithmeticUnit arithmetic_unit_;
    ControlUnit control_unit_;
};

int main() {
    Calculator calc;

    std::cout << calc.Add(5) << std::endl;
    std::cout << calc.Add(4) << std::endl;
    std::cout << calc.Add(3) << std::endl;
    std::cout << calc.Redo(2) << std::endl;
    std::cout << calc.Undo(3) << std::endl;
}
