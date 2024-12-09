#include <iostream>
#include <vector>

void Print(const std::vector<int>& values) {
    for (int x : values) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

class Progression {
   public:
    Progression(int first, int last, int h)
        : first_(first), last_(last), h_(h) {}

    void TemplateMethod() {
        InitializeProgression();
        Progress();
        Print(values_);
    }

   protected:
    void InitializeProgression() {
        values_ = {};
    }

    virtual void Progress() = 0;

    int first_;
    int last_;
    int h_;
    std::vector<int> values_;
};

class ArithmeticProgression : public Progression {
   public:
    ArithmeticProgression(int first, int last, int h)
        : Progression(first, last, h) {}

   private:
    void Progress() override {
        int first = first_;
        do {
            values_.push_back(first);
            first += h_;
        } while (first < last_);
    }
};

int main() {
    ArithmeticProgression progression(1, 10, 2);
    progression.TemplateMethod();
}
