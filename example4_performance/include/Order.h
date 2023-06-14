#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct Order
{
    std::string customer_id;
    std::string order_id;
    std::vector<std::string> product_ids;
    double total_cost;
};

// Provide to_json and from_json functions for our Order struct
void to_json(nlohmann::json &j, const Order &o)
{
    j = nlohmann::json{{"customer_id", o.customer_id}, {"order_id", o.order_id}, {"product_ids", o.product_ids}, {"total_cost", o.total_cost}};
}

void from_json(const nlohmann::json &j, Order &o)
{
    j.at("customer_id").get_to(o.customer_id);
    j.at("order_id").get_to(o.order_id);
    j.at("product_ids").get_to(o.product_ids);
    j.at("total_cost").get_to(o.total_cost);
}
