#include "Amounts_view.h"

using std::string;

void Amounts_view::update_amount(const string& name, double amount)
{
    update_info(name, amount);
}

void Amounts_view::draw()
{
    plot("Amounts");
}
