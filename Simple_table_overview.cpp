#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <typeinfo>
#include <exception>
#include <string>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>
#include <string_view>



class NegativeValueException final : public std::exception{
public:
    NegativeValueException() noexcept = default;

    virtual const char* what() const noexcept override{
        return "invalid value while construction, min value is not allowed\n";
    }
    
    virtual ~NegativeValueException() override = default;
};

class Money final{
public:
    Money(int64_t money, int16_t cent = 0)
        : m_money(0), m_cent(0) {
        
        if(money < 0 || cent < 0)
            throw NegativeValueException{};

        m_money = money;

        if(cent > 99){
            m_money += cent / 100;
            m_cent = cent % 100;
        }
        else{
             m_cent = cent;
        }
    }

    size_t getDigit() const {

        int32_t mul {};
        if(m_cent == 0)
            mul = 1;
        else
            mul = 10;

        int64_t num_ = m_money * mul + m_cent;

        size_t digit {};
        while(num_ != 0){
            num_ /= 10;
            ++digit;
        }
        return digit;
    }

    Money operator+(const Money& m) noexcept{
        return Money{m_money + m.m_money, static_cast<int16_t>(m_cent + m.m_cent)};
    }

    Money operator-(const Money& m) noexcept{
        return Money{m_money - m.m_money, static_cast<int16_t>(m_cent - m.m_cent)};
    }

    bool operator==(const Money& m) noexcept{
        return m_money == m.m_money && m_cent == m.m_cent;
    }

    bool operator!=(const Money& m) noexcept{
        return m_money != m.m_money && m_cent != m.m_cent;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {

        std::stringstream buffer;

        buffer.imbue(std::locale(""));
        buffer << std::noshowpoint
               << std::fixed               
               << std::setprecision(2)
               << "Rp."
               << m.m_money + m.m_cent / 1000.0;

        os << buffer.str();
        return os;
    }
private:

    int64_t m_money;
    int16_t m_cent;

};

struct City{
    std::string name;
    int64_t population;
    Money cost;
};

struct Country{
    std::string name;
    std::vector<City> cities;
};

struct Tours{
    std::string title;
    std::vector<Country> countries;
};

void setConsoleWidth(size_t width, std::ios::fmtflags flag){
    std::cout.width(width);
    std::cout.setf(flag);
}

constexpr int32_t calculateMargin(int32_t value, int32_t margin = 1) noexcept{
    return value + margin * 2;
}

void printHorizontalLine(const std::array<int32_t,4>& arr){
    std::cout.put('+').fill('-');
    for(size_t i {}; i < arr.size(); ++i){
        std::cout.width(arr[i]);
        std::cout << '-' << '+';
    }
    std::cout.fill(' ');
    std::cout << '\n'; 
}

int32_t calculateLeftPadding(int32_t totalCol, size_t strSize) noexcept{
    return (totalCol - strSize) / 2;
}

int32_t calculateRightPadding(int32_t totalCol, size_t strSize) noexcept{
    return totalCol - (calculateLeftPadding(totalCol,strSize) + strSize);
}

void printHeaders(const std::array<std::string_view,4>& str, const std::array<int32_t,4>& colWidth) noexcept{
    std::cout.put('|');
    for(size_t i {}; i < str.size(); ++i){
        int32_t leftPadding = calculateLeftPadding(colWidth[i],str[i].size());
        int32_t rightPadding = calculateRightPadding(colWidth[i],str[i].size());

        std::cout << std::setw(leftPadding)
                  << ' '
                  << str[i]
                  << std::setw(rightPadding)
                  << ' '
                  << '|';
    }
    std::cout << '\n';
}

void printBody(const Tours& tour, std::array<int32_t,4>& colWidth){
    std::cout.setf(std::ios::left);
    for(size_t i {}; i < colWidth.size(); ++i){

        bool isnFirstRow {false};

        std::cout.put('|').width(colWidth[0]);
        std::cout << tour.countries[i].name;

        for(const auto& j : tour.countries[i].cities){
            if(isnFirstRow){
                std::cout.put('|').width(colWidth[0]);
                std::cout << ' ';
            }
            std::cout.put('|').width(colWidth[1]);
            std::cout << j.name
                        << '|'
                        << std::setw(colWidth[2])
                        << j.population
                        << '|'
                        << std::setw(colWidth[3])
                        << j.cost
                        << '|'
                        << '\n';
            isnFirstRow = true;
        } 
        isnFirstRow = false;
        if(i != colWidth.size() - 1){
        std::cout << std::setfill('-')
                  << std::setw(67)
                  << '-'
                  << std::setfill(' ')
                  << '\n';
        }
    }
    std::cout.unsetf(std::ios::left);
}

void printTitle(std::string_view str) noexcept{
    std::cout.width(calculateLeftPadding(62,str.size()));
    std::cout << ' ' << str << '\n';
}


int main(){

    Tours tours
        { "Tour Ticket Prices from Miami",
            {
                {
                    "Colombia", { 
                        { "Bogota", 8778000, {1500000}},
                        { "Cali", 2401000, {2000000}},
                        { "Medellin", 2464000, {2300000}},
                        { "Cartagena", 972000, {1700000}} 
                    },
                },
                {
                    "Brazil", { 
                        { "Rio De Janiero", 13500000, {2100000}},
                        { "Sao Paulo", 11310000, {2350000}},
                        { "Salvador", 18234000, {1450000}}
                    },
                },
                {
                    "Chile", { 
                        { "Valdivia", 260000, {1200000}}, 
                        { "Santiago", 7040000, {1230000}}
                    },
                },
                {   "Argentina", { 
                        { "Buenos Aires", 3010000, {2100000}} 
                    } 
                },
            }
    };

    //country,city,population,cost
    constexpr std::array<int32_t,4> baseWidth {11,14,13,16};

    constexpr std::array<std::string_view,4> headers {
        "Countries",
        "Cities",
        "Populations",
        "Costs"
    };

    std::array<int32_t,baseWidth.size()> columnWidth;
    for(size_t i {}; i < columnWidth.size(); ++i)
        columnWidth[i] = calculateMargin(baseWidth[i]);

    printTitle(tours.title);
    printHorizontalLine(columnWidth);
    printHeaders(headers,columnWidth);
    printHorizontalLine(columnWidth);
    printBody(tours,columnWidth);
    printHorizontalLine(columnWidth);

    return 0;
}
