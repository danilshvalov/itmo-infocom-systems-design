#include <algorithm>
#include <iostream>
#include <vector>

class StrategySort {
   public:
    StrategySort(std::string title) : title_(std::move(title)) {}

    const std::string& GetTitle() const {
        return title_;
    }

    void SetTitle(std::string title) {
        title_ = std::move(title);
    }

    virtual void Sort(std::vector<int>& array) = 0;

    virtual ~StrategySort() = default;

   private:
    std::string title_;
};

class InsertionSort : public StrategySort {
   public:
    InsertionSort() : StrategySort("Insertion sort") {}

    void Sort(std::vector<int>& array) override {
        for (size_t i = 1; i < array.size(); ++i) {
            size_t j = 0;
            int buffer = array[i];
            for (j = i - 1; j >= 0; --j) {
                if (array[j] < buffer) {
                    break;
                }
                array[j + 1] = array[j];
            }
            array[j + 1] = buffer;
        }
    }
};

class SelectionSort : public StrategySort {
   public:
    SelectionSort() : StrategySort("Selection sort") {}

    void Sort(std::vector<int>& array) override {
        for (size_t i = 0; i < array.size() - 1; ++i) {
            size_t k = i;
            for (size_t j = i + 1; j < array.size(); ++j) {
                if (array[k] > array[j]) {
                    k = j;
                }
            }
            if (k != i) {
                std::swap(array[k], array[i]);
            }
        }
    }
};

class Context {
   public:
    Context(std::unique_ptr<StrategySort> sort, std::vector<int> array)
        : sort_(std::move(sort)), array_(std::move(array)) {}

    void Sort() {
        sort_->Sort(array_);
    }

    void PrintArray() const {
        std::cout << "Strategy: " << sort_->GetTitle() << std::endl;
        for (size_t i = 0; i < array_.size(); ++i) {
            std::cout << array_[i] << ' ';
        }
        std::cout << std::endl;
    }

   private:
    std::unique_ptr<StrategySort> sort_;
    std::vector<int> array_;
};

int main() {
    std::vector<int> array = {31, 15, 10, 2, 4, 2, 14, 23, 12, 66};
    auto sort = std::make_unique<SelectionSort>();
    Context context{std::move(sort), std::move(array)};

    context.Sort();
    context.PrintArray();
}
