#include "../inc/Payment.h"
#include <utility>
using namespace std;

Payment::Payment(string payment_id,
                 double amount,
                 string payment_date,
                 string status)
    : payment_id(payment_id),
      amount(amount),
      payment_date(payment_date),
      status(status)
{}

const string &Payment::getPaymentId() const
{
    return payment_id;
}

double Payment::getAmount() const
{
    return amount;
}

const string &Payment::getPaymentDate() const
{
    return payment_date;
}

const string &Payment::getStatus() const
{
    return status;
}

bool Payment::processPayment(double &user_credit)
{
    if (amount <= 0.0 || user_credit < amount || status == "Completed" || status == "Refunded")
    {
        status = "Failed";
        return false;
    }

    user_credit -= amount;
    status = "Completed";
    return true;
}

bool Payment::processRefund(double &user_credit)
{
    if (status != "Completed")
    {
        return false;
    }

    user_credit += amount;
    status = "Refunded";
    return true;
}
