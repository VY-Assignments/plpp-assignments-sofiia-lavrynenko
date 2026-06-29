#ifndef LINE_H
#define LINE_H

#include <string>

class Line
{
    public:
        virtual void print() const = 0;
        virtual std::string serialize() const = 0;
        virtual int getLength() = 0;
        virtual void insert(int index, std::string toInsert) = 0;
        virtual void insertWithReplace(int index, std::string toInsert) = 0;
        virtual void deleteAt(int index, int symbolsToDelete) = 0;
        virtual void copy(int index, int symbolsToCopy, std::string& clipboard) = 0;
        virtual std::vector<int> search(std::string search) = 0;
        virtual Line* clone() const = 0;
        virtual void changeStatus() = 0;

        static Line* deserialize(const std::string& serialized);

        virtual ~Line() = default;
};

class TextLine : public Line
{
    private:
        std::string _text;

    public:
        TextLine(const std::string& text);

        void print() const override;
        std::string serialize() const override;
        int getLength() override;
        void insert(int index, std::string toInsert) override;
        void insertWithReplace(int index, std::string toInsert) override;
        void deleteAt(int index, int symbolsToDelete) override;
        void copy(int index, int symbolsToCopy, std::string& clipboard) override;
        std::vector<int> search(std::string search) override;
        Line* clone() const override;
        void changeStatus() override;
}; 

class CheckLine : public Line
{
    private:
        std::string _text;
        bool _isChecked;

    public:
        CheckLine(const std::string& text, bool isChecked);

        void print() const override;
        std::string serialize() const override;
        int getLength() override;
        void insert(int index, std::string toInsert) override;
        void insertWithReplace(int index, std::string toInsert) override;
        void deleteAt(int index, int symbolsToDelete) override;
        void copy(int index, int symbolsToCopy, std::string& clipboard) override;
        std::vector<int> search(std::string search) override;
        Line* clone() const override;
        void changeStatus() override;
};

class ContactLine : public Line
{
    private:
        std::string _name;
        std::string _email;

    public:
        ContactLine(const std::string& name, const std::string& email);

        void print() const override;
        std::string serialize() const override;
        int getLength() override;
        void insert(int index, std::string toInsert) override;
        void insertWithReplace(int index, std::string toInsert) override;
        void deleteAt(int index, int symbolsToDelete) override;
        void copy(int index, int symbolsToCopy, std::string& clipboard) override;
        std::vector<int> search(std::string search) override;
        Line* clone() const override;
        void changeStatus() override;
};

#endif