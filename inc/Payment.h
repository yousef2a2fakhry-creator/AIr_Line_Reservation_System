#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

class Payment
{
private:
    std::string payment_id;
    double amount;
    std::string payment_date;
    std::string status;

public:
    Payment(std::string payment_id,
            double amount,
            std::string payment_date,
            std::string status = "Failed");

    const std::string &getPaymentId() const;
    double getAmount() const;
    const std::string &getPaymentDate() const;
    const std::string &getStatus() const;

    bool processPayment(double &user_credit);
    bool processRefund(double &user_credit);
};

#endif
