#ifndef LINE_H
#define LINE_H


#include <string>

class Line
{
    public:
        virtual void print() const = 0;
        virtual std::string serialize() const = 0;

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
};

#endif