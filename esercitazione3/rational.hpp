#include <concepts> // for integral
#include <iostream> // for ostream


template<typename I> requires std::integral<I>
class rational{
    I num_;
    I den_;
private: // I put it in private because I don't want the user to call it,
 // but only the constructor and the operators
    bool is_nan() const { return num_ == 0 && den_ == 0; } // methods to check if the rational is NaN or inf 
    // in the way I defined them for integers. Integers don't have NaN or inf natively.
    bool is_inf() const { return num_ != 0 && den_ == 0; }

    I calculate_mcd(I a, I b) { // euclidian algorithm
    // Positive number to simplify
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    
    while (b != 0) {
        I resto = a % b;
        a = b;
        b = resto;
    }
    return a;
    }

    void simplify() {
        if (num_ == 0 && den_ == 0) {
            // I need this because otherwise if I input 0/0 it goes into the next if
            return; // I do not simplify
        }

        if (den_ == 0) {
            // I reduce to canonical forms: 1/0 for +inf and -1/0 for -inf
            if (num_ > 0) {
                num_ = 1;
            } else {
                num_ = -1;
            }
            return; // I do not simplify 
        }

        if (num_ == 0) { // 0/1 is the canonical form for 0
            den_ = 1; 
            return; // I do not simplify
        }
        if (den_ < 0) { // I want the denominator to be positive in order to avoid problems in operations
            //so if it is negative, I change the sign of both num and den
            num_ = -num_;
            den_ = -den_;
        }
        I mcd = calculate_mcd(num_, den_);
        num_ /= mcd; // this /= is the native operator in C, so it works for integers
        den_ /= mcd; // the one I define is for rationals
    }
public:
//default constructor
    rational() 
        : num_(I{0}), den_(I{1})  // I set the default rational to 0 for comodity
    {}
// user-defined constructor
    rational(const I& num, const I& den) 
        : num_(num), den_(den) 
    {simplify();} // whenever I create a rational, it is automatically simplified
    
// methods to call denominator and numerator
    I num() const { return num_; } // I need these methods when I want to extract den and num 
    I den() const { return den_; }
// overload of <<
    friend
    std::ostream& operator<<(std::ostream& os, const rational<I>& r) {
        if (r.den() == 0) {
            if (r.num() == 0) {
                os << "NaN";}
            else if (r.num() > 0) {
                os << "+inf";}
            else {
                os << "-inf";
            }
        }
    
        else if (r.den() == 1) {
            os << r.num(); // it prints only the numerator if den=1
    }
        else if (r.num() == 0) {
            os << 0; // it prints only 0 if num=0, even if den is not 1
        }
        else {
            os << r.num() << "/" << r.den(); // if none of the above cases, it prints num/den
        }
        return os;
    }
// increment operator
    rational& operator+=(const rational& other) {
        if (this->is_nan() || other.is_nan()) { //NaN + anything = NaN
        num_ = 0;
        den_ = 0;
        return *this;}

        if (this->is_inf() || other.is_inf()) { //inf case
        
            // both inf
            if (this->is_inf() && other.is_inf()) { //this->is_inf() is the same as (*this).is_inf()
                // opposite signs, result is NaN
                if (num_ != other.num()) {
                    num_ = 0;
                    den_ = 0;
                }
                // otherwise same sign, i keep the first inf
                return *this;
            }
            
            // only other inf, I make this other
            if (other.is_inf()) {
                num_ = other.num();
                den_ = 0;
            }
            
            // only this inf, I keep it
            return *this;
        }

        num_ = num_ * other.den() + other.num() * den_;
        den_ = den_ * other.den(); // it works because I simplify after.
        
        simplify();
        return *this;
    }
// sum operator
    rational operator+(const rational& other) const{
        rational ret = *this;
        ret+=other;
        return ret;
    }
// decrement operator
    rational& operator-=(const rational& other) {
        if (this->is_nan() || other.is_nan()) { //NaN - anything = NaN
        num_ = 0;
        den_ = 0;
        return *this;}

        if (this->is_inf() || other.is_inf()) { //inf case
        
            // both inf
            if (this->is_inf() && other.is_inf()) { //this->is_inf() is the same as (*this).is_inf()
                // same signs, result is NaN
                if (num_ == other.num()) {
                    num_ = 0;
                    den_ = 0;
                }
                // otherwise opposite sign, i keep the first inf
                return *this;
            }
            
            // only other inf, I make this other with a minus sign
            if (other.is_inf()) {
                num_ = -other.num();
                den_ = 0;
            }
            
            // only this inf, I keep it
            return *this;
        }
        num_ = num_ * other.den() - other.num() * den_;
        den_ = den_ * other.den(); 
        simplify();
        return *this;
    }
// subtraction operator
    rational operator-(const rational& other) const{
        rational ret = *this;
        ret-=other;
        return ret;
    }
// increment multiplication operator
    rational& operator*=(const rational& other) {
        if (this->is_nan() || other.is_nan()) { //NaN * anything = NaN
        num_ = 0;
        den_ = 0;
        return *this;}

        if (this->is_inf() || other.is_inf()) { //inf case
        
            // 0*inf=NaN
            if (num_ == 0 || other.num() == 0) {
                num_ = 0; den_ = 0;
                return *this;
            }
            // otherwise it's inf*inf or inf*number. It always makes inf, I need to check the sign.
            if (num_ * other.num() > 0) { // if the signs are the same, the result is +inf
                num_ = 1;
                den_= 0;
                return *this;
            } 
            else { // if the signs are different, the result is -inf
                num_ = -1;
                den_ = 0;
                return *this;
            }
        }
        num_ *= other.num();
        den_ *= other.den();
        simplify();
        return *this;
    }

// multiplication operator
    rational operator*(const rational& other) const{ 
        rational ret = *this;
        ret*=other;
        return ret;
    }
// increment division operator
    rational& operator/=(const rational& other) {
    // 1. NaN/anything = NaN, anything/NaN = NaN
    if (this->is_nan() || other.is_nan()) {
        num_ = 0; den_ = 0;
        return *this;
    }

    // both inf
    if (this->is_inf() || other.is_inf()) {
        
        // inf/inf=NaN
        if (this->is_inf() && other.is_inf()) {
            num_ = 0; den_ = 0;
            return *this;
        }
        
        // number/inf=0
        if (other.is_inf()) {
            num_ = 0; den_ = 1; // 0/1 stands for 0
            return *this;
        }
        
        // inf/number=inf, I keep the inf but I need to check the sign
        if (this->is_inf()) {
            if (num_ * other.num() > 0) { // if the signs are the same, the result is +inf
                num_ = 1; den_ = 0;
                return *this;
            } 
            else { // if the signs are different, the result is -inf
                num_ = -1; den_ = 0;
                return *this;
            }
        }
    }

    // number/0 = inf, but I need to check the sign
    if (other.num() == 0) {
        if (num_ == 0) { // 0/0=NaN
            num_ = 0; den_ = 0;
        } 
        else if (num_ > 0) {
            num_ = 1; den_ = 0;
        } 
        else { 
            num_ = -1; den_ = 0;
        }

        return *this;
    }
    num_ *= other.den();
    den_ *= other.num();
    simplify();
    return *this;
    }
// division operator
    rational operator/(const rational& other) const{ 
        rational ret = *this;
        ret/=other;
        return ret;
    }

};
